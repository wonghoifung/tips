#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <array>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

using namespace std;

class Solution {
public:
	double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
		int cnt = nums1.size() + nums2.size();
		if (cnt % 2) {
			double res = 0;
			int idx = cnt / 2;
			int i1 = 0, i2 = 0;
			while (idx >= 0)
			{
				if (i1 >= nums1.size()) { 
					res = nums2[i2++]; idx -= 1; 
					if (idx < 0) return res; 
				}
				else if (i2 >= nums2.size()) { 
					res = nums1[i1++]; idx -= 1; 
					if (idx < 0) return res; 
				}
				else {
					if (nums1[i1] < nums2[i2]) {
						res = nums1[i1];
						i1 += 1;
						idx -= 1;
						if (idx < 0) return res;
					}
					else {
						res = nums2[i2];
						i2 += 1;
						idx -= 1;
						if (idx < 0) return res;
					}
				}
			}
		}
		else {
			double pre = 0, res = 0;
			int idx = cnt / 2;
			int i1 = 0, i2 = 0;
			while (idx >= 0)
			{
				if (i1 >= nums1.size()) { 
					pre = res; res = nums2[i2++]; idx -= 1; 
					if (idx < 0) return ((double)pre + res) / 2; 
				}
				else if (i2 >= nums2.size()) { 
					pre = res; res = nums1[i1++]; idx -= 1;
					if (idx < 0) return ((double)pre + res) / 2; 
				}
				else {
					if (nums1[i1] < nums2[i2]) {
						pre = res;
						res = nums1[i1];
						i1 += 1;
						idx -= 1;
						if (idx < 0) return ((double)pre + res) / 2;
					}
					else {
						pre = res;
						res = nums2[i2];
						i2 += 1;
						idx -= 1;
						if (idx < 0) return ((double)pre + res) / 2;
					}
				}
			}
		}
	}
};

int main()
{
	Solution solu;
	{
		vector<int> nums1{ 1 };
		vector<int> nums2{ 1 };
		cout << solu.findMedianSortedArrays(nums1, nums2) << endl;
	}

	std::cin.get();
	return 0;
}




