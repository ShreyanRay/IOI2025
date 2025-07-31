#include <bits/stdc++.h>
using namespace std;
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

vector<vector<int32_t>> create_map(int32_t n, int32_t m, vector <int32_t> a, vector <int32_t> b){
    vector <int> c(4 * n - 1);
    vector <int> dfs_ord;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++){
        adj[a[i]].push_back(b[i]);
        adj[b[i]].push_back(a[i]);
    }
    
    vector <bool> vis(n + 1, false);
    vector <int> dep(n + 1, 0);
    vector <int> p(n + 1, -1);
    vector<vector<int>> ch(n + 1);
    vector <int> tin(n + 1), tout(n + 1);
    int timer = 0;
    
    auto calc_dfs = [&](auto self, int u, int par) -> void{
        vis[u] = true;
        p[u] = par;
        tin[u] = ++timer;
        
        for (int v : adj[u]){
            if (!vis[v]){
                ch[u].push_back(v);
                dep[v] = dep[u] + 1;
                self(self, v, u);
            } 
        }
        
        tout[u] = timer;
    };
    
    calc_dfs(calc_dfs, 1, -1);
    
    int id = 1;
    for (int i = 2; i <= n; i++){
        if (dep[i] > dep[id]){
            id = i;
        }
    }
    
    vector <bool> bad(n + 1, false);
    while (id != 1){
        bad[id] = true;
        id = p[id];
    }
    
    auto dfs = [&](auto self, int u, int par) -> void{
        dfs_ord.push_back(u);
        
        for (int v : ch[u]){
            if (!bad[v]){
                self(self, v, u);
                dfs_ord.push_back(u);
            }
        }
        
        for (int v : ch[u]){
            if (bad[v]){
                self(self, v, u);
                dfs_ord.push_back(u);
            }
        }
    };
    
    dfs(dfs, 1, -1);
    assert(dfs_ord.size() == 2 * n - 1);
    
    vector<vector<int>> need(n + 1);
    for (int i = 0; i < m; i++){
        int u = a[i], v = b[i];
        
        if (u == p[v] || v == p[u]) continue;
        
        if (tin[v] <= tin[u] && tout[v] >= tout[u]){
            swap(u, v);
        }
        
        need[v].push_back(u);
    }
    
    vector <bool> seen(n + 1, false);
    assert(need[1].size() == 0);
    
    vector<vector<int>> rows;
    for (auto x : dfs_ord){
        rows.push_back({x});
        if (!seen[x] && need[x].size() > 0){
            rows.push_back(need[x]);
            rows.push_back({x});
            seen[x] = true;
        }
    }
    
    assert(rows.size() <= 4 * n - 1);
    while (rows.size() < 4 * n - 1){
        rows.push_back(rows.back());
    }
    
    vector<vector<int>> ans(2 * n, vector<int>(2 * n));
    
    for (int i = 0; i < 4 * n - 1; i++){
        int sz = min(i + 1, 4 * n - i - 1);
        assert(sz >= rows[i].size());
        assert(rows[i].size() > 0);
        
        while (rows[i].size() < sz){
            rows[i].push_back(rows[i].back());
        }
        
        // diag is x + y = i - 1 
        int p = 0;
        
        for (int x = 0; x < 2 * n; x++){
            int y = i - x;
            if (0 <= y && y < 2 * n){
                assert(p < rows[i].size());
                ans[x][y] = rows[i][p];
                p++;
            }
        }
    }
    
    return ans;
}

void validate(vector<vector<int>> c, int n, int m, vector <int> a, vector <int> b){
    vector<vector<bool>> allow(n + 1, vector<bool>(n + 1, false));
    vector<vector<int>> done(n + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= n; i++){
        allow[i][i] = true;
    }
    
    for (int i = 0; i < m; i++){
        allow[a[i]][b[i]] = true;
        allow[b[i]][a[i]] = true;
    }
    
    assert(c.size() == 2 * n);
    for (int i = 0; i < 2 * n; i++) assert(c[i].size() == 2 * n);
    for (int i = 0; i < 2 * n; i++){
        for (int j = 0; j < 2 * n; j++){
            assert(1 <= c[i][j] && c[i][j] <= n);
        }
    }
    
    auto add = [&](int x, int y){
        assert(allow[x][y] == true);
        done[x][y]++;
        done[y][x]++;
    };
    
    for (int i = 0; i < 2 * n; i++){
        for (int j = 0; j < 2 * n; j++){
            if (i + 1 < 2 * n){
                add(c[i][j], c[i + 1][j]);
            }
            if (j + 1 < 2 * n){
                add(c[i][j], c[i][j + 1]);
            }
        }
    }
    
    for (int i = 0; i < m; i++){
        assert(done[a[i]][b[i]] >= 1);
    }
}
