#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
	int getRow(char c, int rows[26]) {
		int idx = 0;
		if (c >= 'a' && c <= 'z') idx = c - 'a';
		else if (c >= 'A' && c <= 'Z') idx = c - 'A';
		else return 0;
		return rows[idx];
	}
public:
	vector<string> findWords(vector<string>& words) {
		int rows[26] = { 2, 3, 3, 2, 1, 2, 2, 2, 1, 2, 2, 2, 3, 3, 1, 1, 1, 1, 2, 1, 1, 3, 1, 3, 1, 3 };
		vector<string> res;
		for (auto& s : words) {
			bool samerow = true;
			for (int i = 1; i < s.size(); ++i) {
				if (getRow(s[i], rows) != getRow(s[i - 1], rows)) {
					samerow = false;
					break;
				}
			}
			if (samerow) res.push_back(s);
		}
		return res;
	}
};

int main() {
	{
		vector<string> words{ "Hello", "Alaska", "Dad", "Peace" };
		auto res = Solution().findWords(words);
		for (auto s : res) cout << s << " ";
		cout << endl;
	}

	cin.get();
}

