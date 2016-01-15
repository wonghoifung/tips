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
	void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
		if (nums2.empty() || n == 0) return;
		int i = m + n - 1;
		int i1 = m - 1;
		int i2 = n - 1;
		while (i1 >= 0 && i2 >= 0)
		{
			nums1[i--] = nums1[i1] > nums2[i2] ? nums1[i1--] : nums2[i2--];
		}
		while (i1 >= 0)
		{
			nums1[i--] = nums1[i1--];
		}
		while (i2 >= 0)
		{
			nums1[i--] = nums2[i2--];
		}
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums1{ 4, 5, 6, 0, 0, 0 };
		int m = 3;
		vector<int> nums2{ 1, 2, 3 };
		int n = 3;
		s.merge(nums1, m, nums2, n);
		for (int i = 0; i < m + n; ++i)
		{
			cout << nums1[i] << " ";
		}
		cout << endl;
		copy(nums1.begin(), nums1.end(), ostream_iterator<int>(cout, " ")); 
		cout << endl;
	}

	std::cin.get();
	return 0;
}




