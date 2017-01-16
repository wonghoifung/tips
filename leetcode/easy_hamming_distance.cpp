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

class Solution {
public:
	int hammingDistance(int x, int y) {
		int d = 0;
		for (int i = 0; i < 32; ++i) {
			if (((x >> i) & 1) != ((y >> i) & 1)) d += 1;
		}
		return d;
	}
};

int main() {
	cout << Solution().hammingDistance(1, 4) << endl; // 2

	cin.get();
	return EXIT_SUCCESS;
}

