#pragma once
#include <bits/stdc++.h>
using namespace std;

// Euler Tour Treap supporting vertex updates and vertex/subtree queries
// Time Complexity:
//   constructor: O(N)
//   treeRoot, connected, addEdge, cutParent: O(log N)
//   updateVertex, queryVertexValue, querySubtreeValue, getPathFromRootValue: O(log N)
// Memory Complexity: O(N)

seed_seq seq {
    (uint64_t)chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
mt19937_64 rng64(seq); uniform_int_distribution<long long> dis;
using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0; const bool ISPRE = true, ISPOST = false;
Data merge(const Data &l, const Data &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
struct Node {
    Node *l, *r, *p; int vert, size; bool type; long long pri; Data val, sbtr;
    Node (int vert, bool type, const Data &val) : l(nullptr), r(nullptr), p(nullptr), vert(vert), size(1),
        type(type), pri(dis(rng64)), val(val), sbtr(val) {}
    void update() {
        size = 1; sbtr = val;
        if (l) { l->p = this; size += l->size; sbtr = merge(l->sbtr, sbtr); }
        if (r) { r->p = this; size += r->size; sbtr = merge(sbtr, r->sbtr); }
    }
    void apply(const Lazy &v) { val = applyLazy(val, v); sbtr = applyLazy(sbtr, v); }
};
int Size(Node *x) { return x ? x->size : 0; }
Data Val(Node *x) { return x ? x->val : vdef; }
Data Sbtr(Node *x) { return x ? x->sbtr : qdef; }
void merge(Node *&x, Node *l, Node *r) {
    if (!l || !r) { x = l ? l : r; }
    else if (l->pri > r->pri) { merge(l->r, l->r, r); x = l; }
    else { merge(r->l, l, r->l); x = r; }
    if (x) x->update();
}
void split(Node *x, Node *&l, Node *&r, int lsz) {
    if (!x) { l = r = nullptr; return; }
    x->p = nullptr;
    if (lsz <= Size(x->l)) { split(x->l, l, x->l, lsz); r = x; }
    else { split(x->r, x->r, r, lsz - Size(x->l) - 1); l = x; }
    x->update();
}
Node *min(Node *x) {
    if (!x) return nullptr;
    while (x->l) x = x->l;
    return x;
}
Node *root(Node *x) {
    if (!x) return nullptr;
    while (x->p) x = x->p;
    return x;
}
int index(Node *x) {
    if (!x) return -1;
    int ind = Size(x->l);
    for (; x->p; x = x->p) if (x->p->l != x) ind += 1 + Size(x->p->l);
    return ind;
}
struct EulerTourTreap {
    vector<Node> PRE, POST;
    EulerTourTreap(int N) {
        PRE.reserve(N); POST.reserve(N); Node *dummy = nullptr;
        for (int i = 0; i < N; i++) {
            PRE.emplace_back(i, ISPRE, vdef); POST.emplace_back(i, ISPOST, vdef); merge(dummy, &PRE.back(), &POST.back());
        }
    }
    template <class It> EulerTourTreap(It st, It en) {
        int N = en - st; PRE.reserve(N); POST.reserve(N); Node *dummy = nullptr;
        for (int i = 0; i < N; i++) {
            PRE.emplace_back(i, ISPRE, *(st + i)); POST.emplace_back(i, ISPOST, *(st + i)); merge(dummy, &PRE.back(), &POST.back());
        }
    }
    int treeRoot(int v) { return min(root(&PRE[v]))->vert; }
    bool connected(int v, int w) { return treeRoot(v) == treeRoot(w); }
    bool inSubtree(int v, int w) { // returns true if w is in the subtree of v
        int indW = index(&PRE[w]); return index(&PRE[v]) <= indW && indW <= index(&POST[v]);
    }
    void addEdge(int par, int ch) {
        Node *l, *r; split(root(&PRE[par]), l, r, index(&PRE[par]) + 1); merge(l, l, root(&PRE[ch])); merge(l, l, r);
    }
    void cutParent(int ch) {
        Node *l, *m, *r; split(root(&PRE[ch]), l, m, index(&PRE[ch])); split(m, m, r, index(&POST[ch]) + 1); merge(l, l, r);
    }
    Data getVertexValue(int v) {
        Node *l, *m, *r; split(root(&PRE[v]), l, m, index(&PRE[v])); split(m, m, r, 1);
        Data ret = Val(m); merge(l, l, m); merge(l, l, r); return ret;
    }
    Data getSubtreeValue(int v) { // value may be doubled due to double counting of pre and post
        Node *l, *m, *r; split(root(&PRE[v]), l, m, index(&PRE[v])); split(m, m, r, index(&POST[v]) + 1);
        Data ret = Sbtr(m); merge(l, l, m); merge(l, l, r); return ret;
    }
    Data getPathFromRootValue(int v) { // path from the root to a vertex, in Euler Tour order
        Node *l, *r; split(root(&PRE[v]), l, r, index(&PRE[v]) + 1);
        Data ret = Sbtr(l); merge(l, l, r); return ret;
    }
    void updateVertex(int v, const Lazy &val) {
        Node *l, *preV, *m, *postV, *r; split(root(&PRE[v]), l, preV, index(&PRE[v])); split(preV, preV, m, 1);
        split(m, m, postV, index(&POST[v])); split(postV, postV, r, 1);
        preV->apply(val); postV->apply(val); merge(l, l, preV); merge(l, l, m); merge(l, l, postV); merge(l, l, r);
    }
};
