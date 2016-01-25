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
	bool isUgly(int num) { // 1,2,3,5
		if (num <= 0) return false;
		int ds[] = { 2, 3, 5 };
		for (int i : ds) {
			while (num % i == 0)
			{
				num /= i;
			}
		}
		return num == 1;
	}
};

int main()
{
	Solution s;
	{
		cout << boolalpha << s.isUgly(-51799) << endl;
	}

	std::cin.get();
	return 0;
}




