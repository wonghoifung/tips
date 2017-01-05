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
	int distance(const pair<int, int>& p1, const pair<int, int>& p2) {
		int dx = p1.first - p2.first;
		int dy = p1.second - p2.second;
		return dx * dx + dy * dy; // no need to take square root
	}
public:
	int numberOfBoomerangs(vector<pair<int, int>>& points) {
		int res = 0;
		unordered_map<int, int> m;
		for (int i = 0; i < points.size(); ++i) {
			for (int j = 0; j < points.size(); ++j) {
				if (i == j) continue;
				int d = distance(points[i], points[j]);
				m[d] += 1;
			}
			for (auto& p : m) {
				res += p.second * (p.second - 1); // total number of permutations of size 2 from n different points is n * (n-1)
			}
			m.clear();
		}
		return res;
	}
};

int main() {
	vector<pair<int, int>> points{ { 0, 0 }, { 1, 0 }, { 2, 0 } };
	cout << Solution().numberOfBoomerangs(points) << endl; // 2

	cin.get();
	return EXIT_SUCCESS;
}

