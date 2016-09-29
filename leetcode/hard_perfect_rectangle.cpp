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
	struct Point { int x, y; };
	struct hashPoint {
		size_t operator()(const Point& p) const {
			size_t h1 = std::hash < int > {}(p.x);
			size_t h2 = std::hash < int > {}(p.y);
			return h1 ^ (h2 << 1);
		}
	};
	struct equalPoint {
		bool operator()(const Point& p1, const Point& p2) const {
			return p1.x == p2.x && p1.y == p2.y;
		}
	};
public:
	bool isRectangleCover(vector<vector<int>>& rectangles) {
		int mostbottom = INT_MAX;
		int mostleft = INT_MAX;
		int mosttop = INT_MIN;
		int mostright = INT_MIN;
		int area = 0;
		unordered_set<Point, hashPoint, equalPoint> points;

		auto adddelpoint = [&](const Point& p){
			if (points.count(p) == 0) {
				points.insert(p);
			}
			else {
				points.erase(p);
			}
		};

		for (int i = 0; i < rectangles.size(); ++i) {
			auto& rect = rectangles[i];

			Point bottomright{ rect[2], rect[1] };
			Point topleft{ rect[0], rect[3] };
			Point bottomleft{ rect[0], rect[1] };
			Point topright{ rect[2], rect[3] };

			adddelpoint(bottomright);
			adddelpoint(topleft);
			adddelpoint(bottomleft);
			adddelpoint(topright);

			area += ((topright.x - bottomleft.x) * (topright.y - bottomleft.y));

			mostleft = min(mostleft, bottomleft.x);
			mostbottom = min(mostbottom, bottomleft.y);
			mostright = max(mostright, topright.x);
			mosttop = max(mosttop, topright.y);
		}

		if (points.size() != 4) return false;
		if (points.find({ mostleft, mosttop }) == points.end()) return false;
		if (points.find({ mostleft, mostbottom }) == points.end()) return false;
		if (points.find({ mostright, mosttop }) == points.end()) return false;
		if (points.find({ mostright, mostbottom }) == points.end()) return false;

		int maxarea = (mostright - mostleft) * (mosttop - mostbottom);
		return area == maxarea;
	}
};

int main()
{
	{
		vector<vector<int>> rectangles = {
			{ 0, 0, 4, 1 },
			{ 7, 0, 8, 2 },
			{ 6, 2, 8, 3 },
			{ 5, 1, 6, 3 },
			{ 4, 0, 5, 1 },
			{ 6, 0, 7, 2 },
			{ 4, 2, 5, 3 },
			{ 2, 1, 4, 3 },
			{ 0, 1, 2, 2 },
			{ 0, 2, 2, 3 },
			{ 4, 1, 5, 2 },
			{ 5, 0, 6, 1 }
		};
		cout << boolalpha << Solution().isRectangleCover(rectangles) << endl;
	}
	{
		vector<vector<int>> rectangles = {
			{ 1, 1, 3, 3 },
			{ 3, 1, 4, 2 },
			{ 3, 2, 4, 4 },
			{ 1, 3, 2, 4 },
			{ 2, 3, 3, 4 }
		};
		cout << boolalpha << Solution().isRectangleCover(rectangles) << endl;
	}
	{
		vector<vector<int>> rectangles = {
			{ 1, 1, 2, 3 },
			{ 1, 3, 2, 4 },
			{ 3, 1, 4, 2 },
			{ 3, 2, 4, 4 }
		};
		cout << boolalpha << Solution().isRectangleCover(rectangles) << endl;
	}
	{
		vector<vector<int>> rectangles = {
			{ 1, 1, 3, 3 },
			{ 3, 1, 4, 2 },
			{ 1, 3, 2, 4 },
			{ 3, 2, 4, 4 }
		};
		cout << boolalpha << Solution().isRectangleCover(rectangles) << endl;
	}
	{
		vector<vector<int>> rectangles = {
			{ 1, 1, 3, 3 },
			{ 3, 1, 4, 2 },
			{ 1, 3, 2, 4 },
			{ 2, 2, 4, 4 }
		};
		cout << boolalpha << Solution().isRectangleCover(rectangles) << endl;
	}
	{
		vector<vector<int>> rectangles = { 
			{ 0, 0, 1, 1 },
			{ 0, 1, 3, 2 },
			{ 1, 0, 2, 2 } 
		};
		cout << boolalpha << Solution().isRectangleCover(rectangles) << endl;
	}
	
	std::cin.get();
	return 0;
}



