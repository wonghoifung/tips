#include <iostream>
#include <vector>
#include <map>
#include <limits.h>

using namespace std;

class Solution {
public:
    vector<int> fallingSquares(vector<pair<int, int>>& p) {
        map<pair<int,int>, int> mp;
        mp[{0,1000000000}] = 0;
        vector<int> ans;
        int mx = 0;
        for (auto &v:p) {
            vector<vector<int>> toAdd;
            int len = v.second, a = v.first, b =v.first + v.second, h = 0;
            auto it = mp.upper_bound({a,a});
            if (it != mp.begin() && (--it)->first.second <= a) ++it;
            while (it != mp.end() && it->first.first < b) {
                if (a > it->first.first) toAdd.push_back({it->first.first,a,it->second});
                if (b < it->first.second) toAdd.push_back({b,it->first.second,it->second});
                h = max(h, it->second);
                it = mp.erase(it);
            }
            mp[{a,b}] = h + len;
            for (auto &t:toAdd) mp[{t[0],t[1]}] = t[2];
            mx = max(mx, h + len);
            ans.push_back(mx);
        }
        
        return ans;
    }
};

int main() {
    vector<pair<int, int>> positions = {{1,2},{2,3},{6,1}};
    vector<int> res = Solution().fallingSquares(positions);
    for (int i=0; i<res.size(); ++i) cout << res[i] << endl;
}

