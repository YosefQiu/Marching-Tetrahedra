#include "VTKFileManager.h"

VTKFileManager::PtrUGrid VTKFileManager::ReadUnstructuredGrid(std::string const& fileName)
{
    VTKFileManager::PtrUGrid unstructuredGrid;
    std::string extension = "";
    if (fileName.find_last_of(".") != std::string::npos)
    {
        extension = fileName.substr(fileName.find_last_of("."));
    }

    // Drop the case of the extension
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension == ".vtu")
    {
        vtkNew<vtkXMLUnstructuredGridReader> reader;
        reader->SetFileName(fileName.c_str());
        reader->Update();
        unstructuredGrid = reader->GetOutput();
    }
    else if (extension == ".vtk")
    {
        vtkNew<vtkUnstructuredGridReader> reader;
        reader->SetFileName(fileName.c_str());
        reader->Update();
        unstructuredGrid = reader->GetOutput();
    }
    else
    {
        vtkNew<vtkSphereSource> source;
        source->Update();
        vtkNew<vtkAppendFilter> appendFilter;
        appendFilter->AddInputData(source->GetOutput());
        appendFilter->Update();
        unstructuredGrid = appendFilter->GetOutput();
    }
    Debug("Read unstructured grid from %s", fileName.c_str());
    return unstructuredGrid;
}

VTKFileManager::PtrImg VTKFileManager::ReadImageData(std::string const& fileName, std::string const& attribute_name)
{
    auto  imageData = std::make_shared<ImageDataArray>();

    vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();

    vtkImageData* vtk_imageData = reader->GetOutput();

    int dims[3];
    vtk_imageData->GetDimensions(dims);
    imageData->mWidth = dims[0];
    imageData->mHeight = dims[1];
    imageData->mDepth = dims[2];


    vtkPointData* pointData = vtk_imageData->GetPointData();
    if (!pointData) 
    {
        Debug("VTKFileManager:: No point data found in file.");
        return nullptr;
    }

    vtkDataArray* maxlabelArray = pointData->GetArray(attribute_name.c_str());
    if (!maxlabelArray) 
    {
        Debug("VTKFileManager:: [ %s ] attribute not found in point data.", attribute_name.c_str());
        return nullptr;
    }

    imageData->mData = maxlabelArray;

    if (imageData->mData->GetNumberOfTuples() != imageData->mWidth * imageData->mHeight * imageData->mDepth) 
    {
        Debug("Incorrect number of labels read from file: %s", fileName.c_str());
        return nullptr;
    }
    else
    {
        Debug("Correct number of labels [ %d ] read from file: %s", imageData->mData->GetNumberOfTuples(), fileName.c_str());
    }
  
    return imageData;
}

void VTKFileManager::WriteUnstructuredGrid(std::string const& fileName, PtrUGrid const& unstructuredGrid)
{
    VTKFileManager::PtrUGWriter writer = VTKFileManager::PtrUGWriter::New();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData(unstructuredGrid);
    writer->Write();
    Debug("Write unstructured grid to %s", fileName.c_str());
}
