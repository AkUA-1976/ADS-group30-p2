#include <iostream>
#include <chrono>
#include <string>
#include "graph_reader.h"
#include "dijkstra.h"

using namespace std;
using namespace std::chrono;

// 性能测试：
// 对 5 个数据集分别计时「读图」和「dijkstra」，输出一张性能表。
// 计时原理：用 high_resolution_clock::now() 在要测的代码前后各取一次时间，
// 两个时间相减就是这段代码的耗时，再用 duration_cast 换算成毫秒。
int main() {
    // 要测试的 5 个图文件
    string files[] = {
        "D:/cc1/dimacs-data/USA-road-d.NY.gr",
        "D:/cc1/dimacs-data/USA-road-d.COL.gr",
        "D:/cc1/dimacs-data/USA-road-d.BAY.gr",
        "D:/cc1/dimacs-data/USA-road-d.CAL.gr",
        "D:/cc1/dimacs-data/USA-road-d.FLA.gr",
    };

    // 逐个文件测试
    for (string f : files) {
        // ---- 计时读图 ----
        auto t1 = high_resolution_clock::now();
        int n, m;
        vector<node>* graph = read_graph(f, n, m);
        auto t2 = high_resolution_clock::now();

        if (graph == nullptr) {
            cout << "read failed: " << f << endl;
            continue;   // 读失败就跳过这个文件
        }

        // ---- 计时 dijkstra（统一测 1 号点到最后一个点 n）----
        auto t3 = high_resolution_clock::now();
        int64_t dist = dijkstra(graph, n, 1, n);
        auto t4 = high_resolution_clock::now();

        cout << "---- " << f << " ----" << endl;
        cout << "  n = " << n << ", m = " << m << endl;
        cout << "  read time = " << duration_cast<milliseconds>(t2 - t1).count() << " ms" << endl;
        cout << "  dijkstra(1," << n << ") = " << dist
             << ", time = " << duration_cast<milliseconds>(t4 - t3).count() << " ms" << endl;

        delete[] graph;   // 释放内存
    }

    return 0;
}
