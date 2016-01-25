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
	int nthSuperUglyNumber(int n, vector<int>& primes) {
		vector<int> res(1, 1);

		vector<int> queueHeadIdxs(primes.size(), 0);

		for (int i = 1; i < n; ++i) {
			int next = INT_MAX;

			for (int j = 0; j < primes.size(); ++j) {
				next = min(next, primes[j] * res[queueHeadIdxs[j]]);
			}

			for (int j = 0; j < primes.size(); ++j) {
				if (next == primes[j] * res[queueHeadIdxs[j]]) {
					queueHeadIdxs[j] += 1;
				}
			}

			res.push_back(next);
		}

		return res.back();
	}
};

int main()
{
	Solution s;
	{
		vector<int> primes{ 2, 7, 13, 19 };
		cout << s.nthSuperUglyNumber(12, primes) << endl; // 32 [1, 2, 4, 7, 8, 13, 14, 16, 19, 26, 28, 32]
	}

	std::cin.get();
	return 0;
}




