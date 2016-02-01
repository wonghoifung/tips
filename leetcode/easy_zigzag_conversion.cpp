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
	string convert(string s, int numRows) {
		if (numRows <= 1) return s;
		vector<string> vres(numRows);
		bool down = true;
		int nextstr = 0;
		for (int i = 0; i < s.size(); ++i)
		{
			vres[nextstr] += s.at(i);
			if (down) {
				nextstr += 1;
				if (nextstr == numRows) {
					down = false;
					nextstr = numRows - 2;
				}
			}
			else {
				nextstr -= 1;
				if (nextstr < 0) {
					down = true;
					nextstr = 1;
				}
			}
		}
		string res;
		for (const auto& s : vres) res += s; 
		return res;
	}
};

int main()
{
	Solution s;
	{
		cout << s.convert("PAYPALISHIRING", 3) << endl;
	}

	std::cin.get();
	return 0;
}




