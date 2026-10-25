# 设定 CMake 最低版本要求
cmake_minimum_required(VERSION 3.15)

# 定义工程名称与支持语言
project(OOP_Project_01 CXX)

# 设置 C++ 标准为 C++17，且强制生效
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件查找路径（相当于 g++ 的 -I 参数）
include_directories(include)

# MSVC 下源码含中文注释，指定 UTF-8 消除 C4819 警告
if(MSVC)
    add_compile_options(/utf-8)
endif()

# 核心库：图读取 + 二项堆 Dijkstra + 斐波那契堆 Dijkstra
# 这些是实现代码，不含 main()，统一编译成静态库供各可执行文件复用
add_library(core STATIC
    src/graph_reader.cpp
    src/binominal_queue.cpp
    src/dijkstra.cpp
    src/fibonacci_heap.cpp
    src/dijkstra_fib.cpp
)

# 每个含 main() 的源文件各自生成一个可执行目标
add_executable(app        src/main.cpp)       # 数据读入验证程序
add_executable(test       src/test.cpp)       # 小图正确性测试
add_executable(benchmark  src/benchmark.cpp)  # 大图正确性测试
add_executable(perf       src/perf.cpp)       # 性能测试

# 让各可执行目标链接核心库
target_link_libraries(app       core)
target_link_libraries(test      core)
target_link_libraries(benchmark core)
target_link_libraries(perf      core)
