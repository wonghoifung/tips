#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>s
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
	char formatChar(char c) {
		if (c >= 'a' && c <= 'z') return toupper(c);
		return c;
	}
public:
	string licenseKeyFormatting(string S, int K) {
		stringstream ss;
		int n = K;
		for (int i = S.size() - 1; i >= 0; --i) {
			if (S[i] == '-') continue;
			char c = formatChar(S[i]);
			ss << c;
			n -= 1;
			if (n == 0) {
				if (i == 0) break;
				ss << '-';
				n = K;
			}
		}
		string s = ss.str();
		reverse(s.begin(), s.end());
		auto idx = s.find_first_not_of('-');
		if (idx == string::npos) return "";
		return s.substr(idx);
	}
};

int main() {
	cout << Solution().licenseKeyFormatting("2-4A0r7-4k", 4) << endl;
	cout << Solution().licenseKeyFormatting("2-4A0r7-4k", 3) << endl;
	cout << Solution().licenseKeyFormatting("--a-a-a-a--", 2) << endl;
	cout << Solution().licenseKeyFormatting("---", 3) << endl;

	cin.get();
}
