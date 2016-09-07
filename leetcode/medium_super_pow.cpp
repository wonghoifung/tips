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

class Solution {// ab % k = (a%k)(b%k)%k
	const int base = 1337; 
	int powmod(int a, int k) //a^k mod 1337 where 0 <= k <= 10
	{
		a %= base;
		int result = 1;
		for (int i = 0; i < k; ++i)
			result = (result * a) % base;
		return result;
	}
public:
	int superPow(int a, vector<int>& b) {
		if (b.empty()) return 1;
		int last_digit = b.back();
		b.pop_back();
		return powmod(superPow(a, b), 10) * powmod(a, last_digit) % base;
	}//a^1234567 % k = (a^1234560 % k) * (a^7 % k) % k = (a^123456 % k)^10 % k * (a^7 % k) % k
};

int main()
{
	{
		vector<int> b{ 3 };
		cout << Solution().superPow(2, b) << endl;
	}
	
	{
		vector<int> b{ 1, 0 };
		cout << Solution().superPow(2, b) << endl;
	}

	{
		vector<int> b{ 2, 0, 0 };
		cout << Solution().superPow(2147483647, b) << endl;
	}

	std::cin.get();
	return 0;
}




