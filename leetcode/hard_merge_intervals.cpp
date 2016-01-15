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

ostream& operator<<(ostream& o, const Interval& ivl) {
	o << "[" << ivl.start << ", " << ivl.end << "]";
	return o;
}
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
		if (ivl2.start <= ivl1.end && ivl1.start <= ivl2.end && ivl1.end <= ivl2.end) return { ivl1.start, ivl2.end };
		if (ivl1.start <= ivl2.end && ivl2.start <= ivl1.end && ivl2.end <= ivl1.end) return { ivl2.start, ivl1.end };
		return {}; // error
	}
	vector<Interval> merge(vector<Interval>& intervals) {
		vector<Interval> candidates(intervals);
		bool needContinue = false;
		do{
			needContinue = false;
			vector<Interval> res;
			for (int i = 0; i < candidates.size(); ++i)
			{
				bool hasMerge = false;
				if (res.empty()) {
					res.push_back(candidates[i]);
					continue;
				}
				for (int j = res.size() - 1; j >= 0; --j)
				{
					if (isOverlap(res[j], candidates[i]))
					{
						needContinue = true;
						hasMerge = true;
						Interval tmp = mergeOne(res[j], candidates[i]);
						res[j] = tmp;
						break;
					}
				}
				if (!hasMerge) res.push_back(candidates[i]);
			}
			candidates = std::move(res);
		} while (needContinue);
		return candidates;
	}
};

int main()
{
	Solution s;
	{
		vector<Interval> intervals{ { 5, 5 }, { 1, 6 } };
		vector<Interval> res = s.merge(intervals);
		for (const auto& ivl : res) {
			cout << ivl.start << ", " << ivl.end << endl;
		}
		cout << endl;
	}
	{
		vector<Interval> intervals{ { 1, 3 }, { 2, 6 }, { 8, 10 }, { 15, 18 } };
		vector<Interval> res = s.merge(intervals);
		for (const auto& ivl : res) {
			cout << ivl.start << ", " << ivl.end << endl;
		}
		cout << endl;
	}
	{
		vector<Interval> intervals{ { 1, 4 }, { 0, 4 } };
		vector<Interval> res = s.merge(intervals);
		for (const auto& ivl : res) {
			cout << ivl.start << ", " << ivl.end << endl;
		}
		cout << endl;
	}

	std::cin.get();
	return 0;
}




