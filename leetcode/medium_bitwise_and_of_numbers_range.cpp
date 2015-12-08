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
#include <sstream>
#include <algorithm>
#include <bitset>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

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
	int highest1BitPos(int m) {
		for (size_t i = 31; i >= 0; --i)
		{
			if ((1 << i) & m)
			{
				return i;
			}
		}
	}
	int rangeBitwiseAnd(int m, int n) {
		if (m == 0 || n == 0)
		{
			return 0;
		}
		if (m == n)
		{
			return m;
		}
		
		int mhp = highest1BitPos(m);
		int nhp = highest1BitPos(n);
		if (mhp < nhp)
		{
			return 0;
		}

		int ret = m;
		for (size_t i = m + 1; i <= n; i++)
		{
			ret &= i;
		}

		return ret;
	}
};

int main()
{
	Solution s;
	{
		mytimer t;
		cout << s.rangeBitwiseAnd(20000, 2147483647) << endl;
		cout << s.rangeBitwiseAnd(1, 1) << endl;
		cout << s.rangeBitwiseAnd(1, 10) << endl;
		cout << s.rangeBitwiseAnd(8, 10) << endl;
		cout << s.rangeBitwiseAnd(3, 3) << endl;
		cout << s.rangeBitwiseAnd(6, 7) << endl;
		cout << s.rangeBitwiseAnd(5, 6) << endl;
	}
	std::cin.get();

	return 0;
}

