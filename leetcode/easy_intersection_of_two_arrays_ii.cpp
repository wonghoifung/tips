#include <vector>
#include <iostream>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        unordered_map<int,int> num2cnt;
        for (auto i=0; i<nums1.size(); ++i) {
        	num2cnt[nums1[i]] += 1;
        } 
        for (auto i=0; i<nums2.size(); ++i) {
        	if (num2cnt[nums2[i]] >= 1) {
        		num2cnt[nums2[i]] -= 1;
        		res.push_back(nums2[i]);
        	}
        }
        return res;
    }
};

int main() {
	Solution s;
	{
		vector<int> nums1{1,2,2,1};
		vector<int> nums2{2,2};
		vector<int> res = s.intersect(nums1,nums2);
		for (auto i=0; i<res.size(); ++i) {
			cout << res[i] << endl;
		}
	}
}
