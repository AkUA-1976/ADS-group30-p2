#pragma once
#include <cstdint>
#include <vector>
#include "dijkstra.h" // 复用 struct node

// 基于斐波那契堆的 Dijkstra：
// 每个顶点只在第一次被松弛时 insert，之后用 decreaseKey 改距离，
// nodes[v] 保存顶点 v 对应的堆结点，避免堆里出现重复项。
std::int64_t dijkstra_fib(
    std::vector<node>* graph,
    int n,
    int st,
    int ed
);