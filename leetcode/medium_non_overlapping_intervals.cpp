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

class Solution {
public:
	int eraseOverlapIntervals(vector<Interval>& intervals) {
		sort(intervals.begin(), intervals.end(), [](Interval& i1, Interval& i2) { return i1.start < i2.start; });
		int res = 0, e = INT_MIN;
		for (auto& itv : intervals) {
			if (itv.start < e) {
				res++;
				if (itv.end < e) e = itv.end;
			}
			else e = itv.end;
		}
		return res;
	}
};

int main() {

	cin.get();
	return EXIT_SUCCESS;
}

