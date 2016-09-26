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
public:
	vector<int> lexicalOrder(int n) {
		vector<int> res;
		res.reserve(n);
		int cur = 1;
		for (int i = 1; i <= n; ++i) {
			res.push_back(cur);
			if (cur * 10 <= n) {
				cur *= 10;
			}
			else if (cur % 10 != 9 && cur + 1 <= n) {
				cur += 1;
			}
			else {
				while ((cur / 10) % 10 == 9) {
					cur /= 10;
				}
				cur = cur / 10 + 1;
			}
		}
		return res;
	}
};

int main()
{
	auto print = [](const vector<int>& v) {
		for (auto i : v) {
			cout << i << " ";
		}
		cout << endl;
	};
	print(Solution().lexicalOrder(13));
	print(Solution().lexicalOrder(998));
	
	std::cin.get();
	return 0;
}



