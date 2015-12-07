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
	int hIndex(vector<int>& citations) {
		sort(citations.begin(), citations.end());
		for (size_t i = 0; i < citations.size(); ++i)
		{
			if (citations[i] >= citations.size() - i)
			{
				return citations.size() - i;
			}
		}
	}
};

int main()
{
	Solution s;
	{
		vector<int> citations{ 3, 0, 6, 1, 5 };
		cout << s.hIndex(citations) << endl;
	}

	std::cin.get();

	return 0;
}

