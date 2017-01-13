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
	int fourSumCount(vector<int>& A, vector<int>& B, vector<int>& C, vector<int>& D) {
		unordered_map<int, int> m;
		for (int i = 0; i < C.size(); ++i) {
			for (int j = 0; j < D.size(); ++j) {
				int sum = C[i] + D[j];
				m[sum] += 1;
			}
		}
		int res = 0;
		for (int i = 0; i < A.size(); ++i) {
			for (int j = 0; j < B.size(); ++j) {
				res += m[-1 * (A[i] + B[j])];
			}
		}
		return res;
	}
};

int main() {
	{
		vector<int> A{ 1, 2 }, B{ -2, -1 }, C{ -1, 2 }, D{ 0, 2 };
		cout << Solution().fourSumCount(A, B, C, D) << endl; // 2
	}

	cin.get();
	return EXIT_SUCCESS;
}

