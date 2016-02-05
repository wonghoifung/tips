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
	bool wordBreak(string s, unordered_set<string>& wordDict) {
		vector<bool> dp(s.size() + 1);
		dp[0] = true;
		for (int len = 1; len <= s.size(); ++len)
		{
			for (int prelen = 0; prelen < len; ++prelen)
			{
				if (dp[prelen] && wordDict.count(s.substr(prelen, len - prelen))) {
					dp[len] = true;
					break;
				}
			}
		}
		return dp[s.size()];
	}
};

int main()
{
	Solution s;
	{
		string ss = "leetcode";
		unordered_set<string> wordDict{ "leet", "code" };
		cout << boolalpha << s.wordBreak(ss, wordDict) << endl;
	}
	{
		string ss = "abcd";
		unordered_set<string> wordDict{ "a", "abc", "b", "cd" };
		cout << boolalpha << s.wordBreak(ss, wordDict) << endl;
	}

	std::cin.get();
	return 0;
}




