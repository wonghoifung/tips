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
#include <functional>
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

class Solution {
public:
	unordered_set<string> fails;
	bool isInterleave(string s1, string s2, string s3) {
		if (s1.size() + s2.size() != s3.size()) return false;
		if (s1.empty() && s2.empty() && s3.empty()) return true;
		if (fails.find(s1 + "#" + s2) != fails.end()) return false;
		if (s3.size())
		{
			if (s1.size() && s1[0] == s3[0])
			{
				if (isInterleave(s1.substr(1), s2, s3.substr(1)))
				{
					return true;
				}
			}
			if (s2.size() && s2[0] == s3[0])
			{
				if (isInterleave(s1, s2.substr(1), s3.substr(1)))
				{
					return true;
				}
			}
		}
		fails.insert(s1 + "#" + s2);
		return false;
	}
};

int main()
{
	Solution s;
	{
		cout << boolalpha << s.isInterleave("a", "", "a") << endl;
	}

	std::cin.get();
	return 0;
}




