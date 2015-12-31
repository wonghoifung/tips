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
	vector<pair<int, int>> getSkyline(vector<vector<int>>& buildings) {
		multimap<int, int> coords;
		for (const auto& b : buildings) {
			coords.emplace(b[0], b[2]);
			coords.emplace(b[1], -b[2]);
		}

		multiset<int> heights = { 0 };
		map<int, int> corners;
		for (const auto& p : coords) {
			if (p.second > 0) heights.insert(p.second);
			else heights.erase(heights.find(-p.second));
			int cur_y = *heights.rbegin();
			corners[p.first] = cur_y;
		}

		auto comp = [](pair<int, int> l, pair<int, int> r) { return l.second == r.second; };
		vector<pair<int, int>> results;
		unique_copy(corners.begin(), corners.end(), back_insert_iterator<decltype(results)>(results), comp);

		return results;
	}
};

int main()
{
	Solution solu;
	{
		vector<vector<int>> buildings{ { 2, 9, 10 }, { 3, 7, 15 }, { 5, 12, 12 }, { 15, 20, 10 }, { 19, 24, 8 } };
		vector<pair<int, int>> res = solu.getSkyline(buildings);
		for (const auto& v : res)
		{
			cout << v.first << ", " << v.second << endl;
		}
	}

	std::cin.get();
	return 0;
}




