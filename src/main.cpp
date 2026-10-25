#include <iostream>
#include "graph_reader.h"

using namespace std;

// 数据读入的验证程序：
// 读入 NY 图，并统计邻接表里实际的边数，看是否和 m 一致。
int main() {
    int n, m;   // 顶点数、边数
    vector<node>* graph = read_graph("data/USA-road-d.NY.gr", n, m);
    if(graph == nullptr) return 1;   // 读失败直接退出

    cout << "number of vertices n = " << n << endl;
    cout << "number of edges m = " << m << endl;

    // 累加每个顶点的邻接表长度，得到实际读进来的总边数
    long long total = 0;
    for(int i = 1; i <= n; ++i) {
        total += graph[i].size();
    }
    cout << "total arcs = " << total << endl;   // 应等于 m

    delete[] graph;   // 释放内存
    return 0;
}
