
#pragma once

#include <cstdint>
#include <vector>

// 表示一条有向边
struct node {
    std::int64_t length; // 边权
    int vertex;          // 终点编号
};

// 点对点最短路，使用二项队列（懒插入）：
// 每次松弛成功都插入一个新元素，旧元素留在堆里；
// 出堆时用 valid 数组判断是否已经确定，过期项直接跳过。
// graph 必须指向至少 n+1 个 vector<node>
// 顶点编号范围为 [1,n]
// 返回 -1 表示不可达
std::int64_t dijkstra(
    std::vector<node>* graph,
    int n,
    int st,
    int ed
);