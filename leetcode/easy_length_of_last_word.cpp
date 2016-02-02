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
	int lengthOfLastWord(string s) {
		if (s.empty()) return 0;
		int tail = s.find_last_not_of(' ');
		if (tail == string::npos) return 0;
		int head = s.find_last_of(' ', tail);
		if (tail == string::npos) return tail + 1;
		return tail - head;
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




