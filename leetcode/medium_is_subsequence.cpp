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
	bool isSubsequence(string s, string t) {
		int si = 0, ti = 0;
		while (ti < t.size()) {
			si += s[si] == t[ti++];
			if (si == s.size()) break;
		}
		return si == s.size();
	}
};

int main()
{
	cout << boolalpha << Solution().isSubsequence("abc", "ahbgdc") << endl; // true
	cout << boolalpha << Solution().isSubsequence("axc", "ahbgdc") << endl; // false
	
	std::cin.get();
	return 0;
}



