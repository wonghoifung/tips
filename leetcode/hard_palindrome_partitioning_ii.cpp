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
	int minCut(string s) {
		if (s.empty() || s.size() == 1) return 0;
		int cnt = s.size();
		vector<vector<bool>> isPal(cnt, vector<bool>(cnt, false));
		vector<int> mc(cnt);
		for (int i = cnt - 1; i >= 0; --i)
		{
			mc[i] = INT_MAX;
			for (int j = i; j < cnt; ++j)
			{
				if (s[i] == s[j] && (j - i < 2 || isPal[i + 1][j - 1]))
				{
					isPal[i][j] = true;
					if (j == cnt - 1)
					{
						mc[i] = 0;
					}
					else if (mc[j + 1] + 1 < mc[i])
					{
						mc[i] = mc[j + 1] + 1;
					}
				}
			}
		}
		return mc[0];
	}
};

int main()
{
	Solution s;
	{
		cout << s.minCut("apjesgpsxoeiokmqmfgvjslcjukbqxpsobyhjpbgdfruqdkeiszrlmtwgfxyfostpqczidfljwfbbrflkgdvtytbgqalguewnhvvmcgxboycffopmtmhtfizxkme"
			"ftcucxpobxmelmjtuzigsxnncxpaibgpuijwhankxbplpyejxmrrjgeoevqozwdtgospohznkoyzocjlracchjqnggbfeebmuvbicbvmpuleywrpzwsihivnrwtxcukwplgtobhgxu"
			"kwrdlszfaiqxwjvrgxnsveedxseeyeykarqnjrtlaliyudpacctzizcftjlunlgnfwcqqxcqikocqffsjyurzwysfjmswvhbrmshjuzsgpwyubtfbnwajuvrfhlccvfwhxfqthkcwha"
			"tktymgxostjlztwdxritygbrbibdgkezvzajizxasjnrcjwzdfvdnwwqeyumkamhzoqhnqjfzwzbixclcxqrtniznemxeahfozp") << endl; 
	}

	std::cin.get();
	return 0;
}




