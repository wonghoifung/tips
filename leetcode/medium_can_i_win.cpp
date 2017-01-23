#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
	unordered_map<int, bool> m;
	vector<bool> used;
	int format(const vector<bool>& used) {
		int num = 0;
		for (bool b : used) {
			num <<= 1;
			if (b) num |= 1;
		}
		return num;
	}
	bool helper(int desiredTotal) {
		if (desiredTotal <= 0) return false;
		int key = format(used);
		if (!m.count(key)) {
			for (int i = 1; i < used.size(); ++i) {
				if (!used[i]) {
					used[i] = true;
					if (!helper(desiredTotal - i)) { // opponent lose
						m[key] = true;
						used[i] = false;
						return true;
					}
					used[i] = false;
				}
			}
			m[key] = false;
		}
		return m[key];
	}
public:
	bool canIWin(int maxChoosableInteger, int desiredTotal) {
		/* determine if the first player to move can force a win, assuming both players play optimally */
		int sum = (1 + maxChoosableInteger) * maxChoosableInteger / 2;
		if (sum < desiredTotal) return false;
		if (desiredTotal <= 0) return true;
		m.clear();
		used = vector<bool>(maxChoosableInteger + 1, false);
		return helper(desiredTotal);
	}
};

int main() {
	cout << boolalpha << Solution().canIWin(10, 11) << endl; // false

	cin.get();
}

