#pragma once
#include "ggl.h"

class VTKGrid
{
public:
    VTKGrid() = default;
    virtual ~VTKGrid() = default;
public:
    using PtrUGrid      = vtkSmartPointer<vtkUnstructuredGrid>;
    static PtrUGrid CreateCubeMesh(const int W, const int H, const int D);
    static PtrUGrid ExtractSingleCell(PtrUGrid originalGrid, vtkIdType cellId);
    static PtrUGrid SplitCubeToTetrahedra(PtrUGrid cubeGrid);
    static std::vector<int> GetTetrahedronFaceIndices(int tetrahedronIndex);
    static std::array<int, 4> toLabelIndex(std::array<int, 4> tetrahedronIndex);
};

