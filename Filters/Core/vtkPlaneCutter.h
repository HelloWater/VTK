/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPlaneCutter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPlaneCutter
 * @brief   cut any dataset with a plane and generate a
 * polygonal cut surface
 *
 * vtkPlaneCutter is a specialization of the vtkCutter algorithm to cut a
 * dataset grid with a single plane. It is designed for performance and an
 * exploratory, fast workflow. It produces output polygons that result from
 * cutting the input dataset with the specified plane.
 *
 * This algorithm is fast because it is threaded, and may build (in a
 * preprocessing step) a spatial search structure that accelerates the plane
 * cuts. The search structure, which is typically a sphere tree, is used to
 * quickly cull candidate cells.  (Note that certain types of input data are
 * delegated to other, internal classes; for example image data is delegated
 * to vtkFlyingEdgesPlaneCutter.)
 *
 * Because this filter may build an initial data structure during a
 * preprocessing step, the first execution of the filter may take longer than
 * subsequent operations. Typically the first execution is still faster than
 * vtkCutter (especially with threading enabled), but for certain types of
 * data this may not be true. However if you are using the filter to cut a
 * dataset multiple times (as in an exploratory or interactive workflow) this
 * filter typically works well.
 *
 * @warning
 * This filter chooses the output type based on the input type.
 * 1) if input is vtkDataSet, output is vtkPolyData.
 * 2) if input is vtkPartitionedDataSet, output is vtkPartitionedDataSet.
 * 3) if input is vtkPartitionedDataSetCollection, output is vtkPartitionedDataSetCollection.
 * 4) if input is vtkUniformGridAMR, output is vtkPartitionedDataSetCollection.
 * 5) if input is vtkMultiBlockDataSet, output is vtkMultiBlockDataSet.
 *
 * @warning
 * This filter produces non-merged, potentially coincident points for all
 * input dataset types except vtkImageData (which uses
 * vtkFlyingEdgesPlaneCutter under the hood - which does merge points).
 *
 * @warning
 * This filter delegates to vtkFlyingEdgesPlaneCutter to process image
 * data, but output and input have been standardized when possible.
 *
 * @warning
 * This class has been threaded with vtkSMPTools. Using TBB or other
 * non-sequential type (set in the CMake variable
 * VTK_SMP_IMPLEMENTATION_TYPE) may improve performance significantly.
 *
 * @sa
 * vtkFlyingEdgesPlaneCutter vtk3DLinearGridPlaneCutter vtkCutter vtkPlane
 */

#ifndef vtkPlaneCutter_h
#define vtkPlaneCutter_h

#include "vtkDataObjectAlgorithm.h"
#include "vtkFiltersCoreModule.h" // For export macro
#include "vtkSmartPointer.h"      // For SmartPointer
#include <map>                    // For std::map

class vtkCellArray;
class vtkCellData;
class vtkImageData;
class vtkMultiBlockDataSet;
class vtkMultiPieceDataSet;
class vtkPartitionedDataSet;
class vtkPartitionedDataSetCollection;
class vtkPlane;
class vtkPointData;
class vtkPoints;
class vtkPolyData;
class vtkSphereTree;
class vtkStructuredGrid;
class vtkUniformGridAMR;
class vtkUnstructuredGrid;

