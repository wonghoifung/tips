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
	char findTheDifference(string s, string t) {
		unordered_map<char, int> charcount;
		for (auto c : t) {
			charcount[c] += 1;
		}
		for (auto c : s) {
			charcount[c] -= 1;
			if (charcount[c] == 0) charcount.erase(c);
		}
		return charcount.begin()->first;
	}
};

int main()
{
	cout << Solution().findTheDifference("abcd", "abcde") << endl; // e
	
	std::cin.get();
	return 0;
}



