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
	uint32_t reverseBits(uint32_t n) {
		uint32_t ret = 0;
		for (size_t i = 0; i < 32; ++i)
		{
			if ((1 << i) & n)
			{
				ret |= (1 << (31 - i));
			}
		}
		return ret;
	}
};

int main()
{
	Solution s;
	{
		cout << bitset<32>(65536) << endl;
		cout << bitset<32>(s.reverseBits(65536)) << endl;
	}
	std::cin.get();

	return 0;
}

