#pragma once
#include <cstdint>
#include <vector>
#include "dijkstra.h" // 复用 struct node

// 基于斐波那契堆的 Dijkstra
std::int64_t dijkstra_fib(
    std::vector<node>* graph,
    int n,
    int st,
    int ed
);