#include "ggl.h"
#include "VTKFileManager.h"
#include "VTKGrid.h"
#include "MarchingTet.h"
#include "LookupTable.h"
#include "Log.hpp"

#include <mpi.h>

std::string getFileName(const std::string& filename) 
{
    std::size_t lastDotPosition = filename.find_last_of(".");
    if (lastDotPosition != std::string::npos) 
    {
        return filename.substr(0, lastDotPosition);
    }
    // Return original filename if no extension found
    return filename;
}

std::string replaceExtension(const std::string& filename, const std::string& newExtension) 
{
    std::size_t lastDotPosition = filename.find_last_of(".");
    if (lastDotPosition != std::string::npos) 
    {
        return filename.substr(0, lastDotPosition) + newExtension;
    }
    // Return original filename if no extension found
    return filename;
}

void printHelp() 
{
    std::cout << "Usage: ./app [options]\n"
              << "Options:\n"
              << "  -h           Print this help message\n"
              << "  -input       Specify input filename\n"
              << "  -attribute   Specify attribute name\n"
              << "  -output      (Option)Specify output filename\n";
}

void run(const char* filename, const char* attribute_name, const char* output_name = nullptr)
{
    auto path = getFileName(filename);
    auto finalOutputName = path + "_" + attribute_name + ".vtu";
    auto sphereOutputName = path + "_" + attribute_name + "_critical_points.vtp";
    auto polyGridOutputName = path + "_" + attribute_name + "_smooth.vtp";

    // Check if output_name is nullptr, and if so, replace the filename extension with .vtu
    if (output_name == nullptr) {
        finalOutputName = replaceExtension(filename, ".vtu");
    } 
    else
    {
        finalOutputName = std::string(output_name); // Use output_name directly if it's not nullptr
    }

    if (attribute_name == nullptr) 
    {
        attribute_name = "scalars";
    }

    auto cube_label = VTKFileManager::ReadImageData(filename, attribute_name);
    auto critical_points = VTKFileManager::ExportCriticalPoints(cube_label);
    if (std::find(critical_points.begin(), critical_points.end(), -1) != critical_points.end())
    {
        critical_points.erase(std::remove(critical_points.begin(), critical_points.end(), -1), critical_points.end());
    }
    auto cube_unstructuredGrid = VTKGrid::CreateCubeMesh(cube_label->mWidth, cube_label->mHeight, cube_label->mDepth);

    Debug("cube size %d", cube_unstructuredGrid->GetNumberOfCells());

    Debug("critical points size %d", critical_points.size());

    auto appendFilter = MarchingTetrahedra::AddCriticalPoint(critical_points, cube_unstructuredGrid, 1.5f);

    VTKFileManager::WritePolyData(sphereOutputName, appendFilter->GetOutput());
    

    std::vector<vtkSmartPointer<vtkUnstructuredGrid>> triangle_vec;
    for (auto i = 0; i < cube_unstructuredGrid->GetNumberOfCells(); i++)
    {
        // get current cell label
        auto current_cube_label = cube_label->GetCellLabel(i);
        bool is_same_label = VTKFileManager::ImageDataArray::IsSame(current_cube_label);
        if (is_same_label)
        {
            continue;
        }

        if (std::find(current_cube_label.begin(), current_cube_label.end(), -1) != current_cube_label.end())
        {
            continue;
        }


        
        // get current cell grid
        auto current_cube_grid = VTKGrid::ExtractSingleCell(cube_unstructuredGrid, i);
        // split cube to 6 tetrahedron
        auto current_tets_grid = VTKGrid::SplitCubeToTetrahedra(current_cube_grid);

        for (auto j = 0; j < 6; ++j)
        {
            // get current tet grid
            auto current_tet_grid = VTKGrid::ExtractSingleCell(current_tets_grid, j); 
            auto current_tet_index = VTKGrid::GetTetrahedronFaceIndices(j);

            // get current tet label
            std::array<int, 4> tet_index = {current_tet_index[0], current_tet_index[1], current_tet_index[2], current_tet_index[3]};
            auto current_label_index = VTKGrid::toLabelIndex(tet_index); 
            std::array<int, 4> current_tet_label;
            for (auto k = 0; k < 4; ++k)
            {
                current_tet_label[k] = current_cube_label[current_label_index[k]];
            }

            // find case number
            auto which_case = LookupTable::EvaluateCase(current_tet_label[0], current_tet_label[1], current_tet_label[2], current_tet_label[3]);
            
            // get tetrahedron vertices
            auto current_tet_points = current_tet_grid->GetPoints();

            if (which_case.caseNumber == 3 || which_case.caseNumber == 8 || which_case.caseNumber == 16 || which_case.caseNumber == 21) 
            {
                
                auto grid = MarchingTetrahedra::DoCase1(current_tet_points, {which_case.AIndex, which_case.BIndex, which_case.CIndex, which_case.DIndex});
                triangle_vec.push_back(grid); 
            }
            else if (which_case.caseNumber == 10 || which_case.caseNumber == 17 || which_case.caseNumber == 20)
            {
                auto grid = MarchingTetrahedra::DoCase2(current_tet_points, {which_case.AIndex, which_case.BIndex, which_case.CIndex, which_case.DIndex});
                triangle_vec.push_back(grid);  
            }
            else if (which_case.caseNumber == 11 || which_case.caseNumber == 19 || which_case.caseNumber == 23 || which_case.caseNumber == 24
                    || which_case.caseNumber == 25 || which_case.caseNumber == 26)
            {
                auto grid = MarchingTetrahedra::DoCase3(current_tet_points, {which_case.AIndex, which_case.BIndex, which_case.CIndex, which_case.DIndex});
                triangle_vec.push_back(grid);  
            }
            else if (which_case.caseNumber == 27)
            {
                auto grid = MarchingTetrahedra::DoCase4(current_tet_points, {which_case.AIndex, which_case.BIndex, which_case.CIndex, which_case.DIndex});
                triangle_vec.push_back(grid);
            }
            else if (which_case.caseNumber == 0)
            {
                continue;
            }
        }

    }

    vtkSmartPointer<vtkAppendFilter> append = vtkSmartPointer<vtkAppendFilter>::New();
    Debug("triangle number %d", triangle_vec.size());
    for (auto i = 0; i < triangle_vec.size(); i++)
    {
        append->AddInputData(triangle_vec[i]);
    }
    append->Update();
    VTKFileManager::WriteUnstructuredGrid(finalOutputName, append->GetOutput());

    auto polyGrid = VTKFileManager::UnstructuredGridToPolyData(append->GetOutput());
    auto smooth_grid = VTKFileManager::CleanData(polyGrid, 1e-6);
    VTKFileManager::WritePolyData(polyGridOutputName, smooth_grid);

    Debug("Finished....");
}

