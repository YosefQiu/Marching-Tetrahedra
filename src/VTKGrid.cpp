#include "VTKGrid.h"
#include "Log.hpp"

VTKGrid::PtrUGrid VTKGrid::CreateCubeMesh(const int W, const int H, const int D)
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

    for (int z = 0; z < D; ++z) 
    {
        for (int y = 0; y < H; ++y) 
        {
            for (int x = 0; x < W; ++x) 
            {
                points->InsertNextPoint(x, y, z);
            }
        }
    }


    PtrUGrid unstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    unstructuredGrid->SetPoints(points);


    for (int z = 0; z < D - 1; ++z) 
    {
        for (int y = 0; y < H - 1; ++y) 
        {
            for (int x = 0; x < W - 1; ++x) 
            {
                vtkSmartPointer<vtkHexahedron> hex = vtkSmartPointer<vtkHexahedron>::New();
                hex->GetPointIds()->SetId(0, (z * H * W) + (y * W) + x);
                hex->GetPointIds()->SetId(1, (z * H * W) + (y * W) + (x + 1));
                hex->GetPointIds()->SetId(2, (z * H * W) + ((y + 1) * W) + (x + 1));
                hex->GetPointIds()->SetId(3, (z * H * W) + ((y + 1) * W) + x);
                hex->GetPointIds()->SetId(4, ((z + 1) * H * W) + (y * W) + x);
                hex->GetPointIds()->SetId(5, ((z + 1) * H * W) + (y * W) + (x + 1));
                hex->GetPointIds()->SetId(6, ((z + 1) * H * W) + ((y + 1) * W) + (x + 1));
                hex->GetPointIds()->SetId(7, ((z + 1) * H * W) + ((y + 1) * W) + x);
                unstructuredGrid->InsertNextCell(hex->GetCellType(), hex->GetPointIds());
            }
        }
    }

    Debug("Creating a rectangular mesh with W = %d, H = %d, D = %d", W, H, D);
    return unstructuredGrid;
}

VTKGrid::PtrUGrid VTKGrid::ExtractSingleCell(PtrUGrid originalGrid, vtkIdType cellId)
{
	auto newGrid    = vtkSmartPointer<vtkUnstructuredGrid>::New();
    auto newPoints  = vtkSmartPointer<vtkPoints>::New();
    auto newCells   = vtkSmartPointer<vtkCellArray>::New();

    vtkCell* cell = originalGrid->GetCell(cellId);

    // add point - > newPoints
    for (vtkIdType i = 0; i < cell->GetNumberOfPoints(); ++i) 
    {
        double p[3];
        cell->GetPoints()->GetPoint(i, p);
        vtkIdType globalPointId = cell->GetPointId(i);
        newPoints->InsertNextPoint(p);
      
    }
    newGrid->SetPoints(newPoints);


    vtkSmartPointer<vtkCell> newCell;
    switch(cell->GetCellType()) 
    {
        case VTK_HEXAHEDRON:
            newCell = vtkSmartPointer<vtkHexahedron>::New();
            break;
        case VTK_TETRA:
            newCell = vtkSmartPointer<vtkTetra>::New();
            break;
        case VTK_PYRAMID:
            newCell = vtkSmartPointer<vtkPyramid>::New();
            break;
        case VTK_WEDGE:
            newCell = vtkSmartPointer<vtkWedge>::New();
            break;
        default:
            Debug("Unsupported cell type: %d", cell->GetCellType());
            return nullptr;
    }

    for (vtkIdType i = 0; i < cell->GetNumberOfPoints(); ++i) 
    {
        newCell->GetPointIds()->SetId(i, i); 
    }

    newCells->InsertNextCell(newCell);
    newGrid->SetCells(cell->GetCellType(), newCells);  
    return newGrid;
}

VTKGrid::PtrUGrid VTKGrid::SplitCubeToTetrahedra(PtrUGrid cubeGrid)
{
	auto tetraGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    auto newPoints = vtkSmartPointer<vtkPoints>::New();
    auto cellArray = vtkSmartPointer<vtkCellArray>::New();

    vtkSmartPointer<vtkPoints> cubePoints = cubeGrid->GetPoints();
    for(vtkIdType i = 0; i < cubePoints->GetNumberOfPoints(); ++i) 
    {
        double p[3];
        cubePoints->GetPoint(i, p);
        newPoints->InsertNextPoint(p);
    }

    //  cube index
    //          E----------F
    //         /|         /|
    //        / |        / |
    //       H--|-------G  |
    //       |  A-------|--B
    //       | /        | /
    //       D----------C

    //  6 tetrahedra
    //  {D, C, G, F},
    //  {D, C, B, F},
    //  {D, H, G, F},
    //  {D, B, A, F},
    //  {D, H, E, F},
    //  {D, A, E, F}

   

    int A = 0; int B = 1; int C = 2; int D = 3;
    int E = 4; int F = 5; int G = 6; int H = 7;

    const int tet_size = 6;
    vtkIdType tetraIds[tet_size][4] =
    {
        {D, C, G, F},
        {D, C, B, F},
        {D, H, G, F},
        {D, B, A, F},
        {D, H, E, F},
        {D, A, E, F}

    };

   tetraGrid->SetPoints(newPoints);
   for (int i = 0; i < tet_size; ++i)
    {
        vtkSmartPointer<vtkTetra> tetra = vtkSmartPointer<vtkTetra>::New();
        for (int j = 0; j < 4; ++j)
        {
            tetra->GetPointIds()->SetId(j, tetraIds[i][j]);
        }
        cellArray->InsertNextCell(tetra);
    }

    tetraGrid->SetCells(VTK_TETRA, cellArray);

    return tetraGrid;
}

std::vector<int> VTKGrid::GetTetrahedronFaceIndices(int tetrahedronIndex)
{
	std::vector<int> index;
    if (tetrahedronIndex < 0 || tetrahedronIndex > 5)
    {
        Debug("Index out of range");
        return index;
    }
    int A = 0; int B = 1; int C = 2; int D = 3;
    int E = 4; int F = 5; int G = 6; int H = 7;
    switch (tetrahedronIndex)
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
        Debug("Index out of range");
        return index;
    }
    return index;
}

std::array<int, 4> VTKGrid::toLabelIndex(std::array<int, 4> tetrahedronIndex)
{
	//  cube index
    //          E----------F
    //         /|         /|
    //        / |        / |
    //       H--|-------G  |
    //       |  A-------|--B
    //       | /        | /
    //       D----------C

    // label index
    //          4----------5
    //         /|         /|
    //        / |        / |
    //       6--|-------7  |
    //       |  0-------|--1
    //       | /        | /
    //       2----------3

    // index -> label mapping
    std::unordered_map<int, int> newMapping = 
    {
        {0, 0}, // A -> 0
        {1, 1}, // B -> 1
        {2, 3}, // C -> 3
        {3, 2}, // D -> 2
        {4, 4}, // E -> 4
        {5, 5}, // F -> 5
        {6, 7}, // G -> 7
        {7, 6}  // H -> 6
    };

    std::array<int, 4> mappedIndices = 
    {
        newMapping[tetrahedronIndex[0]],
        newMapping[tetrahedronIndex[1]],
        newMapping[tetrahedronIndex[2]],
        newMapping[tetrahedronIndex[3]]
    };
    return mappedIndices;
}
