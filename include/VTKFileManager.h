#pragma once
#include "ggl.h"
#include "Log.hpp"

class VTKFileManager
{
public:
    using PtrUGrid      = vtkSmartPointer<vtkUnstructuredGrid>;
    using PtrGrid       = vtkSmartPointer<vtkPolyData>;
    using PtrUGWriter   = vtkSmartPointer<vtkXMLUnstructuredGridWriter>;
    struct ImageDataArray
    {
        int mWidth;
        int mHeight;
        int mDepth;
        vtkSmartPointer<vtkDataArray> mData;
        using Label = int;
        ImageDataArray()
        {
            mWidth = 0;
            mHeight = 0;
            mDepth = 0;
            mData = nullptr;
        }
        std::array<int, 8> GetCellLabel(int cellID)
        {
            if (cellID < 0 || cellID >= (mWidth - 1) * (mHeight - 1) * (mDepth - 1))
            {
                Debug("Invalid cell ID");
                return {};
            }

            if (mData == nullptr)
            {
                Debug("Data is null");
                return {};
            }

            int W = mWidth; int H = mHeight; int D = mDepth;
            int x = cellID % (W - 1);
            int y = (cellID / (W - 1)) % (H - 1);
            int z = cellID / ((W - 1) * (H - 1));

            int vertexIndices[8] = 
            {
                z * W * H + y * W + x,                  
                z * W * H + y * W + (x + 1),            
                z * W * H + (y + 1) * W + x,            
                z * W * H + (y + 1) * W + (x + 1),      
                (z + 1) * W * H + y * W + x,            
                (z + 1) * W * H + y * W + (x + 1),      
                (z + 1) * W * H + (y + 1) * W + x,      
                (z + 1) * W * H + (y + 1) * W + (x + 1) 
            };

            std::array<int, 8> labelValue;
            for (int i = 0; i < 8; ++i) 
            {
                int value = mData->GetTuple1(vertexIndices[i]);
                labelValue[i] = value;
            }
            return labelValue;
        }
        static bool IsSame(const std::array<Label, 8>& labels)
        {
            return std::all_of(labels.begin(), labels.end(), [&](Label label) { return label == labels[0]; });
        }
    };
    using PtrImg        = std::shared_ptr<ImageDataArray>;
    VTKFileManager() = default;
    virtual ~VTKFileManager() = default;
public:
    static PtrUGrid ReadUnstructuredGrid(std::string const& fileName);
    static PtrImg ReadImageData(std::string const& fileName, std::string const& attribute_name);
    static void WriteUnstructuredGrid(std::string const& fileName, PtrUGrid const& unstructuredGrid);
    static void WritePolyData(std::string const& fileName, vtkSmartPointer<vtkPolyData> polyData);
    static std::vector<int> ExportCriticalPoints(PtrImg img);
    static PtrGrid UnstructuredGridToPolyData(PtrUGrid const& unstructuredGrid);    
    static PtrGrid CleanData(PtrGrid const& polyData, double tolerance = 1e-6);
};
