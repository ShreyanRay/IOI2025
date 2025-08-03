#include <bits/stdc++.h>
#include "obstacles.h"
using namespace std;
//#define int long long
#define INF (int)2e9

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

struct ufds{
    int n;
    vector <int> root;
    vector <int> L, R;
    
    void init(int _n){
        n = _n;
        root.resize(n);
        L.resize(n);
        R.resize(n);
        for (int i = 0; i < n; i++){
            root[i] = i;
            L[i] = i;
            R[i] = i;
        }
    }
    
    int find(int x){
        if (x == root[x]){
            return x;
        }
        return root[x] = find(root[x]);
    }
    
    void unite(int x, int y){
        x = find(x);
        y = find(y);
        
        root[y] = x;
        L[x] = min(L[x], L[y]);
        R[x] = max(R[x], R[y]);
    }
};

struct Tree{
    int n;
    int get_max;
    vector <int> depth;
    vector <vector<int>> lift, liftw;
    vector <vector<pair<int, int>>> adj;
    const int L = 19;
    
    int merge(int x, int y){
        if (get_max){
            return max(x, y);
        }
        return min(x, y);
    }
    
    void init(int _n, int _get_max){
        n = _n;
        get_max = _get_max;
        depth.resize(n);
        lift.resize(n);
        liftw.resize(n);
        adj.resize(n);
        for (int i = 0; i < n; i++){
            lift[i].resize(L);
            liftw[i].resize(L);
        }
    }
    
    void add_edge(int u, int v, int w){
        adj[v].push_back({u, w});
    }
    
    void dfs(int u, int par = -1){
        for (auto [v, w] : adj[u]){
            depth[v] = depth[u] + 1;
            lift[v][0] = u;
            liftw[v][0] = w;
            dfs(v, u);
        }
    }
    
    void prepare(int root){
        depth[root] = 0;
        
        dfs(root);
        
        for (int i = 1; i < L; i++){
            for (int u = 0; u < n; u++){
                lift[u][i] = lift[lift[u][i - 1]][i - 1];
                liftw[u][i] = merge(liftw[u][i - 1], liftw[lift[u][i - 1]][i - 1]);
            }
        }
    }
    
    int path(int u, int v){
        if (depth[u] < depth[v]){
            swap(u, v);
        }
        
        int ans = get_max ? -INF : INF;
        
        int diff = depth[u] - depth[v];
        for (int i = 0; i < L; i++){
            if (diff >> i & 1){
                ans = merge(ans, liftw[u][i]);
                u = lift[u][i];
            }
        }
        
        if (u == v){
            return ans;
        }
        
        for (int i = L - 1; i >= 0; i--){
            if (lift[u][i] != lift[v][i]){
                ans = merge(ans, liftw[u][i]);
                ans = merge(ans, liftw[v][i]);
                u = lift[u][i];
                v = lift[v][i];
            }
        }
        
        ans = merge(ans, liftw[u][0]);
        ans = merge(ans, liftw[v][0]);
        
        return ans;
    }
};

int n, m;
const int N = 2e5 + 69;

int a[N], b[N];
int min_row[N];
int sega[4 * N];
int segb[4 * N];
Tree t1, t2;

void builda(int l, int r, int pos){
    if (l == r){
        sega[pos] = a[l];
        return;
    }
    
    int mid = (l + r) / 2;
    builda(l, mid, pos * 2);
    builda(mid + 1, r, pos * 2 + 1);
    
    sega[pos] = min(sega[pos * 2], sega[pos * 2 + 1]);
}

int querya(int l, int r, int pos, int ql, int qr){
    if (l >= ql && r <= qr){
        return sega[pos];
    } else if (l > qr || r < ql){
        return INF;
    } else {
        int mid = (l + r) / 2;
        return min(querya(l, mid, pos * 2, ql, qr), querya(mid + 1, r, pos * 2 + 1, ql, qr));
    }
}

