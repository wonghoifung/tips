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
	int kthSmallest(vector<vector<int>>& matrix, int k) {
		int rowcnt = matrix.size();
		int colcnt = matrix[0].size();
		vector<int> curidx(rowcnt, 0);
		int res;
		for (int i = 0; i < k; ++i) {
			int rr = -1;
			int rres = INT_MAX;
			for (int r = 0; r < rowcnt; ++r) {
				if (curidx[r] < colcnt && matrix[r][curidx[r]] < rres) {
					rres = matrix[r][curidx[r]];
					rr = r;
				}
			}
			if (rr != -1) {
				curidx[rr] += 1;
				res = rres;
			}
		}
		return res;
	}
};

int main()
{
	{
		vector<vector<int>> matrix = {
			{ 1, 5, 9 },
			{ 10, 11, 13 },
			{ 12, 13, 15 }
		};
		int k = 8;
		cout << Solution().kthSmallest(matrix, k) << endl;
	}

	std::cin.get();
	return 0;
}




