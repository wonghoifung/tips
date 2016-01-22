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

struct Point {
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int a, int b) : x(a), y(b) {}
};

class Solution {
public:
	int maxPoints(vector<Point>& points) {
		if (points.empty()) return 0;
		if (points.size() <= 2) return points.size();
		int res = 0;
		for (int i = 0; i < points.size(); ++i) {
			unordered_map<double, int> hm;
			int samex = 1;
			int samep = 0;
			for (int j = 0; j < points.size(); ++j) {
				if (j != i) {
					if ((points[j].x == points[i].x) && (points[j].y == points[i].y)) samep += 1;
					if (points[j].x == points[i].x) {
						samex += 1;
						continue;
					}
					double k = (double)(points[j].y - points[i].y) / (double)(points[j].x - points[i].x);
					if (hm.find(k) != hm.end()) hm[k] += 1;
					else hm[k] = 2;
					res = max(res, hm[k] + samep);
				}
			}
			res = max(res, samex);
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




