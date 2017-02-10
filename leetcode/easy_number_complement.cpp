#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	int findComplement(int num) {
		if (num == 0) return 1;
		if (num == 1) return 0;
		int res = num ^ 0xffffffff;
		for (int i = 31; i >= 0; --i) {
			if (res & (1 << i)) res &= ~(1 << i);
			else break;
		}
		return res;
	}
};

int main() {
	cout << Solution().findComplement(5) << endl; // 2
	cout << Solution().findComplement(1) << endl; // 0

	cin.get();
}

