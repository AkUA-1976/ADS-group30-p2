#pragma once
#include <vector>
#include <cstdint>
#include <cmath>
#include <iostream>

// 斐波那契堆节点
struct fib_node {
    int vertex;             // 图顶点编号
    int64_t value;          // 距离值
    int degree;             // 度数
    bool mark;              // 级联剪切标记
    fib_node* parent;
    fib_node* child;
    fib_node* left;
    fib_node* right;

    fib_node(int v, int64_t val);
};

class fibonacci_heap {
private:
    fib_node* minNode;
    int n;
    void consolidate();
    void link(fib_node* y, fib_node* x);
    void cut(fib_node* x, fib_node* y);
    void cascadingCut(fib_node* y);
    void destroyTree(fib_node* node);

public:
    fibonacci_heap();
    ~fibonacci_heap();

    fib_node* insert(int vertex, int64_t value);
    fib_node* getmin() const;
    fib_node* extractmin();
    void decreaseKey(fib_node* x, int64_t newValue);
    bool empty() const;
    int size() const;
};