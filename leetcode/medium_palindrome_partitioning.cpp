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
#include <array>
#include <memory>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <tuple>
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
	bool isPalindrome(const string& s) {
		if (s.empty()) return false;
		for (int i = 0; i < s.size() / 2; ++i)
		{
			if (s.at(i) != s.at(s.size() - i - 1))
			{
				return false;
			}
		}
		return true;
	}
	vector<vector<string>> partition(string s) {
		vector<vector<string>> res;

		for (size_t len = 1; len <= s.size(); ++len)
		{
			string first = s.substr(0, len);
			if (!isPalindrome(first)) continue;
			string left = s.substr(len);
			if (left.empty())
			{
				vector<string> v;
				v.push_back(first);
				res.push_back(v);
				continue;
			}
			vector<vector<string>> r = partition(left);
			for (auto& v : r)
			{
				v.insert(v.begin(), first);
				res.push_back(v);
			}
		}
		return res;
	}
};

int main()
{
	Solution solu;
	{
	
	}

	std::cin.get();
	return 0;
}

