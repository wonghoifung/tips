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

class Solution {
	/* count the number of character that is different to the longest character */
	int count(vector<int>& ch) {
		int max = 0;
		int sum = 0;
		for (int val : ch) {
			sum += val;
			max = ::max(max, val);
		}
		return sum - max;
	}
public:
	int characterReplacement(string s, int k) {
		if (s.empty()) return 0;
		int max = 0;
		vector<int> ch(26, 0);
		for (int start = 0, end = 0; start < s.size(); ++start) {
			while (end < s.size()) {
				ch[s[end] - 'A'] += 1;
				if (count(ch) > k) { /* if exceed k, break */
					ch[s[end] - 'A'] -= 1;
					break;
				}
				end += 1; /* if it is not bigger than k we extend the sliding window */
			}
			max = ::max(max, end - start);
			ch[s[start] - 'A'] -= 1;
		}
		return max;
	}
};

int main() {
	cout << Solution().characterReplacement("ABAB", 2) << endl; // 4
	cout << Solution().characterReplacement("AABABBA", 1) << endl; // 4

	cin.get();
	return EXIT_SUCCESS;
}

