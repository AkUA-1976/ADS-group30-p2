#include <vector>
#include <utility>
#include <cstdint>
#pragma once

struct heapnode{
    int64_t value;
    int vertex;
};

class bitree{
private:
    heapnode node;
    bitree* firstson = nullptr;
    bitree* nextsib = nullptr;
public:
    heapnode rootvalue();
    static bitree* merge(bitree* tree1, bitree* tree2);
    std::vector<bitree*> deletemin();
    bitree(heapnode node):node(node){};
    ~bitree(){
        delete firstson;
        delete nextsib;
    };
};

class biqueue{
private:
    std::vector<bitree*> trees; // 按二项树的大小从大到小排列
    int findminID() const;
public:
    heapnode getmin() const;
    heapnode deletemin();
    static biqueue* merge(biqueue* q1, biqueue* q2);
    void insert(heapnode n);
    biqueue(std::vector<bitree*> init_trees = {}){
        trees = init_trees;
    }
    ~biqueue() {
    for (bitree* root : trees) {
        delete root;
    }
}
};