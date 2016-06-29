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
	/*
 * 	int kadane(const vector<int>& arr, int& start, int& end) {
 * 			if (arr.empty()) return 0;
 * 					vector<int> dp(arr.size(), 0);
 * 							vector<int> sidx(arr.size(), 0);
 * 									dp[0] = arr[0];
 * 											sidx[0] = 0;
 * 													auto m = dp[0];
 * 															for (auto i = 1; i < arr.size(); ++i) {
 * 																		if (arr[i] + dp[i - 1] > arr[i]) {
 * 																						dp[i] = arr[i] + dp[i - 1];
 * 																										sidx[i] = sidx[i - 1];
 * 																													}
 * 																																else {
 * 																																				dp[i] = arr[i];
 * 																																								sidx[i] = i;
 * 																																											}
 * 																																														if (dp[i] > m) {
 * 																																																		m = dp[i];
 * 																																																						start = sidx[i];
 * 																																																										end = i;
 * 																																																													}
 * 																																																															}
 * 																																																																	return m;
 * 																																																																		}
 * 																																																																			*/
	int maxSubArrayNoBiggerThanK(const vector<int>& arr, int& start, int& end, int k) {
		if (arr.empty()) return 0;
		int gm = INT_MIN;
		for (auto i = 0; i < arr.size(); ++i) {
			auto isum = 0;
			for (auto j = i; j < arr.size(); ++j) {
				isum += arr[j];
				if (isum == k) {
					start = i;
					end = j;
					return k;
				}
				if (isum > k) continue;
				if (isum < k && isum > gm) {
					gm = isum;
					start = i;
					end = j;
				}
			}
		}
		return gm;
	}
public:
	int maxSumSubmatrix(vector<vector<int>>& matrix, int k) { 
		if (matrix.empty()) return 0;
		if (matrix[0].empty()) return 0;
		vector<int> dp(matrix.size(), 0);
		int L = 0;
		int R = 0;
		int cm = 0;
		int gm = INT_MIN;
		int left = 0;
		int right = 0;
		int up = 0;
		int bottom = 0;
		int start = 0, end = 0;
		for (L = 0; L < matrix[0].size(); ++L) {
			for (R = L; R < matrix[0].size(); ++R) {
				if (R == L) {
					for (auto i = 0; i < matrix.size(); ++i) {
						dp[i] = matrix[i][L];
					}
				}
				else {
					for (auto i = 0; i < matrix.size(); ++i) {
						dp[i] += matrix[i][R];
					}
				}
				/*cm = kadane(dp, start, end);*/ 
				cm = maxSubArrayNoBiggerThanK(dp, start, end, k);
				if (cm == k) return cm;
				/*printf("left:%d right:%d up:%d bottom:%d cm:%d\n", L, R, start, end, cm);*/
				if (cm > gm && cm <= k) {
					gm = cm;
					left = L;
					right = R;
					up = start;
					bottom = end;
				}
			}
		}
		return gm;
	}
};

int main()
{
	{
		vector<vector<int>> matrix = {
			{ 1, 0, 1 },
			{ 0, -2, 3 }
		};
		int k = 2;
		cout << Solution().maxSumSubmatrix(matrix, k) << endl; // 2
	}

	{
		vector<vector<int>> matrix = { 
			{ 2, 2, -1 } 
		};
		int k = 0;
		cout << Solution().maxSumSubmatrix(matrix, k) << endl; // -1
	}

	{
		vector<vector<int>> matrix = { 
			{ 5, -4, -3, 4 }, 
			{ -3, -4, 4, 5 }, 
			{ 5, 1, 5, -4 } 
		};
		int k = 10;
		cout << Solution().maxSumSubmatrix(matrix, k) << endl; // 10
	}

	{
		vector<vector<int>> matrix = { 
			{ 5, -4, -3, 4 }, 
			{ -3, -4, 4, 5 }, 
			{ 5, 1, 5, -4 } 
		};
		int k = 8;
		cout << Solution().maxSumSubmatrix(matrix, k) << endl; // 8
	}

	std::cin.get();
	return 0;
}




