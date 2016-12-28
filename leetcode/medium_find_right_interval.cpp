#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct Interval {
    int start;
    int end;
    Interval() : start(0), end(0) {}
    Interval(int s, int e) : start(s), end(e) {}
};

#if 0
You may assume the interval's end point is always bigger than its start point.
You may assume none of these intervals have the same start point.
#endif
bool operator<(const Interval& i1, const Interval& i2) {
	return i1.start < i2.start;
}
class Solution {
public:
#if 0
	vector<int> findRightInterval(vector<Interval>& intervals) {
		if (intervals.empty()) return vector<int>();
		if (intervals.size() == 1) return vector<int>{-1};
		map<Interval, int> indexes;
		for (int i = 0; i < intervals.size(); ++i) {
			indexes[intervals[i]] = i;
		}
		sort(intervals.begin(), intervals.end());
		vector<int> res(intervals.size(), -1);
		for (int i = 0; i < intervals.size(); ++i) {
			auto& checked = intervals[i];
			for (int j = i + 1; j < intervals.size(); ++j) {
				if (intervals[j].start >= checked.end) {
					res[indexes[checked]] = indexes[intervals[j]];
					break;
				}
			}
		}
		return res;
	}
#else 
	vector<int> findRightInterval(vector<Interval>& intervals) {
		vector<int> result(intervals.size(), -1);
		map<int, int> intervalMap;
		for (int i = 0; i < intervals.size(); ++i) {
			intervalMap[intervals[i].start] = i;
		}
		for (int i = 0; i < intervals.size(); ++i) {
			auto it = intervalMap.lower_bound(intervals[i].end);
			if (it != intervalMap.end())
				result[i] = intervalMap[it->first];
		}
		return result;
	}
#endif
};

ostream& operator<<(ostream& o, vector<int>& arr) {
	o << "[ ";
	for (int i : arr) o << i << " ";
	o << "]" << endl;
	return o;
}
int main() {
	{
		vector<Interval> intervals{ { 1, 2 } };
		cout << Solution().findRightInterval(intervals) << endl; // [-1]
	}
	{
		vector<Interval> intervals{ { 3, 4 }, { 2, 3 }, { 1, 2 } };
		cout << Solution().findRightInterval(intervals) << endl; // [-1,0,1]
	}
	{
		vector<Interval> intervals{ { 1, 4 }, { 2, 3 }, { 3, 4 } };
		cout << Solution().findRightInterval(intervals) << endl; // [-1,2,-1]
	}
	cin.get();
	return EXIT_SUCCESS;
}

