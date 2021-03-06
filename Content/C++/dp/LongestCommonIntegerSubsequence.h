#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest common subsequence between 2 arrays of integers
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
template <class T, const int MAXN, const int MAXM = MAXN> struct LCIS {
    T A[MAXN], B[MAXM], subsequence[MAXN > MAXM ? MAXN : MAXM]; int dp[MAXN][MAXM], len;
    int solve(int N, int M) {
        fill(dp[0], dp[0] + M + 1, 0);
        for (int i = 1; i <= N; i++) {
            dp[i][0] = 0;
            for (int j = 1; j <= M; j++) {
                if (A[i - 1] == B[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
                else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        len = dp[N][M];
        for (int i = N, j = M, k = len - 1; i > 0 && j > 0;) {
            if (A[i - 1] == B[j - 1]) { subsequence[k--] = A[i - 1]; i--; j--; }
            else if (dp[i - 1][j] > dp[i][j - 1]) i--;
            else j--;
        }
        return len;
    }
};
