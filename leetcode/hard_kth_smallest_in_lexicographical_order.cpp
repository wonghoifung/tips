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
/* denary tree */
class Solution {
	int getMin(int n1, int n2) { 
		return n1 < n2 ? n1 : n2; 
	}
	int calSteps(int n, long n1, long n2) {
		int steps = 0;
		while (n1 <= n) {
			steps += getMin(n + 1, n2) - n1; // TODO why?
			n1 *= 10;
			n2 *= 10;
		}
		return steps;
	}
public:
	int findKthNumber(int n, int k) {
		int curr = 1;
		k = k - 1;
		while (k > 0) {
			int steps = calSteps(n, curr, curr + 1); // TODO why?
			if (steps <= k) {
				curr += 1; // move to the next right neighbor
				k -= steps;
			}
			else {
				curr *= 10; // move to the most left child
				k -= 1;
			}
		}
		return curr;
	}
};
int main() {
	cout << Solution().findKthNumber(13, 2) << endl; // 10

	cin.get();
	return EXIT_SUCCESS;
}

