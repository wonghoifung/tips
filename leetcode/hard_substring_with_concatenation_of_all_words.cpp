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
	vector<int> findSubstring(string s, vector<string>& words) {
		unordered_map<string, int> counts;
		for (const auto& word : words) counts[word] += 1;
		int slen = s.length();
		int wcnt = words.size();
		int wlen = words[0].length();
		vector<int> res;
		for (int i = 0; i < slen - wcnt * wlen + 1; ++i) {
			unordered_map<string, int> seen;
			int c = 0;
			for (; c < wcnt; ++c) {
				string word = s.substr(i + c * wlen, wlen);
				if (counts.find(word) != counts.end()) {
					seen[word] += 1;
					if (seen[word] > counts[word]) break;
				}
				else break;
			}
			if (c == wcnt) res.push_back(i);
		}
		return res;
	}
};

int main()
{
	Solution s;
	{

	}

	std::cin.get();
	return 0;
}




