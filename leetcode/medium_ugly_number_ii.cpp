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

class Solution {
public:
	int nthUglyNumber(int n) {
		vector<int> seq(1, 1);
		int f2 = 0, f3 = 0, f5 = 0;
		for (int i = 1; i < n; ++i)
		{
			int next = min(seq[f2] * 2, min(seq[f3] * 3, seq[f5] * 5));
			if (next == seq[f2] * 2) f2 += 1;
			if (next == seq[f3] * 3) f3 += 1;
			if (next == seq[f5] * 5) f5 += 1;
			seq.push_back(next);
		}
		return seq.back();
	}
};

int main()
{
	Solution solu;
	{
		cout << solu.nthUglyNumber(10) << endl;
	}
	std::cin.get();
	return 0;
}




