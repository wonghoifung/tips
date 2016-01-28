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
	int titleToNumber(string s) {
		int res = 0;
		int mul = 1;
		for (int i = s.size() - 1; i >= 0; --i)
		{
			int n = s.at(i) - 'A' + 1;
			res += n * mul;
			mul *= 26;
		}
		return res;
	}
};

int main()
{
	Solution s;
	{

	}

	std::cin.get();
	return 0;
}




