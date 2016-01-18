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

struct Interval {
    int start;
    int end;
    Interval() : start(0), end(0) {}
    Interval(int s, int e) : start(s), end(e) {}
};

class Solution {
public:
	bool isOverlap(const Interval& ivl1, const Interval& ivl2) {
		if (ivl1.start <= ivl2.start && ivl1.end >= ivl2.end) return true;
		if (ivl2.start <= ivl1.start && ivl2.end >= ivl1.end) return true;
		if (ivl2.start <= ivl1.end && ivl1.start <= ivl2.end && ivl1.end <= ivl2.end) return true;
		if (ivl1.start <= ivl2.end && ivl2.start <= ivl1.end && ivl2.end <= ivl1.end) return true;
		return false;
	}
	Interval mergeOne(const Interval& ivl1, const Interval& ivl2) {
		if (ivl1.start <= ivl2.start && ivl1.end >= ivl2.end) return ivl1;
		if (ivl2.start <= ivl1.start && ivl2.end >= ivl1.end) return ivl2;
		if (ivl2.start <= ivl1.end && ivl1.start <= ivl2.end && ivl1.end <= ivl2.end) return{ ivl1.start, ivl2.end };
		if (ivl1.start <= ivl2.end && ivl2.start <= ivl1.end && ivl2.end <= ivl1.end) return{ ivl2.start, ivl1.end };
		return{}; // error
	}
	vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
		vector<Interval> res;
		bool hasMerge = false;
		for (int i = 0; i < intervals.size(); ++i)
		{
			if (!isOverlap(intervals[i], newInterval))
			{
				if (intervals[i].end < newInterval.start) res.push_back(intervals[i]);
				else if (newInterval.end < intervals[i].start) {
					res.push_back(newInterval);
					for (int k = i; k < intervals.size(); ++k)
						res.push_back(intervals[k]);
					hasMerge = true;
					break;
				}
				continue;
			}
			hasMerge = true;
			Interval newivl = mergeOne(intervals[i], newInterval);
			int j = i + 1;
			for (; j < intervals.size(); ++j)
			{
				if (isOverlap(intervals[j], newivl))
					newivl = mergeOne(intervals[j], newivl);
				else break;
			}
			res.push_back(newivl);
			for (int k = j; k < intervals.size(); ++k)
			{
				res.push_back(intervals[k]);
			}
			break;
		}
		if (!hasMerge) res.push_back(newInterval);
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<Interval> intervals{ { 1, 2 }, { 3, 5 }, { 6, 7 }, { 8, 10 }, { 12, 16 } };
		Interval newInterval{ 4, 9 };
		vector<Interval> res = s.insert(intervals, newInterval);
		for (const auto& i : res) {
			cout << i.start << ", " << i.end << endl;
		}
		cout << endl;
	}
	{
		vector<Interval> intervals{ { 1, 5 } };
		Interval newInterval{ 0, 0 };
		vector<Interval> res = s.insert(intervals, newInterval);
		for (const auto& i : res) {
			cout << i.start << ", " << i.end << endl;
		}
		cout << endl;
	}

	std::cin.get();
	return 0;
}




