
CMAKE_MINIMUM_REQUIRED(VERSION 3.12 FATAL_ERROR)
INCLUDE(ExternalProject) 

IF(MSVC)
    SET(CMAKE_GENERATOR_PLATFORM win64)
ENDIF()

SET(CMAKE_CXX_STANDARD 17)

PROJECT(app)

FIND_PACKAGE(VTK COMPONENTS
  CommonColor
  CommonCore
  CommonDataModel
  FiltersCore
  FiltersSources
  FiltersGeometry
  IOLegacy
  IOXML
  InteractionStyle
  RenderingAnnotation
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

IF (NOT VTK_FOUND)
  MESSAGE(STATUS "VTK not found. Downloading and building VTK now.")

  ExternalProject_Add(VTK
    GIT_REPOSITORY https://gitlab.kitware.com/vtk/vtk.git
    GIT_TAG v9.2.0 
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/VTK
      -DVTK_GROUP_ENABLE_Imaging=NO
      -DVTK_GROUP_ENABLE_MPI=NO
      -DVTK_GROUP_ENABLE_Qt=NO
      -DVTK_GROUP_ENABLE_Rendering=YES
      -DVTK_GROUP_ENABLE_StandAlone=YES
      -DVTK_GROUP_ENABLE_Views=NO
      -DVTK_GROUP_ENABLE_Web=NO
      -DBUILD_SHARED_LIBS=ON
      -DCMAKE_BUILD_TYPE=Release
  )

  SET(VTK_DIR ${CMAKE_BINARY_DIR}/VTK/lib/cmake/vtk-9.2) # 更新为VTK 9.2版本路径
ENDIF()

# Prevent a "command line is too long" failure in Windows.
SET(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/External)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_DEFINITIONS(-D_CMAKE)
ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNINGS)

FILE(GLOB PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
FILE(GLOB PROJECT_SOURCES src/*.cpp)

SOURCE_GROUP("Headers" FILES ${PROJECT_HEADERS})
SOURCE_GROUP("Sources" FILES ${PROJECT_SOURCES})


ADD_EXECUTABLE (
  ${PROJECT_NAME} 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES} 
  )
IF (NOT VTK_FOUND)
add_dependencies(${PROJECT_NAME} VTK)
ENDIF()
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS ${PROJECT_NAME}
  MODULES ${VTK_LIBRARIES}
)
