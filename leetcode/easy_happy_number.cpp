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
	bool isHappyHelper(int n, unordered_set<int>& tracks) {
		if (n != 1 && tracks.find(n) != tracks.end())
		{
			return false;
		}
		int on = n;
		int ret = 0;
		while (n)
		{
			int tmp = n % 10;
			ret += tmp * tmp;
			n /= 10;
		}
		if (ret == 1)
		{
			return true;
		}
		tracks.insert(on);
		return isHappyHelper(ret, tracks);
	}
	bool isHappy(int n) {
		unordered_set<int> tracks;
		return isHappyHelper(n, tracks);
	}
};

int main()
{
	Solution s;
	{
		mytimer t;

	}
	std::cin.get();

	return 0;
}

