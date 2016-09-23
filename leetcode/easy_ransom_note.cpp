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
	bool canConstruct(string ransomNote, string magazine) {
		unordered_map<char, int> mag;
		for (auto c : magazine) {
			mag[c] += 1;
		}
		for (auto c : ransomNote) {
			if (mag[c] > 0) {
				mag[c] -= 1;
			}
			else {
				return false;
			}
		}
		return true;
	}
};

int main()
{
	cout << boolalpha << Solution().canConstruct("a", "b") << endl;
	cout << boolalpha << Solution().canConstruct("aa", "ab") << endl;
	cout << boolalpha << Solution().canConstruct("aa", "aab") << endl;
	
	std::cin.get();
	return 0;
}




