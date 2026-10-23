#include "dijkstra_fib.h"
#include "fibonacci_heap.h"
#include <vector>
using namespace std;

int64_t dijkstra_fib(vector<node>* graph, int n, int st, int ed) {
    if (st == ed) return 0;

    // 沿用原代码的 -1 作为不可达/无穷大
    vector<int64_t> vertex(n + 1, -1);
    vector<fib_node*> nodes(n + 1, nullptr);
    vector<bool> valid(n + 1, false);

    fibonacci_heap heap;
    vertex[st] = 0;
    nodes[st] = heap.insert(st, 0);

    while (!heap.empty()) {
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
                        nodes[v] = heap.insert(v, newDist);
                    } else {
                        heap.decreaseKey(nodes[v], newDist);
                    }
                }
            }
        }
        delete minNode;
    }

    return vertex[ed]; // 不可达返回 -1
}