/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPDescriptiveStatistics.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2011 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
  -------------------------------------------------------------------------*/
/**
 * @class   vtkPDescriptiveStatistics
 * @brief   A class for parallel univariate descriptive statistics
 *
 * vtkPDescriptiveStatistics is vtkDescriptiveStatistics subclass for parallel datasets.
 * It learns and derives the global statistical model on each node, but assesses each
 * individual data points on the node that owns it.
 *
 * @note Kurtosis formula in "Formulas for robust, one-pass parallel computation
 * of covariances and Arbitrary-Order Statistical Moments", P. Pébay, 2008,  has an error
 * (equation 1.6 in the paper). A correct formula can be found in
 * "Formulas for the Computation of Higher-
 * Order Central Moments", P. Pébay, T.B. Terriberry, H. Kolla, J. Bennett, 2016, at equation 3.6.
 * The latter one is being used to compute the 4th moment from partial ones across ranks.
 *
 * @par Thanks:
 * Thanks to Philippe Pebay from Sandia National Laboratories for implementing this class.
 */

#ifndef vtkPDescriptiveStatistics_h
#define vtkPDescriptiveStatistics_h

#include "vtkDescriptiveStatistics.h"
#include "vtkFiltersParallelStatisticsModule.h" // For export macro

class vtkMultiBlockDataSet;
class vtkMultiProcessController;

class VTKFILTERSPARALLELSTATISTICS_EXPORT vtkPDescriptiveStatistics
  : public vtkDescriptiveStatistics
{
public:
  static vtkPDescriptiveStatistics* New();
  vtkTypeMacro(vtkPDescriptiveStatistics, vtkDescriptiveStatistics);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Get/Set the multiprocess controller. If no controller is set,
   * single process is assumed.
   */
  virtual void SetController(vtkMultiProcessController*);
  vtkGetObjectMacro(Controller, vtkMultiProcessController);
  ///@}

  /**
   * Execute the parallel calculations required by the Learn option.
   */
  void Learn(vtkTable* inData, vtkTable* inParameters, vtkMultiBlockDataSet* outMeta) override;

protected:
  vtkPDescriptiveStatistics();
  ~vtkPDescriptiveStatistics() override;

  vtkMultiProcessController* Controller;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkPDescriptiveStatistics(const vtkPDescriptiveStatistics&) = delete;
  void operator=(const vtkPDescriptiveStatistics&) = delete;
};

#endif
