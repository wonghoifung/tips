#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Solution {
public:
	int numberOfArithmeticSlices(vector<int>& A) {
		if (A.empty()) return 0;
		/* [I, [D, C]], C is number of arithmetic subsequences with difference D that ends at A[I], including 2 element sequence */
		vector<unordered_map<int, int>> dp(A.size());
		int res = 0;
		for (int i = 0; i < A.size(); ++i) { // ends at A[i]
			for (int j = 0; j < i; ++j) {
				/* if the result is bigger than the max value of integer or smaller than the min value of integer, */ 
				/* there is no way to have a valid a 3rd integer element to form a arithmetic sequence. */
				if ((long)A[i] - (long)A[j] > INT_MAX || (long)A[i] - (long)A[j] < INT_MIN) continue;
				int diff = A[i] - A[j];
				dp[i][diff] += 1;
				if (dp[j].find(diff) != dp[j].end()) {
					dp[i][diff] += dp[j][diff];
					res += dp[j][diff];
				}
			}
		}
		return res;
	}
};

int main() {
	vector<int> A{ 2, 4, 6, 8, 10 };
	cout << Solution().numberOfArithmeticSlices(A) << endl; // 7

	cin.get();
	return EXIT_SUCCESS;
}

