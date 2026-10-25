#pragma once
#include <vector>
#include <cstdint>
#include <cmath>
#include <iostream>

// 斐波那契堆节点
// 根链表和孩子链表都是循环双向链表：left/right 互为前驱后继，
// 只有一个结点时 left = right = 自己。child 指向孩子链表中的任意一个。
struct fib_node {
    int vertex;             // 图顶点编号
    int64_t value;          // 距离值
    int degree;             // 度数（孩子个数）
    bool mark;              // 级联剪切标记：成为别人孩子之后是否丢过孩子
    fib_node* parent;
    fib_node* child;
    fib_node* left;
    fib_node* right;

    fib_node(int v, int64_t val);
};

// 斐波那契堆
// 不强制每个阶只有一棵树，树的合并推迟到 extractmin 里的 consolidate；
// 因此 insert、merge、decreaseKey 的摊还代价都是 O(1)。
// minNode 指向根链表中的最小结点，n 记录结点总数。
class fibonacci_heap {
private:
    fib_node* minNode;
    int n;
    void consolidate();              // 把根链表中同阶的树两两合并，重建根链表
    void link(fib_node* y, fib_node* x); // 把 y 从根链表摘下，挂成 x 的孩子
    void cut(fib_node* x, fib_node* y);  // 把 x 从 y 的孩子链表摘下，放回根链表
    void cascadingCut(fib_node* y);      // y 已被标记过就继续向上剪切
    void destroyTree(fib_node* node);    // 析构时递归释放整棵树

public:
    fibonacci_heap();
    ~fibonacci_heap();

    // 插入一个结点，返回它的指针，之后可用于 decreaseKey
    fib_node* insert(int vertex, int64_t value);
    fib_node* getmin() const;
    // 删除并返回最小结点；返回的结点已脱离堆，由调用者 delete
    fib_node* extractmin();
    void decreaseKey(fib_node* x, int64_t newValue);
    bool empty() const;
    int size() const;
};
