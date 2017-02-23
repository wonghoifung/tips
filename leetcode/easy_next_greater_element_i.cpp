#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	vector<int> nextGreaterElement(vector<int>& findNums, vector<int>& nums) {
		vector<int> res;
		for (auto fi : findNums) {
			bool found = false;
			bool startFind = false;
			for (auto i : nums) {
				if (i == fi) startFind = true;
				if (startFind && i > fi) {
					res.push_back(i);
					found = true;
					break;
				}
			}
			if (!found) res.push_back(-1);
		}
		return res;
	}
};

int main() {
	{
		vector<int> findNums{ 4, 1, 2 }, nums{ 1, 3, 4, 2 };
		auto res = Solution().nextGreaterElement(findNums, nums); // -1, 3, -1
		for (auto i : res) cout << i << " ";
		cout << endl;
	}
	{
		vector<int> findNums{ 2, 4 }, nums{ 1, 2, 3, 4 };
		auto res = Solution().nextGreaterElement(findNums, nums); // 3, -1
		for (auto i : res) cout << i << " ";
		cout << endl;
	}

	cin.get();
}

