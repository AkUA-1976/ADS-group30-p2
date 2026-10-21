#include <iostream>
#include "graph_reader.h"
#include "dijkstra.h"

using namespace std;

// 正确性测试（小图）：
// 用一个手工能算出答案的小图（test/test.gr），
// 同时验证 read_graph 读得对不对、dijkstra 算得对不对。
int main() {
    int n, m;
    vector<node>* graph = read_graph("test/test.gr", n, m);
    if (graph == nullptr) return 1;   // 读失败直接退出

    cout << "n = " << n << endl;
    cout << "m = " << m << endl;

    // 打印从 1 号点到各点的最短距离
    // 期望答案：dist(1,2)=1，dist(1,3)=3，dist(1,4)=4，dist(1,5)=7
    cout << "dist(1,2) = " << dijkstra(graph, n, 1, 2) << endl;
    cout << "dist(1,3) = " << dijkstra(graph, n, 1, 3) << endl;
    cout << "dist(1,4) = " << dijkstra(graph, n, 1, 4) << endl;
    cout << "dist(1,5) = " << dijkstra(graph, n, 1, 5) << endl;

    delete[] graph;   // 释放内存
    return 0;
}
