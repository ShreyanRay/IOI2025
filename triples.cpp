#include "triples.h"
#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

int count_triples(vector <int32_t> a){
    int n = a.size();
    int ans = 0;
    for (int i = 0; i < n; i++){
        if (i + a[i] < n){
            int j = i + a[i];
            if (a[j] < a[i]){
                int k = j - a[j];
                if (a[k] + a[j] == a[i]){
                    ans++;
                }
                
                if (2 * a[j] != a[i]){
                    k = i + a[j];
                    if (a[k] + a[j] == a[i]){
                        ans++;
                    }
                }
            }
        }
        
        if (i - a[i] >= 0){
            int j = i - a[i];
            if (a[j] < a[i]){
                int k = j + a[j];
                if (a[k] + a[j] == a[i]){
                    ans++;
                }
                
                if (2 * a[j] != a[i]){
                    k = i - a[j];
                    if (a[k] + a[j] == a[i]){
                        ans++;
                    }
                }
            }
        }
    }
    
    vector <int> b[2 * n + 1];
    for (int i = 0; i < n; i++){
        b[a[i] - i + n].push_back(i);
    }
    
    int cutoff = sqrt(n);
    
    for (int x = 1; x <= 2 * n; x++){
        if (b[x].size() <= cutoff){
            for (int v1 = 0; v1 < b[x].size(); v1++){
                int i = b[x][v1];
                for (int v2 = 0; v2 < v1; v2++){
                    int j = b[x][v2];
                    int k = j + a[i];
                    if (k < n && k > i){
                        if (a[i] == (k - j) && a[j] == (k - i) && a[k] == (i - j)){
                            ans++;
                        }
                    }
                }
            }
        } else {
            for (int k = 0; k < n; k++){
                int C = x - n;
                int sum = k - C;
                int diff = a[k];
                
                if (diff % 2 == sum % 2){
                    int i = (sum - diff) / 2;
                    int j = (sum + diff) / 2;
                    
                    if (0 <= i && i < n && 0 <= j && j < n && a[i] - i == C && a[j] - j == C){
                        ans++;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < n; i++){
        int j = i + a[i];
        if (j < n && a[j] > a[i]){
            int k = i + a[j];
            if (k < n){
                if (a[k] + a[i] == a[j] && a[k] != a[i]){
                    ans++;
                }
            }
        }
    }
    
    return ans;
}

std::vector<int32_t> construct_range(int32_t M, int32_t K) {
  return {1, 1, 1};
}
