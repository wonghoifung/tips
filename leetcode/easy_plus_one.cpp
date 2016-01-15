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
	vector<int> plusOne(vector<int>& digits) {
		vector<int> res;
		int carry = 1;
		for (int i = digits.size() - 1; i >= 0; --i)
		{
			int s = carry + digits[i];
			carry = s / 10;
			s %= 10;
			res.push_back(s);
		}
		if (carry) res.push_back(carry);
		reverse(res.begin(), res.end());
		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<int> digits{ 1 };
		vector<int> res = s.plusOne(digits);
		copy(res.begin(), res.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}




