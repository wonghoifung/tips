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

#define snprintf _snprintf_s

class Solution {
public:
	vector<string> readBinaryWatch(int num) {
		const size_t N = 10;
		vector<int> selectors(N, 0);
		for (int i = 0; i < num; ++i) selectors[i] = 1;
		vector<int> values = { 8, 4, 2, 1, 32, 16, 8, 4, 2, 1 };

		vector<string> res;
		do {
			int hour = 0;
			int minute = 0;
			for (size_t i = 0; i < N; ++i) {
				if (selectors[i]) {
					if (i >= 0 && i <= 3) hour += values[i];
					else minute += values[i];
				}
			}
			if (hour >= 0 && hour <= 11 && minute >= 0 && minute <= 59) {
				char buf[16] = { 0 };
				snprintf(buf, 16, "%d:%02d", hour, minute);
				res.push_back(buf);
			}
		} while (prev_permutation(selectors.begin(), selectors.end()));
		return res;
	}
};

int main()
{
	{
		vector<string> res = Solution().readBinaryWatch(1);
		for (auto& s : res) cout << s << " ";
		cout << endl;
	}
	{
		vector<string> res = Solution().readBinaryWatch(2);
		for (auto& s : res) cout << s << " ";
		cout << endl;
	}

	std::cin.get();
	return 0;
}



