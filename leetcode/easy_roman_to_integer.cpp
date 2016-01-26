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

using namespace std;

class Solution {
public:
	int romanToInt(string s) {
		if (s.empty()) return -1;
		unordered_map<char, int> m{
			{ 'I', 1 },
			{ 'V', 5 },
			{ 'X', 10 },
			{ 'L', 50 },
			{ 'C', 100 },
			{ 'D', 500 },
			{ 'M', 1000 }
		};
		int len = s.size();
		int result = m[s.at(len - 1)];
		for (int i = len - 2; i >= 0; --i)
		{
			if (m[s.at(i)] >= m[s.at(i + 1)]) {
				result += m[s.at(i)];
			}
			else {
				result -= m[s.at(i)];
			}
		}
		return result;
	}
};

int main()
{
	Solution s;
	{
		
	}

	std::cin.get();
	return 0;
}




