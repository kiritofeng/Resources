#pragma once
#include <bits/stdc++.h>
using namespace std;

class no_such_element_exception: public runtime_error {
public:
    no_such_element_exception(): runtime_error("No such element exists"){}
    no_such_element_exception(string message): runtime_error(message){}
};

// Decomposes the array into N ^ (1 / R) containers of size N ^ ((R - 1) / R) multiplied by a factor
// The factor should be between 1 and 10, and should be smaller for large N
// Time Complexity:
//   constructor: O(N)
//   insert, erase: O(N ^ (1 / R) + log(N))
//   pop_front: O(N ^ (1 / R))
//   pop_back: O(1) amortized
//   front, back, empty, size: O(1)
//   at, accessor, contains, floor, ceiling, above, below: O(log(N))
//   values: O(N)
// Memory Complexity: O(N)
template <const int R, class Value, class Container, class Comparator = less<Value>>
struct OrderedRootArray {
    Comparator cmp; int n, SCALE_FACTOR; vector<Container> a; vector<int> prefixSZ;
    int ceiling_ind(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].back(), val)) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }
    int floor_ind(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].front())) hi = mid - 1;
            else lo = mid + 1;
        }
        return hi;
    }
    int above_ind(const Value val) const {
        int lo = 0, hi = (int) a.size(), mid;
        while (lo < hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(val, a[mid].back())) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }
    int below_ind(const Value val) const {
        int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (cmp(a[mid].front(), val)) lo = mid + 1;
            else hi = mid - 1;
        }
        return hi;
    }
    OrderedRootArray(const int SCALE_FACTOR = 1) : n(0), SCALE_FACTOR(SCALE_FACTOR) {}
    template <typename It> OrderedRootArray(const It st, const It en, const int SCALE_FACTOR = 1) :
            n(en - st), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); assert(is_sorted(st, en, cmp));
        int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        for (It i = st; i < en; i += rootn) {
            a.emplace_back(i, min(i + rootn, en), SCALE_FACTOR);
            prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    OrderedRootArray(initializer_list<Value> il, const int SCALE_FACTOR = 1) :
            n(il.end() - il.begin()), SCALE_FACTOR(SCALE_FACTOR) {
        assert(n >= 0); assert(is_sorted(il.begin(), il.end(), cmp));
        int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        for (auto i = il.begin(); i < il.end(); i += rootn) {
            a.emplace_back(i, min(i + rootn, il.end()), SCALE_FACTOR);
            prefixSZ.push_back(0);
        }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void insert(const Value val) {
        int i = above_ind(val);
        if (n++ == 0) { a.emplace_back(SCALE_FACTOR); prefixSZ.push_back(0); }
        if (i == (int) a.size()) a[--i].insert(val);
        else a[i].insert(val);
        int rootn = (int) pow(n, (double) (R - 1) / R) * SCALE_FACTOR;
        if ((int) a[i].size() > 2 * rootn) {
            vector<Value> b;
            while (a[i].size() > rootn) { b.push_back(a[i].back()); a[i].pop_back(); }
            reverse(b.begin(), b.end()); a.emplace(a.begin() + i + 1, b.begin(), b.end(), SCALE_FACTOR); prefixSZ.push_back(0);
        }
        for (int j = i + 1; j < (int) a.size(); j++) prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
    }
    bool erase(const Value val) {
        int i = ceiling_ind(val);
        if (i == (int) a.size()) return false;
        if (!a[i].erase(val)) return false;
        --n;
        if (a[i].empty()) { a.erase(a.begin() + i); prefixSZ.pop_back(); }
        for (int j = i + 1; j < (int) a.size(); j++) prefixSZ[j] = prefixSZ[j - 1] + (int) a[j - 1].size();
        return true;
    }
    void pop_front() {
        assert(n > 0); --n; a.front().pop_front();
        if (a.front().empty()) { a.erase(a.begin()); prefixSZ.pop_back(); }
        for (int i = 1; i < (int) a.size(); i++) prefixSZ[i] = prefixSZ[i - 1] + (int) a[i - 1].size();
    }
    void pop_back() {
        assert(n > 0); --n; a.back().pop_back();
        if (a.back().empty()) { a.pop_back(); prefixSZ.pop_back(); }
    }
    const Value &at(const int k) const {
        assert(0 <= k && k < n); int lo = 0, hi = ((int) a.size()) - 1, mid;
        while (lo <= hi) {
            mid = lo + (hi - lo) / 2;
            if (k < prefixSZ[mid]) hi = mid - 1;
            else lo = mid + 1;
        }
        return a[hi].at(k - prefixSZ[hi]);
    }
    const Value &operator [](const int k) const { return at(k); }
    const Value &front() const { assert(n > 0); return a.front().front(); }
    const Value &back() const { assert(n > 0); return a.back().back(); }
    bool empty() const { return n == 0; }
    int size() const { return n; }
    bool contains(const Value val) const {
        int i = ceiling_ind(val);
        return i != (int) a.size() && a[i].contains(val);
    }
    pair<int, Value> floor(const Value val) const {
        int i = floor_ind(val);
        if (i == -1) throw no_such_element_exception("call to floor() resulted in no such value");
        pair<int, Value> j = a[i].floor(val);
        return {prefixSZ[i] + j.first, j.second};
    }
    pair<int, Value> above(const Value val) const {
        int i = above_ind(val);
        if (i == (int) a.size()) throw no_such_element_exception("call to above() resulted in no such value");
        pair<int, Value> j = a[i].above(val);
        return {prefixSZ[i] + j.first, j.second};
    }
    pair<int, Value> below(const Value val) const {
        int i = below_ind(val);
        if (i == -1) throw no_such_element_exception("call to below() resulted in no such value");
        pair<int, Value> j = a[i].below(val);
        return {prefixSZ[i] + j.first, j.second};
    }
    pair<int, Value> ceiling(const Value val) const {
        int i = ceiling_ind(val);
        if (i == (int) a.size()) throw no_such_element_exception("call to ceiling() resulted in no such value");
        pair<int, Value> j = a[i].ceiling(val);
        return {prefixSZ[i] + j.first, j.second};
    }
    vector<Value> values() const {
        vector<Value> ret;
        for (auto &&ai : a) for (auto &&aij : ai.values()) ret.push_back(aij);
        return ret;
    }
};