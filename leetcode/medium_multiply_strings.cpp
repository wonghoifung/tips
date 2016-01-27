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
	pair<int, char> charMultiply(char c1, char c2, int carry) {
		int mul = (c1 - '0') * (c2 - '0') + carry;
		return { mul / 10, mul % 10 + '0' };
	}
	pair<int, char> charPlus(char c1, char c2, int carry) {
		int plu = (c1 - '0') + (c2 - '0') + carry;
		return { plu / 10, plu % 10 + '0' };
	}
	string multiply(string num1, string num2) {
		string res;
		reverse(num1.begin(), num1.end());
		reverse(num2.begin(), num2.end());
		for (int i = 0; i < num2.size(); ++i)
		{
			string round;
			int carry = 0;
			for (int j = 0; j < num1.size(); ++j)
			{
				auto p = charMultiply(num2.at(i), num1.at(j), carry);
				carry = p.first;
				round += p.second;
			}
			if (carry > 0) round += (carry + '0');
			if (res.empty()) res = round;
			else {
				string tmp(res.begin(), res.begin() + i);
				int cur = i;
				int car = 0;
				for (int k = 0; k < round.size(); ++k)
				{
					if (cur < res.size()) {
						auto p = charPlus(round.at(k), res.at(cur), car);
						car = p.first;
						tmp += p.second;
						cur += 1;
					}
					else {
						auto p = charPlus(round.at(k), '0', car);
						car = p.first;
						tmp += p.second;
					}
				}
				for (; cur < res.size(); ++cur) {
					auto p = charPlus(res.at(cur), '0', car);
					car = p.first;
					tmp += p.second;
				}
				if (car > 0) tmp += (car + '0');
				res = tmp;
			}
		}
		reverse(res.begin(), res.end());
		auto idx = res.find_first_not_of('0');
		return idx != string::npos ? res.substr(idx) : "0";
	}
};

int main()
{
	Solution s;
	{
		cout << s.multiply("98", "9") << endl;
	}

	std::cin.get();
	return 0;
}