void buildb(int l, int r, int pos){
    if (l == r){
        segb[pos] = b[l];
        return;
    }
    
    int mid = (l + r) / 2;
    buildb(l, mid, pos * 2);
    buildb(mid + 1, r, pos * 2 + 1);
    
    segb[pos] = min(segb[pos * 2], segb[pos * 2 + 1]);
}

int find_first(int l, int r, int pos, int ql, int x){
    if (r < ql){
        return INF;
    }
    if (segb[pos] >= x){
        return INF;
    }
    
    if (l == r){
        return l;
    }
    
    int mid = (l + r) / 2;
    int ans = find_first(l, mid, pos * 2, ql, x);
    if (ans == INF){
        ans = find_first(mid + 1, r, pos * 2 + 1, ql, x);
    }
    return ans;
}

int find_last(int l, int r, int pos, int qr, int x){
    if (l > qr){
        return -INF;
    }
    if (segb[pos] >= x){
        return -INF;
    }
    
    if (l == r){
        return l;
    }
    
    int mid = (l + r) / 2;
    int ans = find_last(mid + 1, r, pos * 2 + 1, qr, x);
    if (ans == -INF){
        ans = find_last(l, mid, pos * 2, qr, x);
    }
    return ans;
}

void initialize(vector <int> T, vector <int> H){
    n = T.size();
    m = H.size();
    
    for (int i = 0; i < n; i++){
        a[i] = T[i];
    }
    for (int i = 0; i < m; i++){
        b[i] = H[i];
    }
    
    vector <int> ord(m);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int x, int y){
        return b[x] < b[y];
    });
    
    for (int i = 0; i < m; i++){
        min_row[i] = n;
    }
    
    int ptr = 0;
    for (int i = 0; i < n; i++){
        while (ptr < m && b[ord[ptr]] < a[i]){
            min_row[ord[ptr]] = i;
            ptr++;
        }
    }
    
    ufds uf;
    uf.init(m);
    
    builda(0, m - 1, 1);
    buildb(0, m - 1, 1);
    
    vector <bool> alive(m, false);
    
    t1.init(m, 1);
    t2.init(m, 0);
    
    auto add1 = [&](int x, int y, int c){
        t1.add_edge(x, y, c);
    };
    
    auto add2 = [&](int x, int y, int c){
        t2.add_edge(x, y, c);
    };
    
    for (auto i : ord){
        if (i > 0 && alive[i - 1]){
            int x = uf.find(i - 1);
            int L = uf.L[x];
            int R = uf.R[x];
            
            int a = min_row[x];
            int b = min_row[i];
            int val = querya(0, m - 1, 1, a, b);
            if (b == n) val = 0;
            
            int v1 = find_first(0, m - 1, 1, L, val); 
            if (v1 > R) v1 = INF;
            int v2 = find_last(0, m - 1, 1, R, val);
            if (v2 < L) v2 = -INF;
            
            add1(x, i, v1);
            add2(x, i, v2);
            
            uf.unite(i, i - 1);
        }
        
        if (i + 1 < m && alive[i + 1]){
            int x = uf.find(i + 1);
            int L = uf.L[x];
            int R = uf.R[x];
            
            int a = min_row[x];
            int b = min_row[i];
            int val = querya(0, m - 1, 1, a, b);
            if (b == n) val = 0;
            
            int v1 = find_first(0, m - 1, 1, L, val);
            if (v1 > R) v1 = INF;
            int v2 = find_last(0, m - 1, 1, R, val);
            if (v2 < L) v2 = -INF;
            
            add1(x, i, v1);
            add2(x, i, v2);
            
            uf.unite(i, i + 1);
        }
        
        alive[i] = true;
    }
    
    t1.prepare(ord.back());
    t2.prepare(ord.back());
}

bool can_reach(int l, int r, int s, int d){
    return (t1.path(s, d) <= r) && (t2.path(s, d) >= l);
}
