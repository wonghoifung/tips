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
	bool isSelfCrossing(vector<int>& x) {
		int l = x.size();
		if (l <= 3) return false;
		for (int i = 3; i < l; ++i) {
			if (x[i] >= x[i - 2] && x[i - 1] <= x[i - 3]) {
				return true;
			} // 4 cross 1
			if (i >= 4) {
				if (x[i - 1] == x[i - 3] && x[i] + x[i - 4] >= x[i - 2]) {
					return true;
				} // 5 meet 1
			} 
			if (i >= 5) {
				if (x[i - 2] >= x[i - 4] && x[i - 1] <= x[i - 3] &&
					x[i] >= x[i - 2] - x[i - 4] && x[i - 1] >= x[i - 3] - x[i - 5]) {
					return true;
				} // 6 cross 1
			}
		}
		return false;
	}
};

int main()
{
	Solution s;
	{
		vector<int> x{ 2, 1, 1, 2 };
		cout << boolalpha << s.isSelfCrossing(x) << endl; // true
	}
	{
		vector<int> x{ 1, 2, 3, 4 };
		cout << boolalpha << s.isSelfCrossing(x) << endl; // false
	}
	{
		vector<int> x{ 1, 1, 1, 1 };
		cout << boolalpha << s.isSelfCrossing(x) << endl; // true
	}

	std::cin.get();
	return 0;
}




