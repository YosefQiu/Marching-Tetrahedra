#pragma once
#include "ggl.h"


struct TetConfig
{
    int A;
    int B;
    int C;
    int D;
};

class MarchingTetrahedra
{
public:
    MarchingTetrahedra() = default;
    virtual ~MarchingTetrahedra() = default;
public:
    using PtrUGrid      = vtkSmartPointer<vtkUnstructuredGrid>;
    using PtrGrid       = vtkSmartPointer<vtkAppendPolyData>;
    static vtkIdType AddMidpoint(vtkSmartPointer<vtkPoints> points, vtkIdType idx1, vtkIdType idx2);
    static vtkIdType AddCentroid(vtkSmartPointer<vtkPoints> points, vtkIdType idx1, vtkIdType idx2, vtkIdType idx3);
    static vtkIdType AddTetrahedronCentroid(vtkSmartPointer<vtkPoints> points, vtkIdType idx1, vtkIdType idx2, vtkIdType idx3, vtkIdType idx4);
    static PtrUGrid DoCase1(vtkSmartPointer<vtkPoints> points, TetConfig config, PtrUGrid case1_tet = nullptr);
    static PtrUGrid DoCase2(vtkSmartPointer<vtkPoints> points, TetConfig config, PtrUGrid case2_tet = nullptr);
    static PtrUGrid DoCase3(vtkSmartPointer<vtkPoints> points, TetConfig config, PtrUGrid case3_tet = nullptr);
    static PtrUGrid DoCase4(vtkSmartPointer<vtkPoints> points, TetConfig config, PtrUGrid case4_tet = nullptr);
    static PtrGrid AddCriticalPoint(std::vector<int>& critical_points, PtrUGrid const& cube_unstructuredGrid, float radius = 1.5f);
};
