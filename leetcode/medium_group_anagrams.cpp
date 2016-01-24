#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> res;
        unordered_map<string, vector<string>> m;
        for (string s:strs) {
            string ori=s;
            sort(s.begin(), s.end());
            m[s].push_back(ori);
        }
        for (auto it=m.begin(); it!=m.end(); ++it) {
            sort(it->second.begin(), it->second.end());
            res.push_back(it->second);
        }
        return res;
    }
    
};

int main(int argc, char** argv)
{
    Solution s;
    {
        vector<string> strs{"eat", "tea", "tan", "ate", "nat", "bat"};
        auto res=s.groupAnagrams(strs);
        for (const auto& v:res) {
            copy(v.begin(), v.end(), ostream_iterator<string>(cout, " "));
            cout<<endl;
        }
    }
    
    return 0;
}

