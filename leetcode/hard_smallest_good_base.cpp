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

/* https://discuss.leetcode.com/topic/76347/3ms-ac-c-long-long-int-binary-search */
class Solution {
public:
	string smallestGoodBase(string n) {
		unsigned long long tn = (unsigned long long)stoll(n);
		unsigned long long x = 1;
		for (int i = 62; i >= 2; i--) {
			if ((x << i) < tn) {
				unsigned long long cur = mysolve(tn, i);
				if (cur != 0) return to_string(cur);
			}
		}
		return to_string(tn - 1);
	}

	unsigned long long mysolve(unsigned long long n, int d) { // len d
		double tn = (double)n;
		unsigned long long right = (unsigned long long)(pow(tn, 1.0 / d) + 1); // base range, right limit
		unsigned long long left = 2; // base range, left limit
		while (left <= right){ // find the base
			unsigned long long mid = left + (right - left) / 2; // base mid
			unsigned long long sum = 1, cur = 1;
			for (int i = 1; i <= d; i++) {
				cur *= mid;
				sum += cur;
			}
			if (sum == n) return mid;
			if (sum > n) right = mid - 1;
			else left = mid + 1;
		}
		return 0;
	}
};

int main() {
	cout << Solution().smallestGoodBase("470988884881403701") << endl;

	cin.get();
}

