#include <vector>
#include <utility>
#include <cstdint>
#pragma once

// 二项队列（binomial queue）
// 每棵二项树用“左孩子 / 右兄弟”表示：firstson 指向第一个孩子，nextsib 指向下一个兄弟。
// 这样合并两棵树时只需要改几个指针，不用搬动整个孩子数组。
// 本实现是懒插入版本：对外只有 getmin / deletemin / insert / merge，没有 decrease-key，
// Dijkstra 中用一个 valid 数组跳过重复插入产生的过期项。

// 优先队列元素：距离值 + 顶点编号
struct heapnode{
    int64_t value;
    int vertex;
};

// 一棵二项树，根结点存放键值
class bitree{
private:
    heapnode node;              // 根结点的键值
    bitree* firstson = nullptr; // 第一个孩子
    bitree* nextsib = nullptr;  // 下一个兄弟
public:
    heapnode rootvalue();       // 取根结点的键值
    // 合并两棵同阶的二项树：键值小的做根，另一棵挂到它的孩子链表头部
    static bitree* merge(bitree* tree1, bitree* tree2);
    // 摘掉根结点，返回它的孩子们；孩子链表按阶从大到小排列
    std::vector<bitree*> deletemin();
    bitree(heapnode node):node(node){};
    ~bitree(){
        delete firstson;
        delete nextsib;
    };
};

// 二项队列：森林，不同阶的二项树至多各有一棵
class biqueue{
private:
    std::vector<bitree*> trees; // 按二项树的大小从大到小排列：trees[0] 是最高阶
    int findminID() const;      // 扫描所有根，返回最小值所在下标；空队列返回 -1
public:
    heapnode getmin() const;    // 取最小值（不删除）
    heapnode deletemin();       // 删除并返回最小值
    // 合并两个二项队列，返回合并后的队列；q2 的树会被并入 q1
    static biqueue* merge(biqueue* q1, biqueue* q2);
    void insert(heapnode n);    // 插入一个元素
    biqueue(std::vector<bitree*> init_trees = {}){
        trees = init_trees;
    }
    ~biqueue() {
    for (bitree* root : trees) {
        delete root;
    }
}
};