class VTKFILTERSCORE_EXPORT vtkPlaneCutter : public vtkDataObjectAlgorithm
{
public:
  ///@{
  /**
   * Standard construction and print methods.
   */
  static vtkPlaneCutter* New();
  vtkTypeMacro(vtkPlaneCutter, vtkDataObjectAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  ///@}

  /**
   * The modified time depends on the delegated cut plane.
   */
  vtkMTimeType GetMTime() override;

  ///@{
  /**
   * Specify the plane (an implicit function) to perform the cutting. The
   * definition of the plane (its origin and normal) is controlled via this
   * instance of vtkPlane.
   */
  virtual void SetPlane(vtkPlane*);
  vtkGetObjectMacro(Plane, vtkPlane);
  ///@}

  ///@{
  /**
   * Set/Get the computation of normals. The normal generated is simply the
   * cut plane normal. The normal, if generated, is defined by cell data
   * associated with the output polygons. By default computing of normals is
   * disabled.
   */
  vtkSetMacro(ComputeNormals, bool);
  vtkGetMacro(ComputeNormals, bool);
  vtkBooleanMacro(ComputeNormals, bool);
  ///@}

  ///@{
  /**
   * Indicate whether to interpolate attribute data. By default this is
   * enabled. Note that both cell data and point data is interpolated and
   * outputted, except for image data input where only point data are outputted.
   */
  vtkSetMacro(InterpolateAttributes, bool);
  vtkGetMacro(InterpolateAttributes, bool);
  vtkBooleanMacro(InterpolateAttributes, bool);
  ///@}

  ///@{
  /**
   * Indicate whether to generate polygons instead of triangles when cutting
   * structured and rectilinear grid.
   * No effect with other kinds of inputs, enabled by default.
   */
  vtkSetMacro(GeneratePolygons, bool);
  vtkGetMacro(GeneratePolygons, bool);
  vtkBooleanMacro(GeneratePolygons, bool);
  ///@}

  ///@{
  /**
   * Indicate whether to build the sphere tree. Computing the sphere
   * will take some time on the first computation
   * but if the input does not change, the computation of all further
   * slice will be much faster. Default is on.
   */
  vtkSetMacro(BuildTree, bool);
  vtkGetMacro(BuildTree, bool);
  vtkBooleanMacro(BuildTree, bool);
  ///@}

  ///@{
  /**
   * Indicate whether to build tree hierarchy. Computing the tree
   * hierarchy can take some time on the first computation but if
   * the input does not change, the computation of all further
   * slice will be faster. Default is on.
   */
  vtkSetMacro(BuildHierarchy, bool);
  vtkGetMacro(BuildHierarchy, bool);
  vtkBooleanMacro(BuildHierarchy, bool);
  ///@}

protected:
  vtkPlaneCutter();
  ~vtkPlaneCutter() override;

  vtkPlane* Plane;
  bool ComputeNormals;
  bool InterpolateAttributes;
  bool GeneratePolygons;
  bool BuildTree;
  bool BuildHierarchy;

  // Helpers
  vtkSphereTree* GetSphereTree(vtkDataSet*);
  std::map<vtkDataSet*, vtkSmartPointer<vtkSphereTree>> SphereTrees;
  struct vtkInputInfo
  {
    vtkDataObject* Input;
    vtkMTimeType LastMTime;

    vtkInputInfo()
      : Input(nullptr)
      , LastMTime(0)
    {
    }
    vtkInputInfo(vtkDataObject* input, vtkMTimeType mtime)
      : Input(input)
      , LastMTime(mtime)
    {
    }
  };
  vtkInputInfo InputInfo;

  // Pipeline-related methods
  int RequestDataObject(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int RequestUpdateExtent(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;
  int FillOutputPortInformation(int port, vtkInformation* info) override;

  int ExecuteMultiBlockDataSet(vtkMultiBlockDataSet* input, vtkMultiBlockDataSet* output);
  int ExecuteUniformGridAMR(vtkUniformGridAMR* input, vtkPartitionedDataSetCollection* output);
  int ExecutePartitionedDataCollection(
    vtkPartitionedDataSetCollection* input, vtkPartitionedDataSetCollection* output);
  int ExecutePartitionedData(
    vtkPartitionedDataSet* input, vtkPartitionedDataSet* output, bool copyStructure);
  int ExecuteDataSet(vtkDataSet* input, vtkSphereTree* tree, vtkPolyData* output);

  static void AddNormalArray(double* planeNormal, vtkPolyData* polyData);

private:
  vtkPlaneCutter(const vtkPlaneCutter&) = delete;
  void operator=(const vtkPlaneCutter&) = delete;
};

#endif
