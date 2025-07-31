#include "souvenirs.h"
#include <bits/stdc++.h>
using namespace std;

#define int long long

void buy_souvenirs(int32_t n, int p0){
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
