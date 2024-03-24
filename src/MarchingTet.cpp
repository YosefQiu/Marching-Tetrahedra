#include "MarchingTet.h"


vtkIdType MarchingTetrahedra::AddMidpoint(vtkSmartPointer<vtkPoints> points, vtkIdType idx1, vtkIdType idx2)
{
	double p1[3], p2[3], mid[3];
    points->GetPoint(idx1, p1); 
    points->GetPoint(idx2, p2); 

    for (int i = 0; i < 3; ++i) 
    {
        mid[i] = (p1[i] + p2[i]) / 2.0;
    }


    return points->InsertNextPoint(mid);
}

vtkIdType MarchingTetrahedra::AddCentroid(vtkSmartPointer<vtkPoints> points, vtkIdType idx1, vtkIdType idx2, vtkIdType idx3) 
{
    double p1[3], p2[3], p3[3], centroid[3];
    points->GetPoint(idx1, p1); 
    points->GetPoint(idx2, p2); 
    points->GetPoint(idx3, p3); 

    for (int i = 0; i < 3; ++i) 
    {
        centroid[i] = (p1[i] + p2[i] + p3[i]) / 3.0;
    }

    return points->InsertNextPoint(centroid);
}

vtkIdType MarchingTetrahedra::AddTetrahedronCentroid(vtkSmartPointer<vtkPoints> points, vtkIdType idx1, vtkIdType idx2, vtkIdType idx3, vtkIdType idx4) 
{
    double p1[3], p2[3], p3[3], p4[3], centroid[3];
    points->GetPoint(idx1, p1); 
    points->GetPoint(idx2, p2); 
    points->GetPoint(idx3, p3); 
    points->GetPoint(idx4, p4); 


    for (int i = 0; i < 3; ++i) 
    {
        centroid[i] = (p1[i] + p2[i] + p3[i] + p4[i]) / 4.0;
    }

    return points->InsertNextPoint(centroid);
}

MarchingTetrahedra::PtrUGrid MarchingTetrahedra::DoCase1(vtkSmartPointer<vtkPoints> points, TetConfig config, PtrUGrid case1_tet)
{
	int A = config.A, B = config.B, C = config.C, D = config.D;
    vtkIdType midIdx1 = MarchingTetrahedra::AddMidpoint(points, A, B);
    vtkIdType midIdx2 = MarchingTetrahedra::AddMidpoint(points, A, C);
    vtkIdType midIdx3 = MarchingTetrahedra::AddMidpoint(points, A, D);
    vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
    triangle->GetPointIds()->SetId(0, midIdx1); 
    triangle->GetPointIds()->SetId(1, midIdx2); 
    triangle->GetPointIds()->SetId(2, midIdx3); 
    if (case1_tet) 
    {
        case1_tet->InsertNextCell(triangle->GetCellType(), triangle->GetPointIds());
        return case1_tet;
    } 
    else 
    {
        // Create a new unstructured grid to store the triangle if case1_tet is null
        vtkSmartPointer<vtkUnstructuredGrid> newGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

        // Add the triangle to the cell array
        cellArray->InsertNextCell(triangle);

        // Set the points and cells to the new grid
        newGrid->SetPoints(points);
        newGrid->SetCells(triangle->GetCellType(), cellArray);

        // Return the new grid with the triangle
        return newGrid;
    }
}

