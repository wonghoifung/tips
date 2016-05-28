
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        std::vector<int> res;
        unordered_map<int,int> num2cnt;
        multimap<int,int,greater<int>> cnt2num;
        for (auto i=0; i<nums.size(); ++i) {
        	int oldcnt = num2cnt[nums[i]];
        	num2cnt[nums[i]] += 1;
        	if (oldcnt == 0) {
        		cnt2num.insert(make_pair(1,nums[i]));
        	} else {
        		auto p = cnt2num.equal_range(oldcnt);
        		for (auto it = p.first; it != p.second; ++it) {
        			if (it->second == nums[i]) {
        				cnt2num.erase(it);
        				break;
        			}
        		}
        		cnt2num.insert(make_pair(oldcnt+1,nums[i]));
        	}
        }
        auto it = cnt2num.begin();
        for (auto i=0; i<k && it!=cnt2num.end(); ++i, ++it) {
        	res.push_back(it->second);
        }
        return res;
    }
};

int main() {
	Solution s;
	{
		std::vector<int> nums{1,1,1,2,2,3};
		int k = 2;
		std::vector<int> res = s.topKFrequent(nums,k);
		for (auto i=0; i<res.size(); ++i) {
			cout << res[i] << endl;
		}
	}

	return 0;
}
