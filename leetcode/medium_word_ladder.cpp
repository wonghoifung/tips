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
	int ladderLength(string beginWord, string endWord, unordered_set<string>& wordList) {
		wordList.insert(endWord);
		queue<string> toVisit;
		bfs(beginWord, wordList, toVisit);
		int dist = 2;
		while (!toVisit.empty())
		{
			int num = toVisit.size();
			for (int i = 0; i < num; ++i)
			{
				string word = toVisit.front();
				toVisit.pop();
				if (word == endWord) return dist;
				bfs(word, wordList, toVisit);
			}
			dist += 1;
		}
		return 0;
	}
	void bfs(string word, unordered_set<string>& wordList, queue<string>& toVisit) {
		wordList.erase(word);
		for (int i = 0; i < (int)word.length(); ++i) {
			char letter = word[i];
			for (int j = 0; j < 26; ++j) {
				word[i] = 'a' + j;
				if (wordList.find(word) != wordList.end()) {
					toVisit.push(word);
					wordList.erase(word);
				}
			}
			word[i] = letter;
		}
	}
};

int main()
{
	Solution s;
	{
		string beginWord = "hit";
		string endWord = "cog";
		unordered_set<string> wordList{ "hot", "dot", "dog", "lot", "log" };
		cout << s.ladderLength(beginWord, endWord, wordList) << endl; // 5 "hit" -> "hot" -> "dot" -> "dog" -> "cog"
	}

	std::cin.get();
	return 0;
}




