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
	void expand(const string& current, unordered_set<string>& wordList, vector<string>& expansion) {
		for (size_t i = 0; i < current.size(); ++i)
		{
			for (char c = 'a'; c <= 'z'; ++c)
			{
				if (c != current.at(i))
				{
					string s = current; s[i] = c;
					if (wordList.count(s) > 0) expansion.push_back(s);
				}
			}
		}
	}
	void bfs(unordered_map<string, vector<string>>& fatherWords, unordered_map<string, int>& distance,
		const string& beginWord, const string& endWord, unordered_set<string> &wordList) {
		queue<string> q;
		q.push(beginWord);
		distance[beginWord] = 0;
		while (q.size())
		{
			string s = q.front(); q.pop();
			vector<string> expansion;
			expand(s, wordList, expansion);
			for (const auto& son : expansion)
			{
				fatherWords[son].push_back(s);
				if (distance.find(son) == distance.end())
				{
					distance[son] = distance[s] + 1;
					q.push(son);
				}
			}
		}
	}
	void dfs(vector<vector<string>>& ladders, vector<string>& path, const string& current, const string& start, 
		unordered_map<string, int>& distance, unordered_map<string, vector<string>>& fatherWords) {
		path.push_back(current);
		if (current == start)
		{
			reverse(path.begin(), path.end());
			ladders.push_back(path);
			reverse(path.begin(), path.end());
		}
		else
		{
			vector<string>& fathers = fatherWords[current];
			for (const auto& f : fathers)
			{
				auto fit = distance.find(f);
				if (fit != distance.end() && distance[current] == fit->second + 1)
				{
					dfs(ladders, path, f, start, distance, fatherWords);
				}
			}
		}
		path.pop_back();
	}
	vector<vector<string>> findLadders(string beginWord, string endWord, unordered_set<string> &wordList) {
		vector<vector<string>> ladders;
		unordered_map<string, vector<string>> fatherWords;
		unordered_map<string, int> distance;
		wordList.insert(beginWord);
		wordList.insert(endWord);
		bfs(fatherWords, distance, beginWord, endWord, wordList);
		vector<string> path;
		dfs(ladders, path, endWord, beginWord, distance, fatherWords);
		return ladders;
	}
};

int main()
{
	Solution s;
	{
		mytimer t;
		string beginWord = "hit";
		string 	endWord = "cog";
		unordered_set<string> wordList = { "hot", "dot", "dog", "lot", "log" };
		vector<vector<string>> res = s.findLadders(beginWord, endWord, wordList);
		for (const auto& v : res)
		{
			for (const auto& s : v)
			{
				cout << s << " ";
			}
			cout << endl;
		}
	}

	std::cin.get();
	return 0;
}

