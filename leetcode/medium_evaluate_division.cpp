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
	/* floyd-warshall */
	vector<double> calcEquation(vector<pair<string, string>> equations, vector<double>& values, vector<pair<string, string>> queries) {
		unordered_map<string, unordered_map<string, double>> quot;
		for (int i = 0; i < equations.size(); ++i) {
			quot[equations[i].first][equations[i].first] = 1.0;
			quot[equations[i].second][equations[i].second] = 1.0;
			quot[equations[i].first][equations[i].second] = values[i];
			quot[equations[i].second][equations[i].first] = 1.0 / values[i];
		}
		for (auto& kp : quot) {
			auto& m = quot[kp.first];
			for (auto& ip : m) {
				for (auto& jp : m) {
					quot[ip.first][jp.first] = quot[ip.first][kp.first] * quot[kp.first][jp.first];
				}
			}
		}
		vector<double> res(queries.size(), -1.0);
		for (int i = 0; i < queries.size(); ++i) {
			auto& p = queries[i];
			auto it1 = quot.find(p.first);
			if (it1 != quot.end()) {
				auto it2 = it1->second.find(p.second);
				if (it2 != it1->second.end()) {
					res[i] = it2->second;
				}
			}
		}
		return res;
	}
};

int main()
{
	{
		vector<pair<string, string>> equations = { { "a", "b" }, { "b", "c" } };
		vector<double> values = { 2.0, 3.0 };
		vector<pair<string, string>> queries = { { "a", "c" }, { "b", "a" }, { "a", "e" }, { "a", "a" }, { "x", "x" } };
		auto res = Solution().calcEquation(equations, values, queries); // [6.0, 0.5, -1.0, 1.0, -1.0]
		for (auto d : res) {
			cout << d << " ";
		}
		cout << endl;
	}

	{
		vector<pair<string, string>> equations = { { "x1", "x2" }, { "x2", "x3" }, { "x3", "x4" }, { "x4", "x5" } };
		vector<double> values = { 3.0, 4.0, 5.0, 6.0 };
		vector<pair<string, string>> queries = { { "x1", "x5" }, { "x5", "x2" }, { "x2", "x4" }, { "x2", "x2" }, { "x2", "x9" }, { "x9", "x9" } };
		auto res = Solution().calcEquation(equations, values, queries); // [360.00000,0.00833,20.00000,1.00000,-1.00000,-1.00000]
		for (auto d : res) {
			cout << d << " ";
		}
		cout << endl;
	}

	std::cin.get();
	return 0;
}



