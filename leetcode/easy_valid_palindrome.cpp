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
	bool isPalindrome(string s) {
		if (s.empty()) return true;
		int head = 0;
		int tail = s.size() - 1; 
		while (head < tail && head < s.size() && tail >= 0)
		{
			if (isalnum(s[head]) && isalnum(s[tail])) {
				if (tolower(s[head]) != tolower(s[tail]))
					return false;
				head += 1;
				tail -= 1;
				continue;
			}
			if (!isalnum(s[head])) head += 1;
			if (!isalnum(s[tail])) tail -= 1;
		}
		return true;
	}
};

int main()
{
	Solution s;
	{
		cout << s.isPalindrome("A man, a plan, a canal: Panama") << endl;
	}

	std::cin.get();
	return 0;
}




