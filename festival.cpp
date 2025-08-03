#include <bits/stdc++.h>
#include "festival.h"
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

vector <int32_t> max_coupons(int32_t AA, vector <int32_t> P, vector <int32_t> T){
    int A = AA;
    vector <array<int, 3>> a;
    int n = P.size();
    
    vector <array<int, 2>> b;
    
    for (int i = 0; i < n; i++){
        if (T[i] != 1){
            a.push_back({P[i], T[i], i});
        } else {
            b.push_back({P[i], i});
        }
    }
    
    sort(a.begin(), a.end(), [&](array <int, 3> b, array <int, 3> c){
        int v1 = -b[0] * b[1] * c[1] - c[0] * c[1];
        int v2 = -c[0] * c[1] * b[1] - b[0] * b[1];
        return v1 > v2;
    });
    
    sort(b.begin(), b.end());
    
    int m = a.size();
    int k = b.size();
    
    const int MX = 2e14;
    const int L = 71;
    
    vector<vector<int>> dp(n + 1, vector<int>(L, -INF));
    vector<vector<int>> par(n + 1, vector<int>(L, -1));
    
    int ptr = 0;
    bool have_seen = false;
    for (auto [p, t, _] : a){
        if ((A - p) * t >= A){
            assert(!have_seen);
            A = (A - p) * t;
            A = min((int)A, MX);
            ptr++;
        } else {
            have_seen = true;
        }
    }
    
    for (int i = 0; i <= n; i++){
        dp[i][0] = A;
    }
    
    for (int i = ptr + 1; i <= m; i++){
        auto [p, t, _] = a[i - 1]; 
        for (int j = 0; j < L; j++){
            if (dp[i - 1][j] >= 0){
                dp[i][j] = dp[i - 1][j];
                par[i][j] = 0;
            }
        }
        for (int j = 0; j + 1 < L; j++){
            if (dp[i - 1][j] >= p){
                int val = (dp[i - 1][j] - p) * t;
                val = min(val, MX);
                if (val > dp[i][j + 1]){
                    dp[i][j + 1] = val;
                    par[i][j + 1] = 1;
                }
            }
        }
    }
    
    vector <int> ps(k + 1, 0);
    for (int i = 1; i <= k; i++){
        ps[i] = ps[i - 1] + b[i - 1][0];
    }
    
    int ans = -1, id = -1, ok = -1;
    for (int i = 0; i < L; i++){
        if (dp[m][i] >= 0){
            int val = i;
            int lo = 0, hi = k;
            while (lo != hi){
                int mid = (lo + hi + 1) / 2;
                if (ps[mid] <= dp[m][i]){
                    lo = mid;
                } else {
                    hi = mid - 1;
                }
            }
            val += lo;
            
            if (val > ans){
                ans = val;
                id = i;
                ok = lo;
            }
        }
    }
    
    vector <int32_t> vec;
    for (int i = 0; i < ok; i++){
        vec.push_back(b[i][1]);
    }
    for (int i = m; i > ptr; i--){
        if (par[i][id] == 1){
            id--;
            vec.push_back(a[i - 1][2]);
        }
    }
    for (int i = ptr - 1; i >= 0; i--){
        vec.push_back(a[i][2]);
    }
    reverse(vec.begin(), vec.end());
    return vec;
}
