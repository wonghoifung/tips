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
	string countAndSay(int n) {
		string current = "1";
		int index = 1;
		while (n > index)
		{
			index += 1;

			stringstream ss;
			int cnt = 1;
			char c = current[0];
			for (int i = 1; i < current.size(); ++i)
			{
				if (c == current[i]) {
					cnt += 1;
				}
				else {
					ss << cnt << c;
					cnt = 1;
					c = current[i];
				}
			}
			ss << cnt << c;
			current = ss.str();
		}
		return current;
	}
};

int main()
{
	Solution s;
	{
		cout << s.countAndSay(1) << endl;
	}

	std::cin.get();
	return 0;
}




