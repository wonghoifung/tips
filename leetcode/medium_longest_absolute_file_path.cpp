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
	int lengthLongestPath(string input) {
		int maxlen(0);
		vector<char> tmpname;
		bool isfile(false);
		vector<int> pathlenrecord;
		int level = 0;
		int whitespacecnt = 0;
		bool couldbetab(false);
		auto reset = [&](){
			tmpname.clear();
			isfile = false;
		};
		auto pathlen = [&]()->int{
			if (pathlenrecord.empty()) return tmpname.size();
			return pathlenrecord.back() + tmpname.size();
		};
		auto shrink = [&](){
			if (level < pathlenrecord.size()) {
				pathlenrecord.erase(pathlenrecord.begin() + level, pathlenrecord.end());
			}
		};
		for (size_t i = 0; i < input.size(); ++i)
		{
			switch (input[i])
			{
			case '\n':
				whitespacecnt = 0;
				shrink();
				if (isfile) {
					maxlen = max(pathlen(), maxlen);
				}
				else {
					if (pathlenrecord.empty()) pathlenrecord.push_back(tmpname.size() + 1);
					else pathlenrecord.push_back(pathlenrecord.back() + tmpname.size() + 1);
				}
				level = 0;
				reset();
				couldbetab = true;
				break;
			case '\t':
				whitespacecnt = 0;
				level += 1;
				reset();
				couldbetab = true;
				if (level == pathlenrecord.size()) couldbetab = false;
				break;
			case ' ':
				whitespacecnt += 1;
				if (couldbetab && whitespacecnt == 4) {
					level += 1;
					reset();
					whitespacecnt = 0;
					if (level == pathlenrecord.size()) couldbetab = false;
				}
				else if (!couldbetab) {
					tmpname.push_back(input[i]);
				}
				break;
			case '.':
				whitespacecnt = 0;
				tmpname.push_back(input[i]);
				isfile = true;
				couldbetab = false;
				break;
			default:
				whitespacecnt = 0;
				tmpname.push_back(input[i]);
				couldbetab = false;
				break;
			}
		}
		if (!tmpname.empty() && isfile) {
			shrink();
			maxlen = max(pathlen(), maxlen);
		}
		return maxlen;
	}
};

int main()
{
	cout << Solution().lengthLongestPath("a.ext") << endl; // 5
	cout << Solution().lengthLongestPath("dir\n    file.txt") << endl; // 12
	cout << Solution().lengthLongestPath("dir\n        file.txt") << endl; // 16
	cout << Solution().lengthLongestPath("file name with  space.txt") << endl; // 25
	cout << Solution().lengthLongestPath("dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext") << endl; // 20
	cout << Solution().lengthLongestPath("dir\n\tsubdir1\n\t\tfile1.ext\n\t\tsubsubdir1\n\tsubdir2\n\t\tsubsubdir2\n\t\t\tfile2.ext") << endl; // 32
	cout << Solution().lengthLongestPath("dir\n\t        file.txt\n\tfile2.txt") << endl; // 20
	cout << Solution().lengthLongestPath("a\n\tb\n\t\tc.txt\n\taaaa.txt") << endl; // 10
	
	std::cin.get();
	return 0;
}



