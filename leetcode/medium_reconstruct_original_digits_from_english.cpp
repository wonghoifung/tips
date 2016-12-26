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
	string originalDigits(string s) {
		int count[10] = { 0 };
		for (int i = 0; i < s.size(); ++i) {
			char c = s[i];
			switch (c)
			{   
			case 'z':
				count[0] += 1;
				break;
			case 'w':
				count[2] += 1;
				break;
			case 'x':
				count[6] += 1;
				break;
			case 's':
				count[7] += 1; // 7 - 6
				break;
			case 'g':
				count[8] += 1;
				break;
			case 'u':
				count[4] += 1;
				break;
			case 'f':
				count[5] += 1; // 5 - 4
				break;
			case 'h':
				count[3] += 1; // 3 - 8
				break;
			case 'i':
				count[9] += 1; // 9 - 8 - 5 - 6
				break;
			case 'o':
				count[1] += 1; // 1 - 0 - 2 - 4
				break;
			}
		}
		count[7] -= count[6];
		count[5] -= count[4];
		count[3] -= count[8];
		count[9] = count[9] - count[8] - count[5] - count[6];
		count[1] = count[1] - count[0] - count[2] - count[4];
		stringstream ss;
		for (int i = 0; i <= 9; ++i) {
			for (int j = 0; j < count[i]; ++j) {
				ss << i;
			}
		}
		return ss.str();
	}
};

int main() {
	cout << Solution().originalDigits("owoztneoer") << endl; // 012 zero one two
	cout << Solution().originalDigits("fviefuro") << endl; // 45 four five

	cin.get();
	return EXIT_SUCCESS;
}

