#include <iostream>
#include <chrono>
#include <string>
#include <cstdlib>       // srand / rand
#include <iomanip>       // fixed / setprecision
#include "graph_reader.h"
#include "dijkstra.h"
#include "dijkstra_fib.h"

using namespace std;
using namespace std::chrono;

// 性能测试：对 5 个数据集，各跑 1000 对随机查询，
// 对比二项堆（dijkstra）和斐波那契堆（dijkstra_fib）的耗时与正确性。
// 正确性：两种堆跑同一批 1000 对查询，累计 checksum 应完全相等。
int main() {
    const int Q = 1000;            // 查询对数
    const unsigned SEED = 42;      // 固定随机种子，保证每次结果可复现

    string files[] = {
        "D:/cc1/dimacs-data/USA-road-d.NY.gr",
        "D:/cc1/dimacs-data/USA-road-d.COL.gr",
        "D:/cc1/dimacs-data/USA-road-d.BAY.gr",
        "D:/cc1/dimacs-data/USA-road-d.CAL.gr",
        "D:/cc1/dimacs-data/USA-road-d.FLA.gr",
    };

    for (string f : files) {
        // 读图（读图耗时不计入查询计时）
        int n, m;
        vector<node>* graph = read_graph(f, n, m);
        if (graph == nullptr) {
            cout << "read failed: " << f << endl;
            continue;
        }

        // 用固定种子生成 1000 对 (起点, 终点)，两个堆共用同一批查询
        vector<int> st(Q), ed(Q);
        srand(SEED);
        for (int i = 0; i < Q; i++) {
            st[i] = rand() % n + 1;   // 起点：1..n
            ed[i] = rand() % n + 1;   // 终点：1..n
        }

        // ---- 二项堆：跑 1000 对，累计 checksum、数可达对数、计时 ----
        int64_t checksum_binom = 0;
        int reach_binom = 0;
        auto t1 = high_resolution_clock::now();
        for (int i = 0; i < Q; i++) {
            int64_t d = dijkstra(graph, n, st[i], ed[i]);
            if (d != -1) {            // -1 表示不可达，不计入 checksum
                checksum_binom += d;
                reach_binom++;
            }
        }
        auto t2 = high_resolution_clock::now();

        // ---- 斐波那契堆：同样 1000 对 ----
        int64_t checksum_fib = 0;
        int reach_fib = 0;
        auto t3 = high_resolution_clock::now();
        for (int i = 0; i < Q; i++) {
            int64_t d = dijkstra_fib(graph, n, st[i], ed[i]);
            if (d != -1) {
                checksum_fib += d;
                reach_fib++;
            }
        }
        auto t4 = high_resolution_clock::now();

        // 换算成毫秒（用微秒计时，精度更高）
        double binom_ms = duration_cast<microseconds>(t2 - t1).count() / 1000.0;
        double fib_ms   = duration_cast<microseconds>(t4 - t3).count() / 1000.0;

        cout << "---- " << f << " ----" << endl;
        cout << "  顶点 n = " << n << ", 弧 m = " << m << endl;
        cout << "  查询对数 q = " << Q << endl;
        cout << fixed << setprecision(3);
        cout << "  二项堆   总耗时 = " << binom_ms << " ms, 平均每对 = " << binom_ms / Q << " ms" << endl;
        cout << "  斐波那契 总耗时 = " << fib_ms   << " ms, 平均每对 = " << fib_ms   / Q << " ms" << endl;
        cout << "  可达对数 = " << reach_binom << "/" << Q
             << "（二项堆）, " << reach_fib << "/" << Q << "（斐波那契）" << endl;
        cout << "  checksum(二项堆)   = " << checksum_binom << endl;
        cout << "  checksum(斐波那契) = " << checksum_fib << endl;
        cout << "  checksum 是否一致: " << (checksum_binom == checksum_fib ? "是" : "否") << endl;

        delete[] graph;
    }

    return 0;
}
