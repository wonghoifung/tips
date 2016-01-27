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
	string d2s(int d, int di) {
		/* Input is guaranteed to be within the range from 1 to 3999 */
		static unordered_map<int, char> m{
			{ 1, 'I' },
			{ 5, 'V' },

			{ 10, 'X' },
			{ 50, 'L' },

			{ 100, 'C' },
			{ 500, 'D' },

			{ 1000, 'M' }
		};
		string res;
		if (d <= 3 && d >= 1) {
			for (int i = 0; i < d; ++i) {
				res += m[di];
			}
			return res;
		}
		else if (d == 4) {
			int diff = 5 - d;
			for (int i = 0; i < diff; ++i) {
				res += m[di];
			}
			res += m[5 * di];
			return res;
		}
		else if (d == 5) {
			res += m[5 * di];
			return res;
		}
		else if (d > 5 && d < 9) {
			int diff = d - 5;
			res += m[5 * di];
			for (int i = 0; i < diff; ++i) {
				res += m[di];
			}
			return res;
		}
		else if (d == 9) {
			res += m[di];
			res += m[di * 10];
			return res;
		}
		else { // 0
			return "";
		}
	}
	string intToRoman(int num) { 
		string res;
		int di = 1;
		while (num > 0)
		{
			int d = num % 10;
			num /= 10;
			res = d2s(d, di) + res;
			di *= 10;
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		cout << s.intToRoman(9) << endl;
	}

	std::cin.get();
	return 0;
}




