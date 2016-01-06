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
	bool isLettersSame(string s1, string s2) {
		if (s1.size() != s2.size()) return false;
		sort(s1.begin(), s1.end());
		sort(s2.begin(), s2.end());
		return s1 == s2;
	}
	bool isScramble(string s1, string s2) {
		if (s1 == s2) return true;
		if (!isLettersSame(s1, s2)) return false;
		for (int len = 1; len < s1.size(); ++len)
		{
			if (isScramble(s1.substr(0, len), s2.substr(0, len)))
			{
				if (isScramble(s1.substr(len), s2.substr(len)))
				{
					return true;
				}
			}
			
			if (isScramble(s1.substr(0, len), s2.substr(s2.size() - len)))
			{
				if (isScramble(s1.substr(len), s2.substr(0, s2.size() - len)))
				{
					return true;
				}
			}
			
		}
		return false;
	}
};

int main()
{
	Solution solu;
	{
		cout << boolalpha << solu.isScramble("great", "rgeat") << endl;
	}
	{
		cout << boolalpha << solu.isScramble("great", "rgtae") << endl;
	}
	{
		cout << boolalpha << solu.isScramble("abcd", "bdac") << endl;
	}
	{
		cout << boolalpha << solu.isScramble("a", "a") << endl;
	}
	{
		cout << boolalpha << solu.isScramble("ab", "ba") << endl;
	}
	{
		cout << boolalpha << solu.isScramble("abb", "bba") << endl;
	}
	{
		cout << boolalpha << solu.isScramble("abcdd", "dbdac") << endl;
	}
	std::cin.get();
	return 0;
}




