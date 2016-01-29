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
	void doPlus(char a, char b, int& carry, string& res) {
		if (a == '1' && b == '1') {
			if (carry) res = "1" + res;
			else res = "0" + res;
			carry = 1;
		}
		else if (a == '0' && b == '0') {
			if (carry) res = "1" + res;
			else res = "0" + res;
			carry = 0;
		}
		else {
			if (carry) res = "0" + res, carry = 1;
			else res = "1" + res, carry = 0;
		}
	}
	string addBinary(string a, string b) {
		string res;
		int carry = 0;
		int aidx = a.size() - 1;
		int bidx = b.size() - 1;
		for (; aidx >= 0 && bidx >= 0; --aidx, --bidx) {
			doPlus(a[aidx], b[bidx], carry, res);
		}
		if (a.size() != b.size()) {
			int idx;
			string& bigger = a.size() > b.size() ? (idx = aidx, a) : (idx = bidx, b);
			for (int i = idx; i >= 0; --i) {
				doPlus(bigger[i], '0', carry, res);
			}
		}
		if (carry) res = '1' + res;
		return res;
	}
};

int main()
{
	Solution s;
	{
		cout << s.addBinary("1010", "1011") << endl; // 10101
	}

	std::cin.get();
	return 0;
}




