#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum weighted independent set for a tree
// Time Complexity: O(V)
// Memory Complexity: O(V)
template <const int MAXV, class T> struct MaxWeightedIndependentSet {
    vector<int> adj[MAXV]; T val[MAXV], dp[MAXV][2];
    void addEdge(int v, int w) { adj[v].push_back(w); adj[w].push_back(w); }
    void addBiEdge(int v, int w) { addEdge(v, w); addEdge(w, v); }
    T dfs(int v, int prev, bool take) {
        if (dp[v][take] != numeric_limits<T>::max()) return dp[v][take];
        T ret = numeric_limits<T>::lowest();
        int cnt = 0;
        for (int w : adj[v]) {
            if (w == prev) continue;
            cnt++;
            if (!take) ret = max(ret, dfs(w, true));
            ret = max(ret, dfs(w, false));
        }
        if (cnt == 0) return take ? val[v] : 0;
        return take ? (dp[v][take] = ret + val[v]) : (dp[v][take] = ret);
    }
    void clear(int V = MAXV) { for (int i = 0; i < V; i++) adj[i].clear(); }
    T solve(int V, int root = 0) {
        for (int i = 0; i < V; i++) dp[i][0] = dp[i][1] = numeric_limits<T>::max();
        return max(dfs(root, true), dfs(root, false));
    }
};
