#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Solution {
public:
	int thirdMax(const vector<int>& nums) { // the time complexity must be in O(n)
		set<int, greater<int>> q;
		for (auto i : nums) q.insert(i);
		auto it = q.begin();
		if (q.size() >= 3)
			advance(it, 2);
		return *it;
	}
};

int main() {
	cout << Solution().thirdMax({ 3, 2, 1 }) << endl; // 1
	cout << Solution().thirdMax({ 1, 2 }) << endl; // 2
	cout << Solution().thirdMax({ 2, 2, 3, 1 }) << endl; // 1, the third maximum distinct number

	cin.get();
	return EXIT_SUCCESS;
}

