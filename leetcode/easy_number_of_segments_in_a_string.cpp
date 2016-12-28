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
	int countSegments(string s) {
		if (s.empty()) return 0;
		int cnt = 0;
		bool seg = false;
		for (auto c : s) {
			if (isspace(c)) {
				if (seg) cnt += 1;
				seg = false;
			}
			else seg = true;
		}
		if (seg) cnt += 1;
		return cnt;
	}
};

int main() {
	cout << Solution().countSegments("Hello, my name is John") << endl; // 5

	cin.get();
	return EXIT_SUCCESS;
}

