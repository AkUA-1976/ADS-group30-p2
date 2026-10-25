// 斐波那契堆版本的 Dijkstra（真正的 decreaseKey）
// vertex[v] = -1 表示无穷大；nodes[v] 保存顶点 v 在堆中的结点：
// 第一次到达 v 时 insert，之后发现更短距离时直接 decreaseKey，堆里不会出现重复项。
#include "dijkstra_fib.h"
#include "fibonacci_heap.h"
#include <vector>
using namespace std;

int64_t dijkstra_fib(vector<node>* graph, int n, int st, int ed) {
    if (st == ed) return 0; // 起点就是终点

    // 沿用原代码的 -1 作为不可达/无穷大
    vector<int64_t> vertex(n + 1, -1);
    vector<fib_node*> nodes(n + 1, nullptr);
    vector<bool> valid(n + 1, false);

    fibonacci_heap heap;
    vertex[st] = 0;
    nodes[st] = heap.insert(st, 0);

    while (!heap.empty()) {
        // 取出距离最小的结点；它已经从堆里摘下来，用完必须 delete
        fib_node* minNode = heap.extractmin();
        int p = minNode->vertex;
        int64_t d = minNode->value;
        
        // 已经处理过的节点（虽然用了真实的 decreaseKey 不会出现，但为了安全保留）
        if (valid[p]) {
            delete minNode;
            continue;
        }
        
        valid[p] = true;
        nodes[p] = nullptr;

        // 如果已经找到终点，直接返回
        if (p == ed) {
            delete minNode;
            return d;
        }

        // 遍历邻居
        for (size_t i = 0; i < graph[p].size(); i++) {
            int v = graph[p][i].vertex;
            int64_t w = graph[p][i].length;

            if (!valid[v]) {
                int64_t newDist = vertex[p] + w; 
                if (vertex[v] == -1 || newDist < vertex[v]) {
                    vertex[v] = newDist;
                    if (nodes[v] == nullptr) {
                        nodes[v] = heap.insert(v, newDist); // 第一次到达 v
                    } else {
                        heap.decreaseKey(nodes[v], newDist); // 已在堆里，直接改距离
                    }
                }
            }
        }
        delete minNode;
    }

    return vertex[ed]; // 不可达返回 -1
}