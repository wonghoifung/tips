#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
using namespace std;

class Solution {
public:
	int longestPalindrome(string s) {
		unordered_map<char, int> charcount;
		for (char c : s) charcount[c] += 1;
		int res = 0;
		int left = 0;
		for (auto& p : charcount) {
			if (p.second % 2 == 0) {
				res += p.second;
			}
			else {
				res += (p.second - 1);
				left += 1;
			}
		}
		if (left) res += 1;
		return res;
	}
};

int main() {
	cout << Solution().longestPalindrome("abccccdd") << endl; // 7

	cin.get();
	return EXIT_SUCCESS;
}

