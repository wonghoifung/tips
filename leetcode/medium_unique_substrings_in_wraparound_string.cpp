#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
	bool isCorrectOrder(char first, char second) {
		if (second - first == 1) return true;
		if (first == 'z' && second == 'a') return true;
		return false;
	}
public:
	int findSubstringInWraproundString(string p) {
		int count[26] = { 0 };
		int maxLengthCur = 0;
		for (int i = 0; i < p.size(); ++i) {
			if (i > 0 && isCorrectOrder(p[i - 1], p[i])) maxLengthCur += 1;
			else maxLengthCur = 1;
			int index = p[i] - 'a';
			count[index] = max(count[index], maxLengthCur);
		}
		return accumulate(begin(count), end(count), 0);
	}
};

int main() {
	cout << Solution().findSubstringInWraproundString("a") << endl; // 1
	cout << Solution().findSubstringInWraproundString("cac") << endl; // 2
	cout << Solution().findSubstringInWraproundString("zab") << endl; // 6

	cin.get();
}

