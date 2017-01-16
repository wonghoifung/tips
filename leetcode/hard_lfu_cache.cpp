#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

/* Could you do both operations in O(1) time complexity? */
class LFUCache {
	int cap;
	int size;
	int minFreq;
	unordered_map<int, pair<int, int>> kvf; // key to {value, freq}
	unordered_map<int, list<int>::iterator> keyiterators; // key to list iterator;
	unordered_map<int, list<int>> freqlist; // freq to key list

public:
	LFUCache(int capacity) :cap(capacity), size(0) {

	}

	/* Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1. */
	int get(int key) {
		if (kvf.count(key) == 0) return -1;
		freqlist[kvf[key].second].erase(keyiterators[key]); // 从freqlist取出
		kvf[key].second++; // 增加freq
		freqlist[kvf[key].second].push_back(key); // 加到新的freqlist
		keyiterators[key] = --freqlist[kvf[key].second].end(); // key在freqlist的位置
		if (freqlist[minFreq].size() == 0) minFreq++; // 记录最少访问的freqkey
		return kvf[key].first;
	}

	/* Set or insert the value if the key is not already present. When the cache reaches its capacity, */
	/* it should invalidate the least frequently used item before inserting a new item. For the purpose of this problem, */
	/* when there is a tie (i.e., two or more keys that have the same frequency), the least recently used key would be evicted. */
	void put(int key, int value) {
		if (cap <= 0) return;
		int storedValue = get(key);
		if (storedValue != -1) {
			kvf[key].first = value;
			return;
		}
		if (size >= cap) {
			kvf.erase(freqlist[minFreq].front()); // 删除lfu and lru键
			keyiterators.erase(freqlist[minFreq].front());
			freqlist[minFreq].pop_front();
			size--;
		}
		kvf[key] = { value, 1 };
		freqlist[1].push_back(key);
		keyiterators[key] = --freqlist[1].end();
		minFreq = 1;
		size++;
	}
};

int main() {
	LFUCache cache(2);

	cache.put(1, 1);
	cache.put(2, 2);
	cache.get(1);       // returns 1
	cache.put(3, 3);    // evicts key 2
	cache.get(2);       // returns -1 (not found)
	cache.get(3);       // returns 3.
	cache.put(4, 4);    // evicts key 1.
	cache.get(1);       // returns -1 (not found)
	cache.get(3);       // returns 3
	cache.get(4);       // returns 4

	cin.get();
	return EXIT_SUCCESS;
}

