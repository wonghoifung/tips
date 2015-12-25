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
	bool isMatch(string s, string p) {
		int slen = s.size();
		int plen = p.size();
		vector<vector<char>> matrix(slen + 1, vector<char>(plen + 1, false));
		matrix[0][0] = true;
		for (int i = 1; i <= plen; ++i)
		{
			if (p.at(i - 1) == '*')
				matrix[0][i] = true;
			else
				break;
		}
		for (int i = 1; i <= slen; ++i)
		{
			for (int j = 1; j <= plen; ++j)
			{
				char c = p.at(j - 1);
				if (c == '*')
				{
					matrix[i][j] = matrix[i][j - 1] || matrix[i - 1][j];
				}
				else
				{
					matrix[i][j] = matrix[i - 1][j - 1] && (c == '?' || s.at(i - 1) == c);
				}
			}
		}
		return matrix[slen][plen];
	}
	bool isMatchSLOW(string s, string p, int f = 0, int l = 0) {
		if (s.empty() && !p.empty()) return false; 
		if (!s.empty() && p.empty()) return false; 
		if (s.empty() && p.empty()) return true; 
		size_t sidx = 0;
		for (size_t i = 0; i < p.size(); ++i)
		{
			if (sidx >= s.size()) return false; 

			switch (p.at(i))
			{
			case '?':
			{
				sidx += 1;
				break;
			}
			case '*':
			{
				while (i < p.size() && p.at(i) == '*') i += 1;
				if (i >= p.size()) return true;
				while (sidx < s.size() && s.at(sidx) != p.at(i)) sidx += 1;
				if (sidx >= s.size()) return false;
				bool next = isMatchSLOW(s.substr(sidx + 1), p.substr(i + 1), 1, l + 1);
				while (!next)
				{
					sidx += 1;
					while (sidx < s.size() && s.at(sidx) != p.at(i)) sidx += 1;
					if (sidx >= s.size()) return false;
					next = isMatchSLOW(s.substr(sidx + 1), p.substr(i + 1), 2, l + 1);
				}
				return true;
			}
			default:
			{
				if (sidx >= s.size()) return false;
				if (s.at(sidx) != p.at(i)) return false;
				sidx += 1;
				break;
			}
				
			}
		}
		return sidx == s.size();
	}
};

int main()
{
	Solution solu;
	{
		cout << boolalpha << solu.isMatch("aa", "a") << endl; // false
		cout << boolalpha << solu.isMatch("aa", "aa") << endl; // true
		cout << boolalpha << solu.isMatch("aaa", "aa") << endl; // false
		cout << boolalpha << solu.isMatch("aa", "*") << endl; // true
		cout << boolalpha << solu.isMatch("aa", "a*") << endl; // true
		cout << boolalpha << solu.isMatch("ab", "?*") << endl; //true
		cout << boolalpha << solu.isMatch("aab", "c*a*b") << endl; // false
		cout << boolalpha << solu.isMatch("abbbbbbbbbbzccccczkkkkz", "a*****kz") << endl;

		string s = "abbabaaabbabbaababbabbbbbabbbabbbabaaaaababababbbabababaabbababaabbbbbbaaaabababbbaabbbbaabbbbababababbaabbaababaabbbababababbbbaaabbbbbabaaaabbababbbbaababaabbababbbbbababbbabaaaaaaaabbbbbaabaaababaaaabb";
		string p = "**aa*****ba*a*bb**aa*ab****a*aaaaaa***a*aaaa**bbabb*b*b**aaaaaaaaa*a********ba*bbb***a*ba*bb*bb**a*b*bb";
		cout << boolalpha << solu.isMatch(s, p) << endl;
	}

	std::cin.get();
	return 0;
}

