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
	bool isIsomorphic(string s, string t) {
		unordered_map<char, char> m;
		unordered_map<char, char> rm;
		for (size_t i = 0; i < s.size(); ++i)
		{
			auto it = m.find(s.at(i));
			if (it == m.end())
			{
				auto rit = rm.find(t.at(i));
				if (rit == rm.end())
				{
					m[s.at(i)] = t.at(i);
					rm[t.at(i)] = s.at(i);
					continue;
				}
				return false;
			}
			if (it->second != t.at(i))
			{
				return false;
			}
		}
		return true;
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

