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
public:
	int firstUniqChar(string s) {
		unordered_map<char, int> m;
		for (auto c : s) m[c] += 1;
		for (int idx = 0; idx < s.size(); ++idx) {
			if (m[s[idx]] == 1) return idx;
		}
		return -1;
	}
};

int main()
{
	cout << Solution().firstUniqChar("leetcode") << endl; // 0
	cout << Solution().firstUniqChar("loveleetcode") << endl; // 2
	
	std::cin.get();
	return 0;
}