MarchingTetrahedra::PtrUGrid MarchingTetrahedra::DoCase2(vtkSmartPointer<vtkPoints> points, TetConfig config, PtrUGrid case2_tet)
{
	int A = config.A, B = config.B, C = config.C, D = config.D;
    vtkIdType midAD = MarchingTetrahedra::AddMidpoint(points, A, D);
    vtkIdType midAC = MarchingTetrahedra::AddMidpoint(points, A, C);
    vtkIdType midDB = MarchingTetrahedra::AddMidpoint(points, D, B);
    vtkIdType midCB = MarchingTetrahedra::AddMidpoint(points, C, B);

    vtkSmartPointer<vtkTriangle> case2_triangle1 = vtkSmartPointer<vtkTriangle>::New();
    case2_triangle1->GetPointIds()->SetId(0, midAD);
    case2_triangle1->GetPointIds()->SetId(1, midAC);
    case2_triangle1->GetPointIds()->SetId(2, midCB);

    vtkSmartPointer<vtkTriangle> case2_triangle2 = vtkSmartPointer<vtkTriangle>::New();
    case2_triangle2->GetPointIds()->SetId(0, midAD);
    case2_triangle2->GetPointIds()->SetId(1, midDB);
    case2_triangle2->GetPointIds()->SetId(2, midCB);

    if (case2_tet) 
    {
        case2_tet->InsertNextCell(case2_triangle1->GetCellType(), case2_triangle1->GetPointIds());
        case2_tet->InsertNextCell(case2_triangle2->GetCellType(), case2_triangle2->GetPointIds());
        return case2_tet;
    } 
    else 
    {
        // Create a new unstructured grid to store the triangle if case1_tet is null
        vtkSmartPointer<vtkUnstructuredGrid> newGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

        // Add the triangle to the cell array
        cellArray->InsertNextCell(case2_triangle1);
        cellArray->InsertNextCell(case2_triangle2);

        // Set the points and cells to the new grid
        newGrid->SetPoints(points);
        newGrid->SetCells(case2_triangle1->GetCellType(), cellArray);

        // Return the new grid with the triangle
        return newGrid;
    }
        
}

MarchingTetrahedra::PtrUGrid MarchingTetrahedra::DoCase3(vtkSmartPointer<vtkPoints> points, TetConfig config, PtrUGrid case3_tet)
{
	int A = config.A, B = config.B, C = config.C, D = config.D;
    vtkIdType midAC = MarchingTetrahedra::AddMidpoint(points, A, C); // A-C edge
    vtkIdType midAD = MarchingTetrahedra::AddMidpoint(points, A, D); // A-D edge
    vtkIdType midBC = MarchingTetrahedra::AddMidpoint(points, B, C); // B-C edge
    vtkIdType midBD = MarchingTetrahedra::AddMidpoint(points, B, D); // B-D edge
    vtkIdType midCD = MarchingTetrahedra::AddMidpoint(points, C, D); // C-D edge
    vtkIdType midOfMidACD = MarchingTetrahedra::AddCentroid(points, A, C, D); 
    vtkIdType midOfMidBCD = MarchingTetrahedra::AddCentroid(points, B, C, D);

    // Create triangles that make up the octahedron
    vtkSmartPointer<vtkTriangle> case3_triangle1 = vtkSmartPointer<vtkTriangle>::New();
    case3_triangle1->GetPointIds()->SetId(0, midAD);
    case3_triangle1->GetPointIds()->SetId(1, midBD);
    case3_triangle1->GetPointIds()->SetId(2, midOfMidBCD);

    vtkSmartPointer<vtkTriangle> case3_triangle2 = vtkSmartPointer<vtkTriangle>::New();
    case3_triangle2->GetPointIds()->SetId(0, midOfMidACD);
    case3_triangle2->GetPointIds()->SetId(1, midAD);
    case3_triangle2->GetPointIds()->SetId(2, midOfMidBCD);

    vtkSmartPointer<vtkTriangle> case3_triangle3 = vtkSmartPointer<vtkTriangle>::New();
    case3_triangle3->GetPointIds()->SetId(0, midCD);
    case3_triangle3->GetPointIds()->SetId(1, midOfMidACD);
    case3_triangle3->GetPointIds()->SetId(2, midOfMidBCD);

    vtkSmartPointer<vtkTriangle> case3_triangle4 = vtkSmartPointer<vtkTriangle>::New();
    case3_triangle4->GetPointIds()->SetId(0, midAC);
    case3_triangle4->GetPointIds()->SetId(1, midOfMidACD);
    case3_triangle4->GetPointIds()->SetId(2, midOfMidBCD);
        
    vtkSmartPointer<vtkTriangle> case3_triangle5 = vtkSmartPointer<vtkTriangle>::New();
    case3_triangle5->GetPointIds()->SetId(0, midAC);
    case3_triangle5->GetPointIds()->SetId(1, midOfMidBCD);
    case3_triangle5->GetPointIds()->SetId(2, midBC);

    if (case3_tet)
    {
        case3_tet->InsertNextCell(case3_triangle1->GetCellType(), case3_triangle1->GetPointIds());
        case3_tet->InsertNextCell(case3_triangle2->GetCellType(), case3_triangle2->GetPointIds());
        case3_tet->InsertNextCell(case3_triangle3->GetCellType(), case3_triangle3->GetPointIds());
        case3_tet->InsertNextCell(case3_triangle4->GetCellType(), case3_triangle4->GetPointIds());
        case3_tet->InsertNextCell(case3_triangle5->GetCellType(), case3_triangle5->GetPointIds());
        return case3_tet;
    }
    else
    {
        // Create a new unstructured grid to store the triangle if case1_tet is null
        vtkSmartPointer<vtkUnstructuredGrid> newGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

        // Add the triangle to the cell array
        cellArray->InsertNextCell(case3_triangle1);
        cellArray->InsertNextCell(case3_triangle2);
        cellArray->InsertNextCell(case3_triangle3);
        cellArray->InsertNextCell(case3_triangle4);
        cellArray->InsertNextCell(case3_triangle5);

        // Set the points and cells to the new grid
        newGrid->SetPoints(points);
        newGrid->SetCells(case3_triangle1->GetCellType(), cellArray);

        // Return the new grid with the triangle
        return newGrid;
    }
    
}

