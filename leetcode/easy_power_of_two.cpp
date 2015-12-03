#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
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
	bool isPowerOfTwo(int n) {
		if (n == 0)
		{
			return false;
		}
		while (n != 1)
		{
			if (n % 2 == 0)
			{
				n /= 2;
				continue;
			}
			return false;
		}
		return true;
	}
};

int main()
{
	Solution s;

	

	std::cin.get();

	return 0;
}

