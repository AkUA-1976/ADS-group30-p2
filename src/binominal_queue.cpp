#include "binominal_queue.h"
#include <vector>
#include <utility>
using namespace std;

// heapnode
heapnode nullnode = {-1, -1};



// bitree
heapnode bitree::rootvalue(){
    return node;
}

bitree* bitree::merge(bitree* tree1, bitree* tree2){
    if(tree1->rootvalue().value > tree2->rootvalue().value){
        tree1->nextsib = tree2->firstson;
        tree2->firstson = tree1;
        return tree2;
    }
    else{
        tree2->nextsib = tree1->firstson;
        tree1->firstson = tree2;
        return tree1;
    }
}

vector<bitree*> bitree::deletemin(){
    vector<bitree*> sons;
    bitree* son = firstson, *next;
    firstson = nullptr;
    while(son){
        next = son->nextsib;
        sons.push_back(son);
        son->nextsib = nullptr;
        son = next;
    }
    // 在这里，这个bitree节点就可以析构了
    return sons;
}



// biqueue
int biqueue::findminID() const{
    int mintreeID = -1;
    for(int64_t i=0; i<trees.size(); i++){
        if(trees[i] && (mintreeID == -1 || trees[i]->rootvalue().value < trees[mintreeID]->rootvalue().value)){mintreeID = i;}
    }
    return mintreeID;
}

heapnode biqueue::getmin() const{
    int mintreeID = findminID();
    if(mintreeID == -1){return nullnode;}
    heapnode min = trees[mintreeID]->rootvalue();
    return min;
}

heapnode biqueue::deletemin(){
    int minID = findminID();
    if(minID == -1){return nullnode;}

    bitree* mintree = trees[minID];
    heapnode min = mintree->rootvalue();
    trees[minID] = nullptr;
    if(minID == 0){trees.erase(trees.begin());} // 确保二项堆长度不会超

    vector<bitree*> sons = mintree->deletemin();
    biqueue newqueue = biqueue(sons);
    merge(this, &newqueue);

    delete mintree;
    return min; // 返回被删去的最小值（节点）
}

biqueue* biqueue::merge(biqueue* q1, biqueue* q2){
    if(q1 == q2){return q1;} // 防止自身合并
    int64_t i;
    if(q1->trees.size() < q2->trees.size()){q1->trees.swap(q2->trees);}
    int m = q2->trees.size(), n = q1->trees.size();
    bitree* carry = nullptr, *tmp;
    for(i=0; i<m; i++){
        if(q1->trees[n-1-i] && q2->trees[m-1-i]){
            tmp = carry;
            carry = bitree::merge(q1->trees[n-1-i], q2->trees[m-1-i]);
            q1->trees[n-1-i] = tmp;
        }
        else if(carry && q1->trees[n-1-i]){
            carry = bitree::merge(q1->trees[n-1-i], carry);
            q1->trees[n-1-i] = nullptr;
        }
        else if(carry && q2->trees[m-1-i]){
            carry = bitree::merge(carry, q2->trees[m-1-i]);
        }
        else if(q2->trees[m-1-i]){q1->trees[n-1-i] = q2->trees[m-1-i];}
        else if(carry){q1->trees[n-1-i] = carry;carry = nullptr;}
    }
    while(i<n){
        if(carry && q1->trees[n-1-i]){
            carry = bitree::merge(q1->trees[n-1-i], carry);
            q1->trees[n-1-i] = nullptr;
            i++;
            continue;
        }
        else if(carry){q1->trees[n-1-i] = carry;}
        carry = nullptr;
        break;
    }
    if(carry){q1->trees.insert(q1->trees.begin(), carry);}
    q2->trees.clear();
    return q1;
}

void biqueue::insert(heapnode node){
    vector<bitree*> init_trees{new bitree(node)};
    biqueue new_bq = biqueue(init_trees); // 这里不需要new，因为只是包装
    merge(this, &new_bq);
}