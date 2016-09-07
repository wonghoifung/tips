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
	struct pairless {
		bool operator()(const pair<int, int>& p1, const pair<int, int>& p2) {
			return p1.first + p1.second < p2.first + p2.second;
		}
	};
public:
	vector<pair<int, int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
		multiset<pair<int, int>, pairless> s;
		for (int i = 0; i < nums1.size(); ++i) {
			for (int j = 0; j < nums2.size(); ++j) {
				s.insert(make_pair(nums1[i], nums2[j]));
			}
		}
		vector<pair<int, int>> res;
		int c = 0;
		for (auto it = s.begin(); it != s.end(); ++it) {
			if (c == k) break;
			res.push_back(*it);
			c += 1;
		}
		return res;
	}
};

int main()
{
	{
		vector<int> nums1{ 1, 7, 11 };
		vector<int> nums2{ 2, 4, 6 };
		int k = 3;
		auto res = Solution().kSmallestPairs(nums1, nums2, k);
		for (auto& p : res) {
			cout << p.first << ", " << p.second << endl;
		}//Return: [1,2],[1,4],[1,6]
		cout << endl;
	}

	{
		vector<int> nums1{ 1, 1, 2 };
		vector<int> nums2{ 1, 2, 3 };
		int k = 2;
		auto res = Solution().kSmallestPairs(nums1, nums2, k);
		for (auto& p : res) {
			cout << p.first << ", " << p.second << endl;
		}//Return: [1,1],[1,1]
		cout << endl;
	}

	{
		vector<int> nums1{ 1, 2 };
		vector<int> nums2{ 3 };
		int k = 3;
		auto res = Solution().kSmallestPairs(nums1, nums2, k);
		for (auto& p : res) {
			cout << p.first << ", " << p.second << endl;
		}//Return: [1,3],[2,3]
		cout << endl;
	}

	std::cin.get();
	return 0;
}




