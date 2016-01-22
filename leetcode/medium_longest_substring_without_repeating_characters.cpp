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
	int lengthOfLongestSubstring(string s) {
		if (s.empty()) return 0;
		if (s.size() == 1) return 1;
		int cnt = 0;
		for (int i = 0; i < s.size(); ++i)
		{
			int seen[128] = { 0 };
			seen[s[i]] = 1;
			int curcnt = 1;
			bool brk = false;
			for (int j = i + 1; j < s.size(); ++j)
			{
				if (0 == seen[s[j]]) {
					curcnt += 1;
					seen[s[j]] = 1;
				}
				else {
					cnt = max(cnt, curcnt);
					brk = true;
					break;
				}
			}
			if (!brk) cnt = max(cnt, curcnt);
		}
		return cnt;
	}
};

int main()
{
	Solution s;
	{
		cout << s.lengthOfLongestSubstring("xxzqi") << endl;
	}

	std::cin.get();
	return 0;
}




