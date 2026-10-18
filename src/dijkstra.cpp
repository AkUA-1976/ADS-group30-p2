#include "binominal_queue.h"
#include "dijkstra.h"
#include <vector>
#include <utility>
using namespace std;

int64_t dijkstra(vector<node>* graph, int n, int st, int ed){
    // 初始化vertex列表
    vector<int64_t> vertex(n+1, -1);
    vector<bool> valid(n+1, false);
    vertex[st] = 0;
    valid[st] = true;
    // 用vector不用自己管理内存
    // 介于数据集从1开始编号，因此设置长度为n+1的列表，更方便

    // 创建二项队列和用于插入的node，将st插入队列
    biqueue heap;
    heapnode node = {0, st};
    heap.insert(node);

    // 开始搜索
    int p=st;
    while(p != ed){
        for(int64_t i=0; i<graph[p].size(); i++){
            int64_t l = graph[p][i].length;
            int v = graph[p][i].vertex;
            if(vertex[v] == -1 || vertex[p]+l < vertex[v]){
                vertex[v] = vertex[p] + l;
                node = {vertex[v], v};
                heap.insert(node);
            }
        }
        while(valid[p]){
            node = heap.deletemin();
            p = node.vertex;
            if(p == -1){break;}
        }
        if(p == -1){break;}
        valid[p] = 1;
    }

    // 返回结果
    return vertex[ed]; // 如果返回值是-1，就说明不可达
}

