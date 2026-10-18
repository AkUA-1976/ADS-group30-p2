# 设定 CMake 最低版本要求
cmake_minimum_required(VERSION 3.15)

# 定义工程名称与支持语言
project(OOP_Project_01 CXX)

# 设置 C++ 标准为 C++17，且强制生效
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件查找路径（相当于 g++ 的 -I 参数）
include_directories(include)

# 收集所有需要参与编译的源文件
set(SOURCES
    src/binominal_queue.cpp
    src/dijkstra.cpp
    include/binominal_queue.h
    include/dijkstra.h
)

# 生成目标可执行文件
add_executable(app ${SOURCES})
