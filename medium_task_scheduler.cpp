#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        map<char,int> mp;
        int count = 0;
        for (int i=0; i<tasks.size(); ++i)
        {
            mp[tasks[i]]++;
            count = max(count, mp[tasks[i]]);
        }
        
        int ans = (count-1)*(n+1);
        for (map<char,int>::iterator it=mp.begin(); it!=mp.end(); ++it) {
            if (it->second == count) ans+=1;
        }
        return max((int)tasks.size(), ans);
    }
};

int main() {

}