int main(int argc, char* argv[])
{
    // std::string inputFilename ;
    // std::string attributeName;
    // std::string outputFilename;

    // for (int i = 1; i < argc; ++i) 
    // { 
    //     std::string arg = argv[i];
    //     if (arg == "-h") 
    //     {
    //         printHelp();
    //         return 0;
    //     } else if (arg == "-input" && i + 1 < argc) 
    //     { 
    //         inputFilename = argv[++i]; 
    //     } 
    //     else if (arg == "-attribute" && i + 1 < argc) 
    //     {
    //         attributeName = argv[++i];
    //     } 
    //     else if (arg == "-output" && i + 1 < argc) 
    //     {
    //         outputFilename = argv[++i];
    //     } 
    //     else 
    //     {
    //         std::cerr << "Unknown option or missing argument: " << arg << std::endl;
    //         return 1;
    //     }
    // }

    // run(inputFilename.c_str(), "maxlabel", nullptr);
    // run(inputFilename.c_str(), "minlabel", nullptr);
    
    // return 0;
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::string inputFilename ;
    std::string attributeName;
    std::string outputFilename;

    for (int i = 1; i < argc; ++i) 
    { 
        std::string arg = argv[i];
        if (arg == "-h") 
        {
            printHelp();
            MPI_Finalize();
            return 0;
        } else if (arg == "-input" && i + 1 < argc) 
        { 
            inputFilename = argv[++i]; 
        } 
        else if (arg == "-attribute" && i + 1 < argc) 
        {
            attributeName = argv[++i];
        } 
        else if (arg == "-output" && i + 1 < argc) 
        {
            outputFilename = argv[++i];
        } 
        else 
        {
            std::cerr << "Unknown option or missing argument: " << arg << std::endl;
            MPI_Finalize();
            return 1;
        }
    }

    if (world_rank == 0) {
        run(inputFilename.c_str(), "maxlabel", nullptr);
    } else if (world_rank == 1) {
        run(inputFilename.c_str(), "minlabel", nullptr);
    }

    MPI_Finalize();
    return 0;
}
