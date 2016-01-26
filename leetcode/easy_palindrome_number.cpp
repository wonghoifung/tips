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
	int get(int i, int x) {
		int d = pow(10, i);
		x /= d;
		return x %= 10;
	}
	bool isPalindrome(int x) {
		if (x < 0) return false;
		int cnt = 0;
		int tmp = x;
		while (tmp)
		{
			tmp /= 10;
			cnt += 1;
		}
		for (int i = 0; i < cnt / 2; ++i)
		{
			if (get(i, x) != get(cnt - 1 - i, x))
			{
				return false;
			}
		}
		return true;
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




