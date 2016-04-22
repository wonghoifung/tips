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

using namespace std;

class Solution {
public:
	bool isPalindrome(string s) {
		for (size_t i = 0, j = s.size() - 1; i < s.size() / 2; ++i, --j) {
			if (s[i] != s[j]) return false;
		}
		return true;
	}
	vector<vector<int>> palindromePairs(vector<string>& words) {
		vector<vector<int>> res;
		unordered_map<string, int> dict;
		for (int i = 0; i < words.size(); ++i) {
			string key = words[i];
			reverse(key.begin(), key.end());
			dict[key] = i;
		}
		if (dict.find("") != dict.end()) {
			for (int i = 0; i < words.size(); ++i) {
				if (i == dict[""]) continue;
				if (isPalindrome(words[i]))
					res.push_back({ dict[""], i });
			}
		}
		for (int i = 0; i < words.size(); ++i) {
			for (int j = 0; j < words[i].size(); ++j) {
				string left = words[i].substr(0, j);
				string right = words[i].substr(j, words[i].size() - j);
				if (dict.find(left) != dict.end() && isPalindrome(right) && dict[left] != i) {
					res.push_back({ i, dict[left] });
				}
				if (dict.find(right) != dict.end() && isPalindrome(left) && dict[right] != i) {
					res.push_back({ dict[right], i });
				}
			}
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<string> words{ "abcd", "dcba", "lls", "s", "sssll" };
		auto res = s.palindromePairs(words);
		for (auto& vi : res) {
			cout << vi[0] << ", " << vi[1] << endl;
		}
	}

	std::cin.get();
	return 0;
}




