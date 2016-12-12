#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
using namespace std;

class Solution {
public:
	string toHex(int num) {
		static const char hexchar[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
		int mask = 0x0000000f;
		stringstream ss;
		bool first = true;
		for (int shift = 28; shift >= 0; shift -= 4) {
			int x = mask & (num >> shift);
			if (first) {
				if (x == 0) continue;
				first = false;
			}
			ss << hexchar[x];
		}
		if (first) ss << '0';
		return ss.str();
	}
};

int main() {
	cout << Solution().toHex(26) << endl;
	cout << Solution().toHex(-1) << endl;

	cin.get();
	return EXIT_SUCCESS;
}

