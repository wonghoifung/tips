#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	string convertToBase7(int num) {
		if (num == 0) return "0";
		bool neg = num < 0;
		num = abs(num);
		vector<int> vs;
		while (num != 0) {
			vs.push_back(num % 7 + '0');
			num /= 7;
		}
		string s(vs.rbegin(), vs.rend());
		if (neg) s = "-" + s;
		return s;
	}
};

int main() {
	cout << Solution().convertToBase7(100) << endl; // "202"
	cout << Solution().convertToBase7(-7) << endl; // "-10"
	cin.get();
}

