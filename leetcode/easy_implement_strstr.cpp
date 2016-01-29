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
	int strStr(string haystack, string needle) {
		if (needle.empty()) return 0;
		int hstart = 0;
		int hcur = 0;
		int ncur = 0;
		while (hstart + needle.size() <= haystack.size() && 
			hcur < haystack.size() && 
			hcur - hstart < haystack.size())
		{
			if (haystack[hcur] == needle[ncur]) {
				if (ncur == needle.size() - 1) return hstart;
				hcur += 1;
				ncur += 1;
			}
			else {
				hstart += 1;
				hcur = hstart;
				ncur = 0;
			}
		}
		return -1;
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




