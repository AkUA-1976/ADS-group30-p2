#include <iostream>
#include "graph_reader.h"
#include "dijkstra.h"
#include "dijkstra_fib.h"

using namespace std;

// 正确性测试（大图）：
// 在真实图 NY 上验证 dijkstra。
// 道路网是无向的，所以任意两点 A、B 的最短距离应满足 dist(A,B) == dist(B,A)，
// 这里同时算两个方向，看结果是否相等。
int main() {
    int n, m;
    vector<node>* graph = read_graph("data/USA-road-d.NY.gr", n, m);
    if (graph == nullptr) return 1;   // 读失败直接退出

    cout << "n = " << n << endl;
    cout << "m = " << m << endl;

    // 每个点对都算 A->B 和 B->A 两个方向，两个数应该相等
    cout << "1->100000 : " << dijkstra(graph, n, 1, 100000)
         << "   100000->1 : " << dijkstra(graph, n, 100000, 1) << endl;

    cout << "1->200000 : " << dijkstra(graph, n, 1, 200000)
         << "   200000->1 : " << dijkstra(graph, n, 200000, 1) << endl;

    cout << "50000->150000 : " << dijkstra(graph, n, 50000, 150000)
         << "   150000->50000 : " << dijkstra(graph, n, 150000, 50000) << endl;

    cout << "fib 1->100000 : " << dijkstra_fib(graph, n, 1, 100000)
           << "   100000->1 : " << dijkstra_fib(graph, n, 100000, 1) << endl;
     cout << "fib 1->200000 : " << dijkstra_fib(graph, n, 1, 200000)
           << "   200000->1 : " << dijkstra_fib(graph, n, 200000, 1) << endl;
     cout << "fib 50000->150000 : " << dijkstra_fib(graph, n, 50000, 150000)
           << "   150000->50000 : " << dijkstra_fib(graph, n, 150000, 50000) << endl;
    delete[] graph;   // 释放内存
    return 0;
}
