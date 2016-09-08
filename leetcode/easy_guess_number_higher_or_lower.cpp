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

int picked = 2;
int guess(int num) {
	if (num == picked) return 0;
	if (num > picked) return -1;
	return 1;
}

class Solution {
public:
	int guessNumber(int n) {
		int beg = 1;
		int end = n;
		while (true) {
			if (end - beg <= 3) {
				for (int i = beg; i <= end; ++i) {
					if (guess(i) == 0) return i;
				}
			}
			int mid = (end - beg) / 2 + beg;
			int res = guess(mid);
			if (res == 0) return mid;
			else if (res > 0) beg = mid - 1;
			else end = mid + 1;
		}
	}
};

int main()
{
	cout << Solution().guessNumber(2) << endl;

	std::cin.get();
	return 0;
}




