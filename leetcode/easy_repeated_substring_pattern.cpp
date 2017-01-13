#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Solution {
	bool check(string& str, int repeat) {
		int len = str.size();
		string sub = str.substr(0, len / repeat);
		for (int i = 0; i < len; ++i) {
			if (str[i] != sub[i % (len / repeat)])
				return false;
		}
		return true;
	}
public:
	bool repeatedSubstringPattern(string str) {
		int len = str.size();
		if (len < 2) return false;
		for (int r = 2; r <= len; ++r) {
			if (len % r != 0) continue;
			if (check(str, r)) return true;
		}
		return false;
	}
};

int main() {
	auto&& sol = Solution();
	cout << boolalpha << sol.repeatedSubstringPattern("abab") << endl; // true
	cout << boolalpha << sol.repeatedSubstringPattern("aba") << endl; // false
	cout << boolalpha << sol.repeatedSubstringPattern("abcabcabcabc") << endl; // true

	cin.get();
	return EXIT_SUCCESS;
}

