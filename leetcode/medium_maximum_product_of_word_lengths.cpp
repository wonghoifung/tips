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
#include <initializer_list>
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
		start_ = time(nullptr);
	}
	~mytimer()
	{
		time_t end = time(nullptr);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	int maxProduct(vector<string>& words) {
		vector<int> v;
		for (size_t i = 0; i < words.size(); ++i)
		{
			int m = 0;
			for (size_t j = 0; j < words[i].size(); j++)
			{
				m |= (1 << (words[i].at(j) - 'a'));
			}
			v.push_back(m);
		}

		int maxp = 0;
		for (size_t i = 0; i < words.size(); ++i)
		{
			for (size_t j = 0; j < words.size(); ++j)
			{
				if ((v[i] & v[j]) == 0)
				{
					int p = words[i].size() * words[j].size();
					if (p > maxp)
					{
						maxp = p;
					}
				}
			}
		}
		return maxp;
	}
};

int main()
{
	Solution s;
	{
		vector<string> words{"abcw", "baz", "foo", "bar", "xtfn", "abcdef"};
		cout << s.maxProduct(words) << endl;
	}

	std::cin.get();
	return 0;
}

