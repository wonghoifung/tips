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
	string longestCommonPrefix(vector<string>& strs) {
		if (strs.empty()) return "";
		if (strs.size() == 1) return strs[0];
		int len = 0;
		while (true)
		{
			char c = strs[0][len];
			for (int i = 1; i < strs.size(); ++i)
			{
				if (len >= strs[i].size() || strs[i][len] != c) {
					return strs[i].substr(0, len);
				}
			}
			len += 1;
		}
		return "";
	}
};

int main()
{
	Solution s;
	{
	}

	std::cin.get();
	return 0;
}




