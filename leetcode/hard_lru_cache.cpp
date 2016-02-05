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

class LRUCacheBak {
public:
	LRUCacheBak(int capacity) :cap(capacity), insert_seq(0), update_seq(0) {
		assert(cap > 0);
	}

	int get(int key) {
		auto kv_it = kv.find(key);
		if (kv_it == kv.end()) return -1;
		int res = kv_it->second;
		
		/* update seq */
		auto seqlut_it = seqlut.find(key);
		auto pt = seqlut_it->second;
		lruq.erase(keyinfo(key, pt.first, pt.second));
		seqlut_it->second.first = update_seq;
		lruq.insert(keyinfo(key, update_seq, pt.second));
		update_seq += 1;
		if (update_seq < 0) update_seq = 0;

		return res;
	}

	void set(int key, int value) {
		auto kv_it = kv.find(key);
		if (kv_it == kv.end()) {
			if (kv.size() == cap) {
				auto del_it = lruq.end();
				--del_it;
				int del_key = del_it->key;
				kv.erase(del_key);
				seqlut.erase(del_key);
				lruq.erase(del_it);
			}
			kv.insert(make_pair(key, value));
			seqlut.insert(make_pair(key, make_pair(update_seq, insert_seq)));
			lruq.insert(keyinfo(key, update_seq, insert_seq++)); /* TODO handle insert_seq rewind */ 
		}
		else {
			auto kt_it = seqlut.find(key);
			auto pt = kt_it->second;
			lruq.erase(keyinfo(key, pt.first, pt.second));
			kt_it->second.first = update_seq;
			lruq.insert(keyinfo(key, update_seq, pt.second));
			kv_it->second = value;
		}
		update_seq += 1;
		if (update_seq < 0) update_seq = 0;
	}
private:
	int cap;
	int insert_seq;
	int update_seq;
	unordered_map<int, int> kv;
	unordered_map<int, pair<int, int>> seqlut; 
	struct keyinfo
	{
		int key;
		int updateseq;
		int insertseq;
		keyinfo(int k, int ut, int seq) :key(k), updateseq(ut), insertseq(seq) {}
		bool operator< (const keyinfo& rhs) const {
			if (updateseq != rhs.updateseq) return updateseq > rhs.updateseq;
			return insertseq > rhs.insertseq;
		}
	};
	std::set<keyinfo> lruq;
};

class LRUCache
{
	struct Node
	{
		Node* l;
		Node* r;
		int key;
		Node(int key) :l(NULL), r(NULL), key(key) {}
	};

public:
	LRUCache(int capacity) :cap(capacity), size(0), head(new Node(0)), tail(head) { 
		assert(cap > 0);
		assert(head);
	}

	~LRUCache() {
		while (size) delFirst();
		delete head;
	}

	int get(int key) {
		auto it = cache.find(key);
		if (it != cache.end()) {
			moveToLast(it->second.second);
			return it->second.first;
		}
		else {
			return -1;
		}
	}

	void set(int key, int value) {
		auto it = cache.find(key);
		if (it != cache.end()) {
			moveToLast(it->second.second);
			it->second.first = value;
		}
		else {
			if (size == cap) delFirst();
			addToLast(key, value);
		}
	}

private:
	void moveToLast(Node* node) {
		if (node == tail) return;
		node->l->r = node->r;
		if (node->r) node->r->l = node->l;
		tail->r = node;
		node->l = tail;
		tail = node;
	}

	void addToLast(int key, int value) {
		Node* n = new Node(key);
		n->l = tail;
		tail->r = n;
		tail = n;
		cache.insert({ key, { value, n } });
		size += 1;
	}

	void delFirst() {
		Node* n = head->r;
		cache.erase(n->key);
		head->r = n->r;
		if (n->r) n->r->l = head;
		delete n;
		size -= 1;
	}

	unordered_map<int, pair<int, Node*>> cache;
	int cap;
	int size;
	Node* head;
	Node* tail;
};

int main()
{
	LRUCache cache(2);
	cache.set(2, 1);
	cache.set(1, 1);
	cout << cache.get(2) << endl;
	cache.set(4, 1);
	cout << cache.get(1) << endl;
	cout << cache.get(2) << endl;

	std::cin.get();
	return 0;
}




