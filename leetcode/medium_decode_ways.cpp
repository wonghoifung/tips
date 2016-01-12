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
	int numDecodings(string s) {
		if (s.empty()) return 0;
		vector<int> dp(s.size() + 1, 0);
		dp[0] = 1;
		dp[1] = (s.at(0) == '0' ? 0 : 1);
		for (int i = 1; i < s.size(); ++i)
		{
			if (s.at(i) == '0') {
				if (s.at(i - 1) == '0') return 0;
				else if (stoi(s.substr(i - 1, 2)) <= 26) dp[i + 1] = dp[i - 1];
				else return 0;
			}
			else {
				if (s.at(i - 1) != '0' && stoi(s.substr(i - 1, 2)) <= 26) dp[i + 1] = dp[i] + dp[i - 1];
				else dp[i + 1] = dp[i];
			}
		}
		return dp.back();
	}
};

int main()
{
	Solution s;
	{
		cout << s.numDecodings("1234") << endl; 
	}

	std::cin.get();
	return 0;
}




