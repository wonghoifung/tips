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
	int hammingWeight(uint32_t n) {
		int c = 0;
		for (size_t i = 0; i < sizeof(uint32_t) * 8; ++i)
		{
			if (n & (1 << i))
			{
				c += 1;
			}
		}
		return c;
	}
};

int main()
{
	Solution s;
	{
		
	}
	std::cin.get();

	return 0;
}

