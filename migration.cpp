#include <bits/stdc++.h>
#include "migrations.h"
using namespace std;
// #define int long long
#define INF (int)1e18

mt19937_64 RNG(50);

vector<vector<int>> adj;
int dim1, dim2;
int n;

int get_dist(int x, int y){
    queue <int> q;
    q.push(x);
    vector <int> dp(n, INF);
    dp[x] = 0;
    
    while (!q.empty()){
        int u = q.front(); q.pop();
        
        for (int v : adj[u]) if (dp[v] == INF){
            dp[v] = dp[u] + 1;
            q.push(v);
        }
    }
    
    return dp[y];
}

int send_message(int _n, int i, int pi){
    if (i == 1){
        n = _n;
        adj.resize(n);
        for (int i = 0; i < n; i++){
            adj[i].clear();
        }
        dim1 = 0, dim2 = 1;
    }
    adj[i].push_back(pi);
    adj[pi].push_back(i);
    
    if (n - i > 21){
        if (get_dist(i, dim1) > get_dist(dim1, dim2)){
            dim2 = i;
        }
        if (get_dist(i, dim2) > get_dist(dim1, dim2)){
            dim1 = i;
        }
        return 0;
    }
    
    if (n - i > 14){
        if (get_dist(i, dim1) > get_dist(dim1, dim2)){
            dim2 = dim1;
            dim1 = i;
            return 4;
        } 
        if (get_dist(i, dim2) > get_dist(dim1, dim2)){
            dim1 = i;
            return 4;
        }
        
        int bt = (n - i) - 15;
        int cp = dim1;
        while (bt--){
            cp /= 4;
        }
        
        return (cp % 4);
    }
    
    int bt = (n - i) - 1;
    int cp = dim2;
    while (bt--){
        cp /= 2;
    }
    
    if (get_dist(i, dim1) > get_dist(dim1, dim2)){
        dim2 = i;
        return 0;
    }
    if (get_dist(i, dim2) > get_dist(dim1, dim2)){
        dim1 = i;
        return 1 + (cp % 2);
    }
    return 3 + (cp % 2);
}

pair<int, int> longest_path(vector <int> S){
    int u = 0, v = 0;
    
    int n = S.size();
    for (int i = 1; i < n; i++){
        if (n - i > 21){
            continue;
        }
        if (n - i > 14){
            if (S[i] == 4){
                u = i;
            } else {
                int bt = (n - i) - 15;
                int cp = u;
                for (int i = 0; i < bt; i++){
                    cp /= 4;
                }
                u += (S[i] - (cp % 4)) * pow(4, bt);
            }
        } else {
            if (S[i] == 0){
                v = i;
            } else if (S[i] <= 2){
                u = i;
                int bt = (n - i) - 1;
                int cp = v;
                for (int i = 0; i < bt; i++){
                    cp /= 2;
                }
                v += (S[i] - 1 - (cp % 2)) * pow(2, bt);
            } else {
                int bt = (n - i) - 1;
                int cp = v;
                for (int i = 0; i < bt; i++){
                    cp /= 2;
                }
                v += (S[i] - 3 - (cp % 2)) * pow(2, bt);
            }
        }
    }
    
    return make_pair(u, v);
}
