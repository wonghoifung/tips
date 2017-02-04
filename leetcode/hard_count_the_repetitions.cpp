#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
	int getMaxRepetitions(string s1, int n1, string s2, int n2) {
		unordered_map<int, pair<int, int>> r; // key:current position in s1, value:current position in n*s1 and how many s2 have been found
		int k = 0;
		for (int i = 0; i < s1.length() * n1; ) {
			int rr = i % s1.length();
			if (r.find(rr) == r.end()) {
				r[rr] = make_pair(i, k);
			}
			else {
				auto i0 = r[rr].first;
				auto k0 = r[rr].second; 

				int n = (s1.length() * n1 - i0) / (i - i0); // we have n * (k - k0) complete s2 left

				/* skip repeated patterns */
				i = i0 + n * (i - i0);
				k = k0 + n * (k - k0);
			}

			for (int j = 0; i < s1.length() * n1 && j < s2.length(); ++i, ++j) {
				while (i < s1.length() * n1 && s1[i % s1.length()] != s2[j]) ++i;
			}

			if (i >= s1.length() * n1) break;

			++k;
		}
		return k / n2;
	}
};

int main() {
	{
		string s1 = "abc", s2 = "ab";
		int n1 = 4, n2 = 2;
		cout << Solution().getMaxRepetitions(s1, n1, s2, n2) << endl; // 2
	}
	{
		string s1 = "phqghumeaylnlfdxfircvscxggbwkfnqduxwfnfozvsrtkjprepggxrpnrvystmwcysyycqpevikef", s2 = "fmznimkkasvwsrenzkycxfxtlsgypsfad";
		int n1 = 1000000, n2 = 333;
		cout << Solution().getMaxRepetitions(s1, n1, s2, n2) << endl;
	}

	cin.get();
}

