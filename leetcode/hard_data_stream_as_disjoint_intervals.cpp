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

using namespace std;

struct Interval {
	int start;
	int end;
	Interval() : start(0), end(0) {}
	Interval(int s, int e) : start(s), end(e) {}
};

class SummaryRanges {
	set<int> s;
public:
	SummaryRanges() {

	}

	void addNum(int val) {
		s.insert(val);
	}

	vector<Interval> getIntervals() {
		vector<Interval> result;
		int last = -1;
		Interval inval; 
		for (auto it = s.begin(); it != s.end(); ++it)
		{
			if (last == -1) {
				inval.start = inval.end = *it;
				last = *it;
			}
			else {
				if (last + 1 == *it) {
					inval.end = *it;
					last = *it;
				}
				else {
					result.push_back(inval);
					inval.start = inval.end = *it;
					last = *it;
				}
			}
		}
		result.push_back(inval);
		return result;
	}
};

int main()
{
	SummaryRanges obj;
	int integers[] = { 1, 3, 7, 2, 6 };
	for (auto i : integers)
	{
		obj.addNum(i);
	}
	vector<Interval> param_2 = obj.getIntervals();
	for (auto& inval : param_2)
	{
		cout << "[" << inval.start << ", " << inval.end << "]" << endl;
	}

	std::cin.get();
	return 0;
}




