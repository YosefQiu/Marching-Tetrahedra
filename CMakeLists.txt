
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

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
  message(FATAL_ERROR "ReadAllUnstructuredGridTypes: Unable to find the VTK build folder.")
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

TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS ${PROJECT_NAME}
  MODULES ${VTK_LIBRARIES}
)
