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

class WordDictionary {
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
		bool subSearchPattern(const string& word, TrieNode* pstart) const {
			auto p = pstart;
			for (size_t i = 0; i < word.size(); ++i)
			{
				char c = word.at(i);
				if (c == '.')
				{
					for (auto& apair : p->childs)
					{
						if (subSearchPattern(word.substr(i + 1), apair.second))
						{
							return true;
						}
					}
					return false;
				}
				else
				{
					auto it = p->childs.find(c);
					if (it != p->childs.end())
					{
						p = it->second;
						continue;
					}
					return false;
				}
			}
			return p && p->cnt > 0;
		}
		bool searchPattern(const string& word) const {
			return subSearchPattern(word, root);
		}
	private:
		TrieNode* root;
	};
	Trie tree;
	void addWord(const string& word) {
		tree.insert(word);
	}
	bool search(const string& word) const {
		return tree.searchPattern(word);
	}
};



int main()
{
	WordDictionary wordDictionary;
	wordDictionary.addWord("word");
	cout << boolalpha << wordDictionary.search("pattern") << endl;
	
	std::cin.get();

	return 0;
}

