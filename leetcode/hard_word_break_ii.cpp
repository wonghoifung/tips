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
	void dfs(vector<vector<int>>& pos, const string& s, int i, const string& pattern, vector<string>& res) {
		if (i < 0)
		{
			res.push_back(pattern);
			return;
		}
		for (int item : pos[i])
		{
			string w = s.substr(item, i + 1 - item);
			string next = pattern.size() == 0 ? w : w + " " + pattern;
			dfs(pos, s, item - 1, next, res);
		}
	}
	vector<string> wordBreak(string s, unordered_set<string>& wordDict) {
		vector<string> res;
		vector<vector<int>> pos(s.size());
		for (size_t i = 0; i < s.size(); ++i)
		{
			for (size_t j = 0; j <= i; ++j)
			{
				if (wordDict.find(s.substr(j, i + 1 - j)) != wordDict.end() && (j == 0 || pos[j - 1].size() > 0))
				{
					pos[i].push_back(j);
				}
			}
		}
		dfs(pos, s, s.size() - 1, "", res);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		mytimer t;
		string s = "catsanddog";
		unordered_set<string> wordDict{ "cat", "cats", "and", "sand", "dog" };
		vector<string> res = solu.wordBreak(s, wordDict);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n"));
		/* ["cats and dog", "cat sand dog"] */
	}
	{
		mytimer t;
		string s = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
		unordered_set<string> wordDict{ "a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa", "aaaaaaa", "aaaaaaaa", "aaaaaaaaa", "aaaaaaaaaa" };
		vector<string> res = solu.wordBreak(s, wordDict);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n"));
	}

	std::cin.get();
	return 0;
}

