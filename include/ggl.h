#pragma once
//c++ header file
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <iomanip>
#include <chrono>
#include <limits>
#include <numbers>
#include <cmath>

//c header file
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <atomic>

#define LOADING_VTK             1

#if LOADING_VTK == 1
    #include <vtkAppendFilter.h>
    #include <vtkSphereSource.h>
    #include <vtkUnstructuredGrid.h>
    #include <vtkUnstructuredGridReader.h>
    #include <vtkXMLUnstructuredGridReader.h>
    #include <vtkXMLUnstructuredGridWriter.h>
    #include <vtkXMLPolyDataWriter.h>
    #include <vtkActor.h>
    #include <vtkXMLImageDataReader.h>
    #include <vtkImageData.h>
    #include <vtkCamera.h>
    #include <vtkPointData.h>
    #include <vtkDataSetMapper.h>
    #include <vtkCallbackCommand.h>
    #include <vtkNamedColors.h>
    #include <vtkNew.h>
    #include <vtkProperty.h>
    #include <vtkRenderWindow.h>
    #include <vtkRenderWindowInteractor.h>
    #include <vtkRenderer.h>
    #include <vtkSmartPointer.h>
    #include <vtkPoints.h>
    #include <vtkLine.h>
    #include <vtkCellArray.h>
    #include <vtkAppendFilter.h>
    #include <vtkSphereSource.h>
    #include <vtkAppendPolyData.h>
    #include <vtkTetra.h>
    #include <vtkPyramid.h>
    #include <vtkLookupTable.h>
    #include <vtkGeometryFilter.h>
    #include <vtkCleanPolyData.h>
    #include <vtkUnsignedCharArray.h>
    #include <vtkTransform.h>
    #include <vtkTransformFilter.h>
    #include <vtkAxesActor.h> 
    #include <vtkTriangle.h>
    #include <vtkMergePoints.h>
    #include <vtkWedge.h>
    #include <vtkGenericCell.h>
    #include <vtkCellData.h>
    #include <vtkHexahedron.h>
#endif

inline std::string ToVertex(const int& value)
{
    std::string res;
    switch (value)
    {   
    case 0 : res = "A"; break;
    case 1 : res = "B"; break;
    case 2 : res = "C"; break;
    case 3 : res = "D"; break;
    case 4 : res = "E"; break;
    case 5 : res = "F"; break;
    case 6 : res = "G"; break;
    case 7 : res = "H"; break;
    default: res = "NaN"; break;
    }
    return res;
}

        // {D, C, G, F},
        // {D, C, B, F},
        // {D, H, G, F},
        // {D, B, A, F},
        // {D, H, E, F},
        // {D, A, E, F}

inline std::vector<int> GetTetIndex(const int& value)
{
    std::vector<int> index;
    if (value < 0 || value > 5)
    {
        std::cerr << "Index out of range" << std::endl;
        return index;
    }
    int A = 0; int B = 1; int C = 2; int D = 3;
    int E = 4; int F = 5; int G = 6; int H = 7;
    switch (value)
    {
    case 0 : index = {D, C, G, F}; break;
    case 1 : index = {D, C, B, F}; break;
    case 2 : index = {D, H, G, F}; break;
    case 3 : index = {D, B, A, F}; break;
    case 4 : index = {D, H, E, F}; break;
    case 5 : index = {D, A, E, F}; break;
    }
    if (index.size() != 4)
    {
        std::cerr << "Index out of range" << std::endl;
        return {};
    }
    return index;
}