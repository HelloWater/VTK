set(TestChartUnicode_ARGS
  DATA{../Data/Fonts/DejaVuSans.ttf}
  -E 25
  )
set(TestChartsOn3D_ARGS
  -E 16
  )
set(TestColorTransferFunction_ARGS
  -E 80
  )
set(TestContextUnicode_ARGS
  DATA{../Data/Fonts/DejaVuSans.ttf}
  )
set(TestControlPointsHandleItem_ARGS
  -E 30
  )
set(TestLinePlot_ARGS
  -E 25
  )
set(TestLinePlot2_ARGS
  -E 25
  )
set(TestLinePlotInteraction_ARGS
  -E 25
  )
set(TestMultipleRenderers_ARGS
  -E 25
  )
set(TestMultipleScalarsToColors_ARGS
  -E 25
  )
set(TestParallelCoordinates_ARGS
  -E 15
  )
set(TestParallelCoordinatesDouble_ARGS
  -E 15
  )

vtk_add_test_cxx(vtkChartsCoreCxxTests tests
  TestAdjustLowerBoundForLogPlot.cxx
  TestContextScene.cxx,NO_DATA,NO_VALID
  TestColorTransferControlPointsItem.cxx,NO_DATA,NO_VALID
  TestCompositeControlPointsItem.cxx,NO_DATA,NO_VALID
  TestControlPointsItemEvents.cxx,NO_DATA,NO_VALID
  TestAreaPlot.cxx
  TestAxes.cxx
  TestAxes2.cxx
  TestBagPlot.cxx
  TestBarGraph.cxx
  TestBarGraphHorizontal.cxx
  TestBarGraphSelection.cxx
  TestBoxPlot.cxx
  TestBoxPlot2.cxx
  TestCategoryLegend.cxx
  TestColorTransferFunction.cxx
  TestChartBadPoints.cxx
  TestChartDouble.cxx
  TestChartDoubleColors.cxx
  TestChartDoubleColorsOpaque.cxx
  TestChartMatrix.cxx
  TestChartMatrix2.cxx
  TestChartMatrix3.cxx
  TestChartPicking.cxx,NO_DATA,NO_VALID
  TestChartTileScaling.cxx
  TestChartUnicode.cxx
  TestChartsOn3D.cxx
  TestChartXYInvertedAxis.cxx
  TestChartXYZ.cxx
  TestChartXYZMarginsRotations.cxx
  TestChartXYZOuterEdgeLabelling.cxx
  TestChartXYZUnconstrained.cxx
  TestContext.cxx
  TestContextArea.cxx
  TestContextAreaFixedAspect.cxx
  TestContextAreaFixedMargins.cxx
  TestContextAreaFixedRect.cxx
  TestContextImage.cxx
  TestContextItemStacking.cxx
  TestContextUnicode.cxx
  TestControlPointsHandleItem.cxx
  TestDiagram.cxx
  TestFunctionalBagPlot.cxx
  TestHistogram2D.cxx
  TestInteractiveChartXYZ.cxx
  TestLegendHiddenPlots.cxx
  TestLinePlot.cxx
  TestLinePlotDouble.cxx
  TestLinePlotDouble2.cxx
  TestLinePlot3D.cxx
  TestLinePlotAxisFonts.cxx
  TestLinePlot2.cxx
  TestLinePlotInteraction.cxx
  TestLinePlotSelection.cxx
  TestLinePlotSelection2.cxx
  TestLinePlotSelectionMultiBlock.cxx
  TestMultipleChartRenderers.cxx
  TestMultipleRenderers.cxx
  TestMultipleScalarsToColors.cxx
  TestParallelCoordinates.cxx
  TestParallelCoordinatesDouble.cxx
  TestPieChart.cxx
  TestPlotBarRangeHandlesItem.cxx,NO_DATA,NO_VALID
  TestPlotMatrix.cxx
  TestPlotRangeHandlesItem.cxx,NO_DATA,NO_VALID
  TestPropItem.cxx
  TestRangeHandlesItemEvents.cxx,NO_DATA,NO_VALID
  TestScalarsToColors.cxx
  TestScatterPlot.cxx
  TestScatterPlotMatrix.cxx
  TestScatterPlotMatrixVehicles.cxx
  TestScatterPlotMatrixVisible.cxx
  TestScientificPlot.cxx
  TestStackedBarGraph.cxx
  TestStackedPlot.cxx
  TestSurfacePlot.cxx
  TestZoomAxis.cxx
  TestZoomIndividualAxis.cxx
  )
vtk_test_cxx_executable(vtkChartsCoreCxxTests tests
  DISABLE_FLOATING_POINT_EXCEPTIONS
  RENDERING_FACTORY
  )
