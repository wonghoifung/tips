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
	class TrieNode {
	public:
		char c;
		int cnt;
		unordered_map<char, TrieNode*> childs;
		TrieNode() {
			c = 0;
			cnt = 0;
		}
	};
	class Trie {
	public:
		Trie() {
			root = new TrieNode();
		}
		~Trie() {
			deleteTrieNode(root);
		}
		void deleteTrieNode(TrieNode* tn) {
			if (tn)
			{
				for (auto& p : tn->childs)
				{
					deleteTrieNode(p.second);
				}
				delete tn;
			}
		}
		void insert(const string& word) {
			auto p = root;
			for (auto c : word)
			{
				auto it = p->childs.find(c);
				if (it != p->childs.end())
				{
					p = it->second;
					continue;
				}
				p->childs.insert({ c, new TrieNode() });
				p->childs[c]->c = c;
				p = p->childs[c];
			}
			p->cnt += 1;
		}
		TrieNode* findNode(const string& s) const {
			auto p = root;
			for (auto c : s)
			{
				auto it = p->childs.find(c);
				if (it != p->childs.end())
				{
					p = it->second;
					continue;
				}
				return NULL;
			}
			return p;
		}
		bool search(const string& word) const {
			auto p = findNode(word);
			return p && p->cnt > 0;
		}
		bool startsWith(const string& prefix) const {
			return findNode(prefix);
		}
		TrieNode* root;
	};

	int w;
	int h;
	void searchWord(int x, int y, const string& prefix, set<pair<int, int>>& history, vector<vector<char>>& board, vector<string>& result, unordered_set<string>& sresult, Trie& wordsTree) {
		char c = board[x][y];
		string word = prefix + c;
		if (wordsTree.search(word))
		{
			if (sresult.find(word) == sresult.end())
			{
				result.push_back(word);
				sresult.insert(word);
			}
		}
		if (wordsTree.startsWith(word))
		{
			if (x > 0) // not top most
			{
				if (history.find({ x - 1, y }) == history.end())
				{
					history.insert({ x - 1, y });
					searchWord(x - 1, y, word, history, board, result, sresult, wordsTree);
					history.erase({ x - 1, y });
				}
			}
			if (x < h - 1) // not bottom most
			{
				if (history.find({ x + 1, y }) == history.end())
				{
					history.insert({ x + 1, y });
					searchWord(x + 1, y, word, history, board, result, sresult, wordsTree);
					history.erase({ x + 1, y });
				}
			}
			if (y > 0) // not left most
			{
				if (history.find({ x, y - 1 }) == history.end())
				{
					history.insert({ x, y - 1 });
					searchWord(x, y - 1, word, history, board, result, sresult, wordsTree);
					history.erase({ x, y - 1 });
				}
			}
			if (y < w - 1) // not right most
			{
				if (history.find({ x, y + 1 }) == history.end())
				{
					history.insert({ x, y + 1 });
					searchWord(x, y + 1, word, history, board, result, sresult, wordsTree);
					history.erase({ x, y + 1 });
				}
			}
		}
	}
	void searchWord(int x, int y, const string& prefix, set<pair<int, int>>& history, vector<vector<char>>& board, vector<string>& result, unordered_set<string>& sresult, TrieNode* treeNode) {
		char c = board[x][y];

		auto it = treeNode->childs.find(c);
		if (it != treeNode->childs.end())
		{
			string word = prefix + c;
			if (it->second->cnt > 0) // match whole word
			{
				if (sresult.find(word) == sresult.end())
				{
					result.push_back(word);
					sresult.insert(word);
				}
			}
			/*match prefix*/
			{
				if (x > 0) // not top most
				{
					if (history.find({ x - 1, y }) == history.end())
					{
						history.insert({ x - 1, y });
						searchWord(x - 1, y, word, history, board, result, sresult, it->second);
						history.erase({ x - 1, y });
					}
				}
				if (x < h - 1) // not bottom most
				{
					if (history.find({ x + 1, y }) == history.end())
					{
						history.insert({ x + 1, y });
						searchWord(x + 1, y, word, history, board, result, sresult, it->second);
						history.erase({ x + 1, y });
					}
				}
				if (y > 0) // not left most
				{
					if (history.find({ x, y - 1 }) == history.end())
					{
						history.insert({ x, y - 1 });
						searchWord(x, y - 1, word, history, board, result, sresult, it->second);
						history.erase({ x, y - 1 });
					}
				}
				if (y < w - 1) // not right most
				{
					if (history.find({ x, y + 1 }) == history.end())
					{
						history.insert({ x, y + 1 });
						searchWord(x, y + 1, word, history, board, result, sresult, it->second);
						history.erase({ x, y + 1 });
					}
				}
			}
		}
	}
	bool checkWord(string w, vector<string>& words) {
		Trie tree;
		for (auto& word : words)
		{
			tree.insert(word);
		}
		return tree.search(w);
	}
	vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
		vector<string> ret;

		h = board.size();
		if (h == 0)
		{
			return ret;
		}
		w = board[0].size();

		Trie tree;
		for (auto& word : words)
		{
			tree.insert(word);
		}
		
		unordered_set<string> sresult;
		set<pair<int, int>> history;
		for (size_t x = 0; x < h; ++x)
		{
			for (size_t y = 0; y < w; ++y)
			{
				history.insert({ x, y });
				/*searchWord(x, y, "", history, board, ret, sresult, tree);*/
				searchWord(x, y, "", history, board, ret, sresult, tree.root);
				history.erase({ x, y });
			}
		}

		return ret;
	}
};

