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
#include <stdarg.h>
#include <stdio.h>

using namespace std;

class Solution {
	int longestSubstr(string& s, int k, int l, int r) {
		if (r - l < k) return 0;
		int cnt[26], start = l, res = 0;
		memset(cnt, 0, sizeof(cnt));
		for (int i = l; i < r; ++i) ++cnt[s[i]];
		for (int i = l; i < r; ++i) {
			if (cnt[s[i]] < k) {
				res = max(longestSubstr(s, k, start, i), res);
				start = i + 1;
			}
		}
		if (start == l) return r - l;
		return max(longestSubstr(s, k, start, r), res);
	}
public:
	int longestSubstring(string s, int k) {
		for (int i = 0; i < s.length(); ++i) s[i] -= 'a';
		return longestSubstr(s, k, 0, s.length());
	}
};

int main()
{
	cout << Solution().longestSubstring("aaabb", 3) << endl; // 3
	cout << Solution().longestSubstring("ababbc", 2) << endl; // 5
	cout << Solution().longestSubstring("aaabbb", 3) << endl; // 6
	cout << Solution().longestSubstring("ababacb", 3) << endl; // 0
	
	std::cin.get();
	return 0;
}



