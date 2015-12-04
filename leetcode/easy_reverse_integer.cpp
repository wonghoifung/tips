#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <deque>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(NULL);
	}
	~mytimer()
	{
		time_t end = time(NULL);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	int reverse(int x) {
		if (x == INT_MIN)
		{
			return 0;
		}
		bool neg = x < 0 ? true : false;
		x = abs(x);
		long long ret = x % 10;
		x /= 10;
		while (x > 0)
		{
			ret *= 10;
			ret += x % 10;
			x /= 10;
			if (ret > INT_MAX)
			{
				return 0;
			}
		}
		return neg ? -ret : ret;
	}
};

int main()
{
	Solution s;
	{
		int x = 123;
		cout << s.reverse(x) << endl;
	}
	{
		int x = -123;
		cout << s.reverse(x) << endl;
	}
	{
		int x = 1534236469;
		cout << s.reverse(x) << endl;
	}
	{
		int x = 2147483648; 
		x *= -1;
		cout << "x: " << x << endl;
		cout << s.reverse(x) << endl;
	}
	{
		int x = 1563847412;
		cout << "x: " << x << endl;
		cout << s.reverse(x) << endl;
	}

	std::cin.get();

	return 0;
}

