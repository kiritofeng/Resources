#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename T, const T MAXVAL>
struct OrderStatisticSegmentTree {
    struct Node { Node *left = nullptr, *right = nullptr; int size = 0; };
    Node *root = nullptr;
    void insert(Node *&cur, T cL, T cR, T val) {
        if (cL > val || cR < val) return;
        if (!cur) cur = new Node();
        if (cL == cR) { cur->size++; return; }
        T m = cL + (cR - cL) / 2; insert(cur->left, cL, m, val); insert(cur->right, m + 1, cR, val);
        cur->size = (cur->left ? cur->left->size : 0) + (cur->right ? cur->right->size : 0);
    }
    void erase(Node *cur, T cL, T cR, T val) {
        if (cL > val || cR < val) return;
        if (cL == cR) {
           if (cur->size > 0) cur->size--;
           return;
        }
        T m = cL + (cR - cL) / 2;
        if (cur->left) erase(cur->left, cL, m, val);
        if (cur->right) erase(cur->right, m + 1, cR, val);
        cur->size = (cur->left ? cur->left->size : 0) + (cur->right ? cur->right->size : 0);
    }
    bool contains(Node *cur, T cL, T cR, T val) {
        if (!cur || cL > val || cR < val) return false;
        if (cL == cR) return cur->size > 0;
        T m = cL + (cR - cL) / 2; return contains(cur->left, cL, m, val) || contains(cur->right, m + 1, cR, val);
    }
    int getRank(Node *cur, T cL, T cR, T l, T r) {
        if (!cur || cL > r || cR < l) return 0;
        if (cL >= l && cR <= r) return cur->size;
        T m = cL + (cR - cL) / 2; return getRank(cur->left, cL, m, l, r) + getRank(cur->right, m + 1, cR, l, r);
    }
    T select(Node *cur, T cL, T cR, int k) {
        if (cL == cR) return cL;
        int t = cur->left ? cur->left->size : 0;
        T m = cL + (cR - cL) / 2;
        if (t > k) return select(cur->left, cL, m, k);
        else return select(cur->right, m + 1, cR, k - t);
    }
    void values(Node *cur, T cL, T cR, vector<T> &queue) {
        if (!cur) return;
        if (cL == cR) {
            for (int i = 0; i < cur->size; i++) queue.push_back(cL);
            return;
        }
        T m = cL + (cR - cL) / 2; values(cur->left, cL, m, queue); values(cur->right, m + 1, cR, queue);
    }
    void clear(Node *cur) {
        if (!cur) return;
        clear(cur->left); clear(cur->right); delete cur;
    }
    void clear() { clear(root); root = nullptr; }
    void insert(T val) { insert(root, 0, MAXVAL, val); }
    void erase(T val) { erase(root, 0, MAXVAL, val); }
    bool contains(T val) { return contains(root, 0, MAXVAL, val); }
    int getRank(T val) { return getRank(root, 0, MAXVAL, 0, val - 1); }
    T select(int k) {
        if (!root || k < 0 || k >= root->size) throw invalid_argument("k is not in range 0 to size");
        return select(root, 0, MAXVAL, k);
    }
    vector<T> values() { vector<T> queue; values(root, 0, MAXVAL, queue); return queue; }
    bool isEmpty() { return !root || root->size == 0; }
    int size() { return root ? root->size : 0; }
};
