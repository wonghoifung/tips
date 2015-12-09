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
private:
	TrieNode* root;
};

int main()
{
	Trie trie;
	trie.insert("somestring");
	cout << boolalpha << trie.search("key") << endl;
	cout << boolalpha << trie.search("somestring") << endl;
	cout << boolalpha << trie.startsWith("some") << endl;
	cout << boolalpha << trie.startsWith("zz") << endl;
	trie.insert("ab");
	cout << boolalpha << trie.search("a") << endl;
	cout << boolalpha << trie.startsWith("a") << endl;

	std::cin.get();

	return 0;
}

