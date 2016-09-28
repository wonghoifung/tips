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
public:
#if 0
	int lastRemaining(int n) {
		if (n == 1) return 1;
		if (n <= 5) return 2;
		if (n % 2 == 1) n -= 1;
		list<int> l;
		for (int i = 1; i <= n; ++i) l.push_back(i);
		bool leftToRight = true;
		auto it = l.begin();
		while (l.size() > 1) {
			if (leftToRight) {
				if (it != l.end()) {
					auto itdel = it;
					auto itlast = l.end(); --itlast;
					if (it == itlast) {
						advance(it, -1);
						leftToRight = false;
					}
					else {
						advance(it, 2);
					}
					l.erase(itdel);
				}
				else {
					--it;
					leftToRight = false;
				}
			}
			else {
				if (it != l.begin()) {
					auto itdel = it;
					auto itsecond = l.begin(); ++itsecond;
					if (it == itsecond) {
						advance(it, -1);
						leftToRight = true;
					}
					else {
						advance(it, -2);
					}
					l.erase(itdel);
				}
				else {
					leftToRight = true;
					auto itdel = it;
					++it;
					l.erase(itdel);
				}
			}
		}
		return l.back();
	}
#endif
	int lastRemaining(int n) {
		bool left = true;
		int remaining = n;
		int step = 1;
		int head = 1;
		while (remaining > 1) {
			if (left || remaining % 2 == 1) {
				head += step;
			}
			remaining /= 2;
			step *= 2;
			left = !left;
		}
		return head;
	}
};

int main()
{
	cout << Solution().lastRemaining(8122) << endl;
	
	std::cin.get();
	return 0;
}



