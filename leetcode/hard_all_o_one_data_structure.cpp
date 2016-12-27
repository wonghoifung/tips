#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#if 0
class AllOne {
	unordered_map<string, int> kcount;
	map<int, unordered_set<string>> ckeys;
public:
	/** Initialize your data structure here. */
	AllOne() {

	}

	/** Inserts a new key <Key> with value 1. Or increments an existing key by 1. */
	void inc(string key) {
		int oldc = kcount[key];
		int newc = oldc + 1;
		kcount[key] = newc;
		ckeys[oldc].erase(key); if (ckeys[oldc].empty()) ckeys.erase(oldc);
		ckeys[newc].insert(key);
	}

	/** Decrements an existing key by 1. If Key's value is 1, remove it from the data structure. */
	void dec(string key) {
		int oldc = kcount[key];
		if (oldc == 0) {
			kcount.erase(key);
			return;
		}
		int newc = oldc - 1;
		kcount[key] = newc;
		ckeys[oldc].erase(key); if (ckeys[oldc].empty()) ckeys.erase(oldc);
		if (newc == 0) {
			kcount.erase(key);
			return;
		}
		ckeys[newc].insert(key);
	}

	/** Returns one of the keys with maximal value. */
	string getMaxKey() {
		if (ckeys.empty()) return "";
		auto end = ckeys.end(); --end;
		return *(end->second.begin());
	}

	/** Returns one of the keys with Minimal value. */
	string getMinKey() {
		if (ckeys.empty()) return "";
		return *(ckeys.begin()->second.begin());
	}
};
#endif

/* crash on vs2013 */
class AllOne {
public:

	void inc(string key) {

		/* If the key doesn't exist, insert it with value 0. */
		if (!bucketOfKey.count(key))
			bucketOfKey[key] = buckets.insert(buckets.begin(), { 0, { key } });

		/* Insert the key in next bucket and update the lookup. */
		auto next = bucketOfKey[key], bucket = next++;
		if (next == buckets.end() || next->value > bucket->value + 1)
			next = buckets.insert(next, { bucket->value + 1, {} });
		next->keys.insert(key);
		bucketOfKey[key] = next;

		/* Remove the key from its old bucket. */
		bucket->keys.erase(key);
		if (bucket->keys.empty())
			buckets.erase(bucket);
	}

	void dec(string key) {

		/* If the key doesn't exist, just leave. */
		if (!bucketOfKey.count(key))
			return;

		/* Maybe insert the key in previous bucket and update the lookup. */
		auto prev = bucketOfKey[key], bucket = prev--;
		bucketOfKey.erase(key);
		if (bucket->value > 1) {
			if (bucket == buckets.begin() || prev->value < bucket->value - 1)
				prev = buckets.insert(bucket, { bucket->value - 1, {} });
			prev->keys.insert(key);
			bucketOfKey[key] = prev;
		}

		/* Remove the key from its old bucket. */
		bucket->keys.erase(key);
		if (bucket->keys.empty())
			buckets.erase(bucket);
	}

	string getMaxKey() {
		return buckets.empty() ? "" : *(buckets.rbegin()->keys.begin());
	}

	string getMinKey() {
		return buckets.empty() ? "" : *(buckets.begin()->keys.begin());
	}

private:
	struct Bucket { int value; unordered_set<string> keys; };
	list<Bucket> buckets;
	unordered_map<string, list<Bucket>::iterator> bucketOfKey;
};

int main() {
	AllOne obj;
	obj.inc("abc");
	obj.inc("abc");
	obj.inc("xyz");
	cout << obj.getMaxKey() << endl; // abc
	cout << obj.getMinKey() << endl; // xyz
	obj.dec("abc");
	cout << obj.getMaxKey() << endl; // abc or xyz
	cout << obj.getMinKey() << endl; // abc or xyz
	obj.dec("abc");
	cout << obj.getMaxKey() << endl; // xyz
	cout << obj.getMinKey() << endl; // xyz

	cin.get();
	return EXIT_SUCCESS;
}

