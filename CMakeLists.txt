cmake_minimum_required(VERSION 3.14)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ogame-notify)

set(proj_src main.cpp)

message("The sources to compile are these:")
message(${proj_src})


add_executable(${PROJECT_NAME} ${proj_src})
# matplotlibcppで使うPythonとNumpyを探す
find_package(Python3 COMPONENTS Development NumPy)
target_include_directories(${PROJECT_NAME} PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Python3::Python Python3::NumPy)
target_link_libraries(${PROJECT_NAME} PRIVATE dpp)
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp)


set(CMAKE_VERBOSE_MAKEFILE OFF)