int main()
{
	Solution s;
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'o', 'a', 'a', 'n' },
			{ 'e', 't', 'a', 'e' },
			{ 'i', 'h', 'k', 'r' },
			{ 'i', 'f', 'l', 'v' } };
		vector<string> words{ "oath", "pea", "eat", "rain" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'a', 'a', 'a', 'a' },
			{ 'a', 'a', 'a', 'a' },
			{ 'a', 'a', 'a', 'a' } };
		vector<string> words{ "aaaaaaaaaaaa", "aaaaaaaaaaaaa", "aaaaaaaaaaab" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'a', 'b' },
			{ 'c', 'd' } };
		vector<string> words{ "cdba" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'a', 'b' },
			{ 'c', 'd' } };
		vector<string> words{ "acdb" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'a', 'b' },
			{ 'a', 'a' } };
		vector<string> words{ "aba", "baa", "bab", "aaab", "aaa", "aaaa", "aaba" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	std::cin.get();

	return 0;
}
/*
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
	class TrieNode {
	public:
		char c;
		int cnt;
		TrieNode* childs[26];
		TrieNode() {
			c = 0;
			cnt = 0;
			memset(&childs[0], 0, sizeof(TrieNode*) * 26);
		}
	};
	class Trie {
	public:
		Trie() {
			root = new TrieNode();
		}
		~Trie() {
			deleteTrieNode(root);
		}
		void deleteTrieNode(TrieNode* tn) {
			if (tn)
			{
				for (auto& p : tn->childs)
				{
					deleteTrieNode(p);
				}
				delete tn;
			}
		}
		static int index(char c) { return c - 'a'; }
		void insert(const string& word) {
			auto p = root;
			for (auto c : word)
			{
				if (p->childs[index(c)])
				{
					p = p->childs[index(c)];
					continue;
				}
				p->childs[index(c)] = new TrieNode();
				p->childs[index(c)]->c = c;
				p = p->childs[index(c)];
			}
			p->cnt += 1;
		}
		TrieNode* root;
	};
	int w;
	int h;
	void searchWord(int x, int y, const string& prefix, bool** history, vector<vector<char>>& board, vector<string>& result, unordered_set<string>& sresult, TrieNode* treeNode) {
		char c = board[x][y];

		if (treeNode->childs[Trie::index(c)])
		{
			string word = prefix + c;
			if (treeNode->childs[Trie::index(c)]->cnt > 0) // match whole word
			{
				if (sresult.find(word) == sresult.end())
				{
					result.push_back(word);
					sresult.insert(word);
				}
			}
			{ // match prefix
				if (x > 0) // not top most
				{
					if (!history[x - 1][y])
					{
						history[x - 1][y] = true;
						searchWord(x - 1, y, word, history, board, result, sresult, treeNode->childs[Trie::index(c)]);
						history[x - 1][y] = false;
					}
				}
				if (x < h - 1) // not bottom most
				{
					if (!history[x + 1][y])
					{
						history[x + 1][y] = true;
						searchWord(x + 1, y, word, history, board, result, sresult, treeNode->childs[Trie::index(c)]);
						history[x + 1][y] = false;
					}
				}
				if (y > 0) // not left most
				{
					if (!history[x][y - 1])
					{
						history[x][y - 1] = true;
						searchWord(x, y - 1, word, history, board, result, sresult, treeNode->childs[Trie::index(c)]);
						history[x][y - 1] = false;
					}
				}
				if (y < w - 1) // not right most
				{
					if (!history[x][y + 1])
					{
						history[x][y + 1] = true;
						searchWord(x, y + 1, word, history, board, result, sresult, treeNode->childs[Trie::index(c)]);
						history[x][y + 1] = false;
					}
				}
			}
		}
	}
	vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
		vector<string> ret;

		h = board.size();
		if (h == 0)
		{
			return ret;
		}
		w = board[0].size();

		Trie tree;
		for (auto& word : words)
		{
			tree.insert(word);
		}
		
		bool** history = new bool*[h];
		for (int i = 0; i<h; ++i) {
			history[i] = new bool[w];
			memset(history[i], false, sizeof(bool)*w);
		}

		unordered_set<string> sresult;
		for (size_t x = 0; x < h; ++x)
		{
			for (size_t y = 0; y < w; ++y)
			{
				history[x][y] = true;
				searchWord(x, y, "", history, board, ret, sresult, tree.root);
				history[x][y] = false;
			}
		}

		for (int i = 0; i<h; ++i) {
			delete[] history[i];
		}
		delete[] history;

		return ret;
	}
};

int main()
{
	Solution s;
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'o', 'a', 'a', 'n' },
			{ 'e', 't', 'a', 'e' },
			{ 'i', 'h', 'k', 'r' },
			{ 'i', 'f', 'l', 'v' } };
		vector<string> words{ "oath", "pea", "eat", "rain" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'a', 'a', 'a', 'a' },
			{ 'a', 'a', 'a', 'a' },
			{ 'a', 'a', 'a', 'a' } };
		vector<string> words{ "aaaaaaaaaaaa", "aaaaaaaaaaaaa", "aaaaaaaaaaab" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'a', 'b' },
			{ 'c', 'd' } };
		vector<string> words{ "cdba" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'a', 'b' },
			{ 'c', 'd' } };
		vector<string> words{ "acdb" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	{
		mytimer tt;
		vector<vector<char>> board{
			{ 'a', 'b' },
			{ 'a', 'a' } };
		vector<string> words{ "aba", "baa", "bab", "aaab", "aaa", "aaaa", "aaba" };
		vector<string> ret = s.findWords(board, words);
		copy(ret.begin(), ret.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}
	std::cin.get();

	return 0;
}

 */
