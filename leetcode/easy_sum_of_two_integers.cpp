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
	int getBit(int val, int pos) {
		return (val >> pos) & 1;
	}
	int setBit(int val, int pos, int bitval) {
		return val | (bitval << pos);
	}
public:
	int getSum(int a, int b) {
		int res = 0;
		auto carry = 0;
		auto bitcnt = sizeof(int) * 8;
		for (auto pos = 0; pos < bitcnt; ++pos) {
			auto bita = getBit(a, pos);
			auto bitb = getBit(b, pos);
			if (bita == 0 && bitb == 0) {
				if (carry) {
					res = setBit(res, pos, carry);
					carry = 0;
				}
			}
			else if (bita == 1 && bitb == 1) {
				if (carry == 0) {
					carry = 1;
				}
				else {
					res = setBit(res, pos, carry);
				}
			}
			else {
				if (carry == 0) {
					res = setBit(res, pos, 1);
				}
			}
		}
		return res;
	}
};

int main()
{
	cout << Solution().getSum(1, 2) << endl;
	cout << Solution().getSum(-1, 2) << endl;
	cout << Solution().getSum(-1, -2) << endl;

	std::cin.get();
	return 0;
}




