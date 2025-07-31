#include <bits/stdc++.h>
using namespace std;
#define int long long
#define INF (int)1e18

mt19937_64 RNG(chrono::steady_clock::now().time_since_epoch().count());

int n;
const int N = 105;
int hidden_p[N];
int counter[N];

pair <vector <int>, int> transaction(int m){
    assert(m >= hidden_p[n - 1] && m < hidden_p[0]);
    
    vector <int> bought;
    for (int i = 0; i < n; i++){
        if (m >= hidden_p[i]){
            m -= hidden_p[i];
            bought.push_back(i);
            counter[i]++;
        }
    }
    
    return make_pair(bought, m);
}

void buy_souvenirs(int n, int p0){
    vector<set<int>> depend(n);
    vector<int> sum(n);
    vector<int> ans(n, -1);
    ans[0] = p0;
    
    vector <int> counter(n, 0);
    
    while (true){
        int left = 0;
        for (int i = 0; i < n; i++){
            left += ans[i] == -1;
        }
        
        if (left == 0){
            break;
        }
        
        bool have = false;
        
        for (int i = n - 1; i >= 0; i--){
            if (ans[i] == -1) have = true;
            if (ans[i] != -1 && have){
                auto [vv, ll] = transaction(ans[i] - 1);
                set <int> ss;
                for (auto x : vv) ss.insert(x), counter[x]++;
                int got = ans[i] - 1 - ll;
                
                for (int j = 0; j < n; j++){
                    if (ans[j] != -1 && ss.count(j)){
                        ss.erase(j);
                        got -= ans[j];
                    }
                }
                
                depend[i + 1] = ss;
                sum[i + 1] = got;
                break;
            }
            if (depend[i].size() == 1){
                ans[i] = sum[i];
                depend[i].clear();
                
                for (int j = 0; j < n; j++){
                    if (depend[j].count(i)){
                        sum[j] -= ans[i];
                        depend[j].erase(i);
                    }
                }
                break;
            }
            if (depend[i].size()){
                int x = sum[i] / (int)depend[i].size();
                auto [vv, ll] = transaction(x);
                set <int> ss;
                for (auto x : vv) ss.insert(x), counter[x]++;
                
                int got = x - ll;
                
                for (int j = 0; j < n; j++){
                    if (ans[j] != -1 && ss.count(j)){
                        got -= ans[j];
                        ss.erase(j);
                    }
                }
                
                assert(!ss.empty());
                int k = *ss.begin();
                depend[k] = ss;
                sum[k] = got;
                break;
            }
        }
    }
    
    for (int i = 0; i < n; i++){
        while (counter[i] < i){
            counter[i]++;
            transaction(ans[i]);
        }
    }
}

void Solve() 
{
    cin >> n;
    
    for (int i = 0; i < n; i++){
        cin >> hidden_p[i];
        counter[i] = 0;
    }
    
    buy_souvenirs(n, hidden_p[0]);
    
    for (int i = 0; i < n; i++){
        assert(counter[i] == i);
    }
}

int32_t main() 
{
    auto begin = std::chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int t = 1;
    // freopen("in",  "r", stdin);
    // freopen("out", "w", stdout);
    
    // cin >> t;
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
