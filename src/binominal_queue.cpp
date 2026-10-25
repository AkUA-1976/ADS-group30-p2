// 二项队列的实现
// trees 中下标 0 对应最高阶，下标越大阶数越小；合并时从末尾（最低阶）开始，
// 像二进制加法一样从低位向高位处理进位。
#include "binominal_queue.h"
#include <vector>
#include <utility>
using namespace std;

// heapnode
// 空队列时 getmin / deletemin 的返回值；vertex = -1 表示队列已空
heapnode nullnode = {-1, -1};



// bitree
heapnode bitree::rootvalue(){
    return node;
}

// 合并两棵同阶二项树：键值小的当根，另一棵挂到它的孩子链表头部。
// 孩子链表保持“阶从大到小”，所以新挂上的树（阶最小）放在最前面。
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

// 摘掉根结点，把它的孩子拆成独立的树返回。
// 孩子链表按阶从大到小排列，拆开后正好是一组合法的二项树。
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
// 最小值一定在某个根上，线性扫描所有根即可；空队列返回 -1
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

    // 先把最小根摘下来，记下它的键值
    bitree* mintree = trees[minID];
    heapnode min = mintree->rootvalue();
    trees[minID] = nullptr;
    if(minID == 0){trees.erase(trees.begin());} // 删掉最高阶那一格，避免 trees 只增不减

    // 最小根的孩子本身构成一个二项队列（阶 k-1..0），和剩下的森林合并即可
    vector<bitree*> sons = mintree->deletemin();
    biqueue newqueue = biqueue(sons);
    merge(this, &newqueue);

    delete mintree; // 现在它已经没有孩子，可以安全释放
    return min; // 返回被删去的最小值（节点）
}

// 合并两个二项队列：把 q2 的树并入 q1。
// 先保证 q1 更长，然后从最低阶开始逐位相加：同一阶有两棵树时合并成 carry（进位），
// carry 再和更高一阶的树继续合并，过程和二进制加法完全一样。
biqueue* biqueue::merge(biqueue* q1, biqueue* q2){
    if(q1 == q2){return q1;} // 防止自身合并
    int64_t i;
    if(q1->trees.size() < q2->trees.size()){q1->trees.swap(q2->trees);}
    int m = q2->trees.size(), n = q1->trees.size();
    bitree* carry = nullptr, *tmp;
    for(i=0; i<m; i++){
        if(q1->trees[n-1-i] && q2->trees[m-1-i]){
            // 这一阶两棵树都有：合并成进位，原来的进位（如果有）填回这一格
            tmp = carry;
            carry = bitree::merge(q1->trees[n-1-i], q2->trees[m-1-i]);
            q1->trees[n-1-i] = tmp;
        }
        else if(carry && q1->trees[n-1-i]){
            // 只有 q1 有树：和进位合并，继续向上进位
            carry = bitree::merge(q1->trees[n-1-i], carry);
            q1->trees[n-1-i] = nullptr;
        }
        else if(carry && q2->trees[m-1-i]){
            // 只有 q2 有树：和进位合并，这一格仍然是空的
            carry = bitree::merge(carry, q2->trees[m-1-i]);
        }
        else if(q2->trees[m-1-i]){q1->trees[n-1-i] = q2->trees[m-1-i];}
        else if(carry){q1->trees[n-1-i] = carry;carry = nullptr;}
    }
    // q2 的树用完了；如果还有进位，就在 q1 剩余的高阶部分找位置
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
    // 进位超出最高阶：说明多出一棵更高阶的树，插到数组最前面
    if(carry){q1->trees.insert(q1->trees.begin(), carry);}
    q2->trees.clear(); // 树已经全部并入 q1，清空避免 q2 析构时重复释放
    return q1;
}

// 插入一个元素：把单结点包装成一个临时队列，再和当前队列合并
void biqueue::insert(heapnode node){
    vector<bitree*> init_trees{new bitree(node)};
    biqueue new_bq = biqueue(init_trees); // 这里不需要 new，因为只是临时包装
    merge(this, &new_bq);
}