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
class Solution {
	bool isAnagram(string s1, string s2) {
		unordered_map<char, int> charcount;
		for (char c : s1) charcount[c] += 1;
		for (char c : s2) {
			auto it = charcount.find(c);
			if (it == charcount.end()) return false;
			it->second -= 1;
			if (it->second == 0) charcount.erase(it);
		}
		return charcount.empty();
	}
public:
	vector<int> findAnagrams(string s, string p) {
		vector<int> res;
		int slen = s.size();
		int plen = p.size();
		if (slen < plen) return res;
		for (int i = 0; i <= slen - plen; ++i) {
			if (isAnagram(s.substr(i, plen), p)) res.push_back(i);
		}
		return res;
	}
};
#else
class Solution {
public:
	vector<int> findAnagrams(string s, string p) {
		vector<int> res;
		if (s.empty() || p.empty()) return res;
		vector<int> phash(256, 0);
		for (char c : p) phash[c] += 1;
		int left = 0, right = 0, count = p.size();
		while (right < s.size()) {
			if (phash[s[right++]]-- >= 1) count--;
			if (count == 0) res.push_back(left);
			if (right - left == p.size() && phash[s[left++]]++ >= 0) count++;
		}
		return res;
	}
};
#endif

ostream& operator<<(ostream& o, const vector<int>& arr) {
	o << "[ ";
	for (int i : arr) {
		o << i << " ";
	}
	o << "]" << endl;
	return o;
}
int main() {
	cout << Solution().findAnagrams("cbaebabacd", "abc") << endl;
	cout << Solution().findAnagrams("abab", "ab") << endl;

	cin.get();
	return EXIT_SUCCESS;
}

