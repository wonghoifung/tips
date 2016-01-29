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
	int addDigits(int num) {
		int newd = 0;
		int cnt = 10;
		int i = 0;
		while (num > 0)
		{
			newd += (num % 10);
			num /= 10;
			if (num == 0) {
				if (newd < 10) break;
				num = newd;
				newd = 0;
			}
		}
		return newd;
	}
};

int main()
{
	Solution s;
	{
		cout << s.addDigits(19) << endl;
	}

	std::cin.get();
	return 0;
}




