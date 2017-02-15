#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>s
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

/* https://discuss.leetcode.com/topic/75465/concise-3ms-c-solution/2 */
#define MAX_STEP 6
class Solution {
	string shrink(string s) {
		while (s.size() > 0) {
			int start = 0;
			bool done = true;
			for (int i = 0; i <= s.size(); ++i) {
				if (i == s.size() || s[i] != s[start]) {
					if (i - start >= 3) {
						s = s.substr(0, start) + s.substr(i);
						done = false;
						break;
					}
					start = i;
				}
			}
			if (done) break;
		}
		return s;
	}
	int helper(string b, string h) {
		if (b.empty()) return 0;
		if (h.empty()) return MAX_STEP;
		int res = MAX_STEP;
		for (int i = 0; i < h.size(); ++i) {
			int j = 0;
			int n = b.size();
			while (j < n) {
				int k = b.find(h[i], j);
				if (k == string::npos) break;
				if (k < n - 1 && b[k] == b[k + 1]) {
					string nextb = shrink(b.substr(0, k) + b.substr(k + 2));
					if (nextb.empty()) return 1;
					string nexth = h;
					nexth.erase(i, 1);
					res = min(res, helper(nextb, nexth) + 1);
					k += 1;
				}
				else if (i > 0 && h[i] == h[i - 1]) {
					string nextb = shrink(b.substr(0, k) + b.substr(k + 1));
					if (nextb.empty()) return 2;
					string nexth = h;
					nexth.erase(i, 1);
					nexth.erase(i - 1, 1);
					res = min(res, helper(nextb, nexth) + 2);
				}
				j = k + 1;
			}
		}
		return res;
	}
public:
	int findMinStep(string board, string hand) {
		sort(hand.begin(), hand.end());
		int res = helper(board, hand);
		return res > hand.size() ? -1 : res;
	}
};

int main() {
	cout << Solution().findMinStep("WRRBBW", "RB") << endl; // -1
	cout << Solution().findMinStep("WWRRBBWW", "WRBRW") << endl; // 2
	cout << Solution().findMinStep("G", "GGGGG") << endl; // 2
	cout << Solution().findMinStep("RBYYBBRRB", "YRBGB") << endl; // 3

	cin.get();
}

