#pragma once

#include <string>
#include <vector>

#include "dijkstra.h"

// 数据读入模块
// 作用：把 DIMACS 格式的文本图文件（.gr）读进来，转成 dijkstra 需要的稀疏表（邻接表）。

// 读取图文件
// 参数：
//   filename —— 图文件的路径
//   n        —— 输出参数：顶点个数（从 p 行读到）
//   m        —— 输出参数：边的条数（从 p 行读到）
// 返回值：指向 vector<node> 数组的指针，下标 1..n 有效；读失败返回 nullptr
std::vector<node>* read_graph(const std::string& filename, int& n, int& m);
