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
	bool isValid(string s) {
		stack<char> seen;
		for (int i = 0; i < s.size(); ++i)
		{
			char c = s[i];
			if (c == '(' || c == '[' || c == '{') {
				seen.push(c);
			}
			else {
				if (seen.empty()) return false;
				char l = seen.top();
				switch (c)
				{
				case ')':
					if (l != '(') return false;
					break;
				case ']':
					if (l != '[') return false;
					break;
				case '}':
					if (l != '{') return false;
					break;
				default:
					return false;
				}
				seen.pop();
			}
		}
		return seen.empty();
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