MarchingTetrahedra::PtrUGrid MarchingTetrahedra::DoCase4(vtkSmartPointer<vtkPoints> points, TetConfig config, PtrUGrid case4_tet)
{
    int A = config.A, B = config.B, C = config.C, D = config.D;
    vtkIdType midAB = MarchingTetrahedra::AddMidpoint(points, A, B); // A-B edge
    vtkIdType midAC = MarchingTetrahedra::AddMidpoint(points, A, C); // A-C edge
    vtkIdType midAD = MarchingTetrahedra::AddMidpoint(points, A, D); // A-D edge
    vtkIdType midBC = MarchingTetrahedra::AddMidpoint(points, B, C); // B-C edge
    vtkIdType midBD = MarchingTetrahedra::AddMidpoint(points, B, D); // B-D edge
    vtkIdType midCD = MarchingTetrahedra::AddMidpoint(points, C, D); // C-D edge

    vtkIdType midOfMidABCD = MarchingTetrahedra::AddTetrahedronCentroid(points, A, B, C, D);

    vtkIdType midOfMidACD = MarchingTetrahedra::AddCentroid(points, A, C, D);
    vtkIdType midOfMidBCD = MarchingTetrahedra::AddCentroid(points, B, C, D);
    vtkIdType midOfMidABD = MarchingTetrahedra::AddCentroid(points, A, B, D);
    vtkIdType midOfMidABC = MarchingTetrahedra::AddCentroid(points, A, B, C);

    vtkSmartPointer<vtkTriangle> case4_triangle1 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle1->GetPointIds()->SetId(0, midOfMidACD);
    case4_triangle1->GetPointIds()->SetId(1, midAD);
    case4_triangle1->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle2 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle2->GetPointIds()->SetId(0, midAD);
    case4_triangle2->GetPointIds()->SetId(1, midOfMidABD);
    case4_triangle2->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle3 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle3->GetPointIds()->SetId(0, midOfMidABD);
    case4_triangle3->GetPointIds()->SetId(1, midBD);
    case4_triangle3->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle4 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle4->GetPointIds()->SetId(0, midBD);
    case4_triangle4->GetPointIds()->SetId(1, midOfMidBCD);
    case4_triangle4->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle5 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle5->GetPointIds()->SetId(0, midOfMidBCD);
    case4_triangle5->GetPointIds()->SetId(1, midBC);
    case4_triangle5->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle6 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle6->GetPointIds()->SetId(0, midBC);
    case4_triangle6->GetPointIds()->SetId(1, midOfMidABC);
    case4_triangle6->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle7 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle7->GetPointIds()->SetId(0, midOfMidABC);
    case4_triangle7->GetPointIds()->SetId(1, midAC);
    case4_triangle7->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle8 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle8->GetPointIds()->SetId(0, midAC);
    case4_triangle8->GetPointIds()->SetId(1, midOfMidACD);
    case4_triangle8->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle9 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle9->GetPointIds()->SetId(0, midCD);
    case4_triangle9->GetPointIds()->SetId(1, midOfMidACD);
    case4_triangle9->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle10 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle10->GetPointIds()->SetId(0, midOfMidBCD);
    case4_triangle10->GetPointIds()->SetId(1, midCD);
    case4_triangle10->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle11 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle11->GetPointIds()->SetId(0, midOfMidABD);
    case4_triangle11->GetPointIds()->SetId(1, midAB);
    case4_triangle11->GetPointIds()->SetId(2, midOfMidABCD);

    vtkSmartPointer<vtkTriangle> case4_triangle12 = vtkSmartPointer<vtkTriangle>::New();
    case4_triangle12->GetPointIds()->SetId(0, midAB);
    case4_triangle12->GetPointIds()->SetId(1, midOfMidABC);
    case4_triangle12->GetPointIds()->SetId(2, midOfMidABCD);
    
    if (case4_tet)
    {
        case4_tet->InsertNextCell(case4_triangle1->GetCellType(), case4_triangle1->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle2->GetCellType(), case4_triangle2->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle3->GetCellType(), case4_triangle3->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle4->GetCellType(), case4_triangle4->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle5->GetCellType(), case4_triangle5->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle6->GetCellType(), case4_triangle6->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle7->GetCellType(), case4_triangle7->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle8->GetCellType(), case4_triangle8->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle9->GetCellType(), case4_triangle9->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle10->GetCellType(), case4_triangle10->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle11->GetCellType(), case4_triangle11->GetPointIds());
        case4_tet->InsertNextCell(case4_triangle12->GetCellType(), case4_triangle12->GetPointIds());
        return case4_tet;
    }
    else
    {
        // Create a new unstructured grid to store the triangle if case1_tet is null
        vtkSmartPointer<vtkUnstructuredGrid> newGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

        // Add the triangle to the cell array
        cellArray->InsertNextCell(case4_triangle1);
        cellArray->InsertNextCell(case4_triangle2);
        cellArray->InsertNextCell(case4_triangle3);
        cellArray->InsertNextCell(case4_triangle4);
        cellArray->InsertNextCell(case4_triangle5);
        cellArray->InsertNextCell(case4_triangle6);
        cellArray->InsertNextCell(case4_triangle7);
        cellArray->InsertNextCell(case4_triangle8);
        cellArray->InsertNextCell(case4_triangle9);
        cellArray->InsertNextCell(case4_triangle10);
        cellArray->InsertNextCell(case4_triangle11);
        cellArray->InsertNextCell(case4_triangle12);

        // Set the points and cells to the new grid
        newGrid->SetPoints(points);
        newGrid->SetCells(case4_triangle1->GetCellType(), cellArray);

        // Return the new grid with the triangle
        return newGrid;
    }
    
}

MarchingTetrahedra::PtrGrid MarchingTetrahedra::AddCriticalPoint(std::vector<int>& critical_points, PtrUGrid const& cube_unstructuredGrid, float radius)
{
	auto appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();

    for (vtkIdType index : critical_points) 
    {
        double pos[3];
        // 假设GetPointPositionById是一个函数，根据索引返回网格上点的位置
        cube_unstructuredGrid->GetPoint(index, pos); // VTK网格可以直接使用GetPoint获取位置

        vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->SetCenter(pos);
        sphereSource->SetRadius(radius); // 设置小球的半径，根据需要调整
        sphereSource->Update();

        appendFilter->AddInputData(sphereSource->GetOutput());
    }

    appendFilter->Update();
    return appendFilter;
}
