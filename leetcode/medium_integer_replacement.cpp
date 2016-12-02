#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <array>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

class Solution {
	unordered_map<int, int> visited;
public:
	int integerReplacement(int n) {
		if (n == 1) return 0;
		if (visited.count(0) == 0) {
			if (n & 1 == 1) {
				visited[n] = 2 + min(integerReplacement(n >> 1), integerReplacement((n >> 1) + 1));
			}
			else {
				visited[n] = 1 + integerReplacement(n >> 1);
			}
		}
		return visited[n];
	}
};

int main()
{
	cout << Solution().integerReplacement(8) << endl; // 3
	cout << Solution().integerReplacement(7) << endl; // 4
	cout << Solution().integerReplacement(2147483647) << endl; // 32

	std::cin.get();
	return 0;
}



