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
public:
	int findMinArrowShots(vector<pair<int, int>>& points) {
		if (points.size() == 1) return 1;
		sort(points.begin(), points.end(), [](const pair<int, int>& p1, const pair<int, int>& p2){
			if (p1.second != p2.second) return p1.second < p2.second;
			return p1.first < p2.first;
		});
		int res = 0;
		int end = INT_MIN;
		for (auto& p : points) {
			if (p.first > end) {
				res += 1;
				end = p.second;
			}
		}
		return res;
	}
};

int main() {
	{
		vector<pair<int, int>> points = { { 10, 16 }, { 2, 8 }, { 1, 6 }, { 7, 12 } };
		cout << Solution().findMinArrowShots(points) << endl; // 2
	}
	{
		vector<pair<int, int>> points = { { -1, 1 }, { 0, 1 }, { 2, 3 }, { 1, 2 } };
		cout << Solution().findMinArrowShots(points) << endl; // 2
	}
	cin.get();
	return EXIT_SUCCESS;
}

