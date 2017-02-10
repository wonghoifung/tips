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
public:
	int magicalString(int n) {
		string S = "122";
		int i = 2;
		while (S.size() < n) {
			S += string(S[i++] - '0', S.back() ^ 3);
		}
		return count(S.begin(), S.begin() + n, '1');
	}
};

int main() {
	cout << Solution().magicalString(6) << endl; // 3

	cin.get();
}

