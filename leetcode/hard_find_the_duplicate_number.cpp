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
	int findDuplicate(const vector<int>& nums) {
		int n = nums.size() - 1; // 1 to n
		int l = 1;
		int r = n;
		int m = (l + r) / 2;
		int lcnt = 0;
		int rcnt = 0;
		int mcnt = 0;
		bool iseven = nums.size() % 2 == 0;
		while (l < r)
		{
			m = (l + r) / 2;
			iseven = (r - l + 1) % 2 == 0;
			for (int i = 0; i < nums.size(); ++i)
			{
				if (iseven)
				{
					if (nums[i] <= m && nums[i] >= l) lcnt += 1;
					else if (nums[i] > m && nums[i] <= r) rcnt += 1;
				}
				else
				{
					if (nums[i] < m && nums[i] >= l) lcnt += 1;
					else if (nums[i] > m && nums[i] <= r) rcnt += 1;
					else if (nums[i] == m) mcnt += 1;
				}
			}
			if (mcnt > 1) return m;

			if (l + 1 == m && lcnt > rcnt && l - r == 1) {
				int cnt1 = 0, cnt2 = 0;
				for (int i = 0; i <= n; ++i) if (nums[i] == l && ++cnt1 > 1) return l; else if (nums[i] == m && ++cnt2 > 1) return m;
			}
			else if (m + 1 == r && rcnt > lcnt && r - l == 1) {
				int cnt1 = 0, cnt2 = 0;
				for (int i = 0; i <= n; ++i) if (nums[i] == r && ++cnt1 > 1) return r; else if (nums[i] == m && ++cnt2 > 1) return m;
			}

			if (lcnt > rcnt) { r = m; }
			else { l = m; }
			lcnt = 0;
			rcnt = 0;
			mcnt = 0;
		}
	}
};

int main()
{
	Solution s;
	{
		vector<int> nums{ 4, 3, 1, 4, 2 };
		cout << s.findDuplicate(nums) << endl;
	}

	std::cin.get();
	return 0;
}




