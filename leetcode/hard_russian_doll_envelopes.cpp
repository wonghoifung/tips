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
#if 0
	map<pair<int, int>, set<pair<int, int>>> biggers;
public:
	string pairToString(const pair<int, int>& one) {
		stringstream ss;
		ss << "[" << one.first << "," << one.second << "]";
		return ss.str();
	}
	bool canPutInside(pair<int, int>& one, pair<int, int>& another) {
		return one.first < another.first && one.second < another.second;
	}
	int findMaxLevel(const pair<int, int>& one) {
		auto& biggerones = biggers[one];
		if (biggerones.empty()) return 1;
		int maxlevel = 0;
		for (auto it = biggerones.begin(); it != biggerones.end(); ++it) {
			int level = 1 + findMaxLevel(*it);
			maxlevel = max(level, maxlevel);
		}
		return maxlevel;
	}
	int maxEnvelopes(vector<pair<int, int>>& envelopes) {
		for (auto i = 0; i < envelopes.size(); ++i) {
			for (auto j = i + 1; j < envelopes.size(); ++j) {
				if (canPutInside(envelopes[i], envelopes[j])) {
					biggers[envelopes[i]].insert(envelopes[j]);
				}
				else if (canPutInside(envelopes[j], envelopes[i])) {
					biggers[envelopes[j]].insert(envelopes[i]);
				}
			}
		}

		for (auto it = biggers.begin(); it != biggers.end(); ++it) {
			cout << "core: " << pairToString(it->first) << endl;
			auto& bb = it->second;
			cout << "  ";
			for (auto bit = bb.begin(); bit != bb.end(); ++bit) {
				cout << pairToString(*bit) << " ";
			}
			cout << endl;
		}

		int maxelop = 0;
		for (auto it = biggers.begin(); it != biggers.end(); ++it) {
			int level = findMaxLevel(it->first);
			maxelop = max(level, maxelop);
		}
		return maxelop;
	}
#endif

public:
	bool canPutInside(pair<int, int>& one, pair<int, int>& another) {
		return one.first < another.first && one.second < another.second;
	}
	int maxEnvelopes(vector<pair<int, int>>& envelopes) {
		int res = 0, n = envelopes.size();
		vector<int> dp(n, 1);
		sort(envelopes.begin(), envelopes.end());
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < i; ++j) {
				if (canPutInside(envelopes[j], envelopes[i])) {
					dp[i] = max(dp[i], dp[j] + 1);
				}
			}
			res = max(res, dp[i]);
		}
		return res;
	}
};

int main()
{
	vector<pair<int, int>> envelopes = { { 5, 4 }, { 6, 4 }, { 6, 7 }, { 2, 3 } };
	cout << Solution().maxEnvelopes(envelopes) << endl; // 3 ([2,3] => [5,4] => [6,7])

	std::cin.get();
	return 0;
}




