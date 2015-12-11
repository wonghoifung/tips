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
#include <memory>
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
	int numTrees(int n) {
		if (n == 0) return 0;
		vector<int> R; R.resize(n + 1, 0);
		R[0] = 0;
		R[1] = 1;
		for (int i = 2; i <= n; i++)
		{
			R[i] += 2 * R[i - 1];
			for (int j = 1; j < i - 1; j++)
			{
				R[i] += (R[j] * R[i - 1 - j]);
			}
		}
		return R[n];
	}
};

int main()
{
	Solution s;
	{
		mytimer tt;
		cout << s.numTrees(3) << endl;
	}

	std::cin.get();
	return 0;
}

