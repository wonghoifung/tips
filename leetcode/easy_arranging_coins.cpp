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
public:
	int arrangeCoins(int n) {
		if (n < 1) return 0;
		int i = 1;
		for (;;) {
			n -= i;
			if (n < i + 1) break;
			i += 1;
		}
		return i;
	}
};

int main() {
	cout << Solution().arrangeCoins(5) << endl; // 2
	cout << Solution().arrangeCoins(8) << endl; // 3

	cin.get();
	return EXIT_SUCCESS;
}

