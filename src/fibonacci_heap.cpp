// 斐波那契堆的实现
// 根链表和孩子链表都是循环双向链表：只有一个结点时 left = right 指向自己，
// 遍历时用“回到起点”作为结束条件，因此不需要额外的头结点。
#include "fibonacci_heap.h"

// 新结点自己组成一个长度为 1 的循环链表
fib_node::fib_node(int v, int64_t val) : vertex(v), value(val), degree(0), mark(false),
                                         parent(nullptr), child(nullptr) {
    left = right = this;
}

fibonacci_heap::fibonacci_heap() : minNode(nullptr), n(0) {}

fibonacci_heap::~fibonacci_heap() {
    if (minNode != nullptr) destroyTree(minNode);
}

// 释放以 node 为起点的整条循环兄弟链；先递归释放每个结点的孩子，再删自己
void fibonacci_heap::destroyTree(fib_node* node) {
    if (node == nullptr) return;
    fib_node* start = node;
    do {
        fib_node* child = node->child;
        if (child != nullptr) destroyTree(child);
        fib_node* next = node->right;
        delete node;
        node = next;
    } while (node != start);
}

// 插入：把新结点接到根链表里 minNode 的左边，O(1)；返回指针供 decreaseKey 使用
fib_node* fibonacci_heap::insert(int vertex, int64_t value) {
    fib_node* node = new fib_node(vertex, value);
    if (minNode == nullptr) {
        minNode = node;
    } else {
        node->right = minNode->right;
        node->left = minNode;
        minNode->right->left = node;
        minNode->right = node;
        if (value < minNode->value) {
            minNode = node;
        }
    }
    n++;
    return node;
}

fib_node* fibonacci_heap::getmin() const { return minNode; }
bool fibonacci_heap::empty() const { return minNode == nullptr; }
int fibonacci_heap::size() const { return n; }

// 把 y 从根链表里摘下，挂成 x 的孩子（调用前保证 x 的键值不大于 y）。
// 挂到孩子链表末尾，y 的标记清空，x 的度数加一。
void fibonacci_heap::link(fib_node* y, fib_node* x) {
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if (x->child == nullptr) {
        x->child = y;
        y->left = y->right = y;
    } else {
        y->right = x->child->right;
        y->left = x->child;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = false;
}

// extractmin 之后的整理：把根链表中度数相同的树两两合并，保证剩下的树度数互不相同。
// 合并后度数为 d 的树至多一棵，所以 A 数组只需要 O(log n) 个位置。
void fibonacci_heap::consolidate() {
    // 节点度数的上界约为 log_phi(n) ≈ 1.44*log2(n)，这里取 2*log2(n)+2 留足余量
    int maxDegree = (n > 1) ? static_cast<int>(2 * std::log2(n)) + 2 : 2;
    std::vector<fib_node*> A(maxDegree, nullptr);
    std::vector<fib_node*> rootNodes;
    // 先把根链表里的结点收集起来：合并会修改链表，边遍历边改容易乱
    fib_node* curr = minNode;
    if (curr != nullptr) {
        do {
            rootNodes.push_back(curr);
            curr = curr->right;
        } while (curr != minNode);
    }

    // 每个根按度数放进 A 数组；位置被占了就合并，度数加一后继续找空位
    for (fib_node* w : rootNodes) {
        fib_node* x = w;
        int d = x->degree;
        while (d < maxDegree && A[d] != nullptr) {
            fib_node* y = A[d];
            if (x->value > y->value) std::swap(x, y); // 保证 x 是较小的根
            link(y, x);
            A[d] = nullptr;
            d++;
        }
        if (d < maxDegree) A[d] = x;
    }

    // 用 A 数组里剩下的树重建根链表，同时找出新的最小值
    minNode = nullptr;
    for (int i = 0; i < maxDegree; i++) {
        if (A[i] != nullptr) {
            if (minNode == nullptr) {
                minNode = A[i];
                minNode->left = minNode->right = minNode;
            } else {
                A[i]->right = minNode->right;
                A[i]->left = minNode;
                minNode->right->left = A[i];
                minNode->right = A[i];
                if (A[i]->value < minNode->value) minNode = A[i];
            }
        }
    }
}

// 删除最小结点并返回它（返回的结点已脱离堆，由调用者 delete）
fib_node* fibonacci_heap::extractmin() {
    fib_node* z = minNode;
    if (z != nullptr) {
        // 把 z 的孩子们逐个摘下来，接到根链表里（parent 置空）
        if (z->child != nullptr) {
            fib_node* child = z->child;
            do {
                fib_node* next = child->right;
                child->parent = nullptr;
                child->left = minNode;
                child->right = minNode->right;
                minNode->right->left = child;
                minNode->right = child;
                child = next;
            } while (child != z->child);
        }
        // 从根链表中删掉 z
        z->left->right = z->right;
        z->right->left = z->left;

        if (z == z->right) {
            minNode = nullptr; // z 是唯一的根，堆空了
        } else {
            minNode = z->right; // 先随便指一个，consolidate 会重建并找到真正的最小值
            consolidate();
        }
        n--;
    }
    return z;
}

// 把 x 从 y 的孩子链表里剪下来，放回根链表，并清掉 x 的标记。
// 如果 x 正好是 y->child 指向的结点，还要把 y->child 挪到它的兄弟上。
void fibonacci_heap::cut(fib_node* x, fib_node* y) {
    if (x->right == x) {
        y->child = nullptr;
    } else {
        x->left->right = x->right;
        x->right->left = x->left;
        if (y->child == x) y->child = x->right;
    }
    y->degree--;
    x->right = minNode->right;
    x->left = minNode;
    minNode->right->left = x;
    minNode->right = x;
    x->parent = nullptr;
    x->mark = false;
}

// 级联剪切：父结点没被标记过就标记它；已经标记过说明它此前已经丢过一个孩子，
// 把它也从它的父亲那里剪下来，并继续向上检查。
void fibonacci_heap::cascadingCut(fib_node* y) {
    fib_node* z = y->parent;
    if (z != nullptr) {
        if (y->mark == false) {
            y->mark = true;
        } else {
            cut(y, z);
            cascadingCut(z);
        }
    }
}

// 把 x 的键值减小到 newValue。若减小后比父结点还小，就把 x 剪到根链表；
// 若 x 本来就是根，只需要更新最小值指针。
void fibonacci_heap::decreaseKey(fib_node* x, int64_t newValue) {
    if (newValue > x->value) return; // 只支持减小；Dijkstra 不会传更大的值
    x->value = newValue;
    fib_node* y = x->parent;
    if (y != nullptr && x->value < y->value) {
        cut(x, y);
        cascadingCut(y);
    }
    if (x->value < minNode->value) minNode = x;
}