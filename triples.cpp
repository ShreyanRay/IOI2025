#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

int smart(int n, vector <int> a){
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
    
    int cutoff = 0;
    
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

int brute(int n, vector <int> a){
    int ans = 0;
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            for (int k = j + 1; k < n; k++){
                vector <int> v1 = {a[i], a[j], a[k]};
                vector <int> v2 = {j - i, k - j, k - i};
                sort(v1.begin(), v1.end());
                sort(v2.begin(), v2.end());
                
                ans += (v1 == v2);
            }
        }
    }
    
    return ans;
}

void Solve() 
{
    int n; cin >> n;
    
    vector <int> a(n);
    for (auto &x : a) cin >> x;
    
    cout << smart(n, a) << "\n";
}

int32_t main() 
{
    auto begin = std::chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // freopen("in",  "r", stdin);
    // freopen("out", "w", stdout);
    
 //   cin >> t;
    for(int i = 1; i <= t; i++) 
    {
        //cout << "Case #" << i << ": ";
        Solve();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cerr << "Time measured: " << elapsed.count() * 1e-9 << " seconds.\n"; 
    return 0;
}
