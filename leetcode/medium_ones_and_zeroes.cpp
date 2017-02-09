#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
public:
	int findMaxForm(vector<string>& strs, int m, int n) {
		vector<vector<int>> memo(m + 1, vector<int>(n + 1, 0));
		int numZeroes, numOnes;
		for (auto& s : strs) {
			numZeroes = numOnes = 0;
			for (auto c : s) {
				if (c == '0') numZeroes += 1;
				else if (c == '1') numOnes += 1;
			}
			for (int i = m; i >= numZeroes; --i) {
				for (int j = n; j >= numOnes; --j) {
					memo[i][j] = max(memo[i][j], memo[i - numZeroes][j - numOnes] + 1); // skip s or form s
				}
			}
		}
		return memo[m][n];
	}
};

int main() {
	{
		vector<string> strs{ "10", "0001", "111001", "1", "0" };
		cout << Solution().findMaxForm(strs, 5, 3) << endl; // 4
	}
	{
		vector<string> strs{ "10", "0", "1" };
		cout << Solution().findMaxForm(strs, 1, 1) << endl; // 2
	}

	cin.get();
}

