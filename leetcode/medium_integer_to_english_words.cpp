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
/*
 * 123 -> "One Hundred Twenty Three"
 * 12345 -> "Twelve Thousand Three Hundred Forty Five"
 * 1234567 -> "One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven"
 * */
class Solution {
public:
	string part2str(int n) {
		static const string nums1[] = { "", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine",
			"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" };

		static const string nums2[] = { "", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };

		if (n < 20) return nums1[n];
		
		string res;
		int n2 = n % 100;
		if (n2 < 20) {
			res = nums1[n2];
		}
		else {
			int tendigit = n2 / 10;
			res = nums2[tendigit];
			string digit = nums1[n2 % 10];
			res = digit.empty() ? res : res + " " + digit;
		}
		int n3 = n / 100;
		if (n3 > 0) res = res.empty() ? nums1[n3] + " Hundred" : nums1[n3] + " Hundred " + res;
		return res;
	}
	string numberToWords(int num) { // Given input is guaranteed to be less than 2^31 - 1.( 2 147 483 648 )
		if (num == 0) return "Zero";

		static const string div[] = { "", "Thousand", "Million", "Billion" };
		
		int dividx = 0;
		string res;
		while (num > 0)
		{
			int n = num % 1000;
			num /= 1000;
			if (n)
			{
				if (dividx == 0) res = part2str(n);
				else res = res.empty() ? part2str(n) + " " + div[dividx] : part2str(n) + " " + div[dividx] + " " + res;
			}
			dividx += 1;
		}
		return res;
	}
};

int main()
{
	Solution s;
	{
		cout << s.numberToWords(123) << endl;
		cout << s.numberToWords(12345) << endl;
		cout << s.numberToWords(1234567) << endl;
		cout << s.numberToWords(1000000) << endl;
	}

	std::cin.get();
	return 0;
}




