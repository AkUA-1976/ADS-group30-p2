#include "fibonacci_heap.h"

fib_node::fib_node(int v, int64_t val) : vertex(v), value(val), degree(0), mark(false),
                                         parent(nullptr), child(nullptr) {
    left = right = this;
}

fibonacci_heap::fibonacci_heap() : minNode(nullptr), n(0) {}

fibonacci_heap::~fibonacci_heap() {
    if (minNode != nullptr) destroyTree(minNode);
}

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

void fibonacci_heap::consolidate() {
    // 节点度数的上界约为 log_phi(n) ≈ 1.44*log2(n)，这里取 2*log2(n)+2 留足余量
    int maxDegree = (n > 1) ? static_cast<int>(2 * std::log2(n)) + 2 : 2;
    std::vector<fib_node*> A(maxDegree, nullptr);
    std::vector<fib_node*> rootNodes;
    fib_node* curr = minNode;
    if (curr != nullptr) {
        do {
            rootNodes.push_back(curr);
            curr = curr->right;
        } while (curr != minNode);
    }

    for (fib_node* w : rootNodes) {
        fib_node* x = w;
        int d = x->degree;
        while (d < maxDegree && A[d] != nullptr) {
            fib_node* y = A[d];
            if (x->value > y->value) std::swap(x, y);
            link(y, x);
            A[d] = nullptr;
            d++;
        }
        if (d < maxDegree) A[d] = x;
    }

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

fib_node* fibonacci_heap::extractmin() {
    fib_node* z = minNode;
    if (z != nullptr) {
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
        z->left->right = z->right;
        z->right->left = z->left;
        
        if (z == z->right) {
            minNode = nullptr;
        } else {
            minNode = z->right;
            consolidate();
        }
        n--;
    }
    return z;
}

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

void fibonacci_heap::decreaseKey(fib_node* x, int64_t newValue) {
    if (newValue > x->value) return;
    x->value = newValue;
    fib_node* y = x->parent;
    if (y != nullptr && x->value < y->value) {
        cut(x, y);
        cascadingCut(y);
    }
    if (x->value < minNode->value) minNode = x;
}