import vtk
import argparse

def read_vtu(file_path):
    """读取VTU文件"""
    reader = vtk.vtkXMLUnstructuredGridReader()
    reader.SetFileName(file_path)
    reader.Update()
    return reader.GetOutput()

def unstructured_grid_to_poly_data(unstructured_grid):
    """将vtkUnstructuredGrid转换为vtkPolyData"""
    geometry_filter = vtk.vtkGeometryFilter()
    geometry_filter.SetInputData(unstructured_grid)
    geometry_filter.Update()
    return geometry_filter.GetOutput()

def clean_poly_data(poly_data, tolerance=1e-6):
    """清理vtkPolyData，合并接近的顶点"""
    clean_filter = vtk.vtkCleanPolyData()
    clean_filter.SetInputData(poly_data)
    clean_filter.SetTolerance(tolerance)
    clean_filter.Update()
    return clean_filter.GetOutput()

def write_vtp(poly_data, file_path):
    """将vtkPolyData写入VTP文件"""
    writer = vtk.vtkXMLPolyDataWriter()
    writer.SetFileName(file_path)
    writer.SetInputData(poly_data)
    writer.Write()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process VTU files.')
    parser.add_argument('input_file', type=str, help='Input VTU file path')
    parser.add_argument('output_file', type=str, help='Output VTP file path')

    args = parser.parse_args()

    # 读取VTU文件
    unstructured_grid = read_vtu(args.input_file)

    # 将vtkUnstructuredGrid转换为vtkPolyData
    poly_data = unstructured_grid_to_poly_data(unstructured_grid)

    # 清理PolyData
    cleaned_poly_data = clean_poly_data(poly_data)

    # 保存处理后的PolyData
    write_vtp(cleaned_poly_data, args.output_file)

    print("处理完成，结果已保存至：", args.output_file)
