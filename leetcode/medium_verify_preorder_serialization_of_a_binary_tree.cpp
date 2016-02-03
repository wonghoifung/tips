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
	bool isValidSerialization(string preorder) {
		vector<string> nodes;
		string cur;
		for (int i = 0; i < preorder.size(); ++i)
		{
			if (preorder[i] == ' ') continue;
			if (preorder[i] == '#') {
				if (!cur.empty()) nodes.push_back(cur);
				cur = "";
				nodes.push_back("#");
			}
			else if (preorder[i] == ',') {
				if (!cur.empty()) nodes.push_back(cur);
				cur = "";
			}
			else {
				cur += preorder[i];
			}
		}
		if (!cur.empty()) nodes.push_back(cur);
		int diff = 1;
		for (const auto& s : nodes)
		{
			if (--diff < 0) return false;
			if (s != "#") diff += 2;
		}
		return diff == 0;
	}
};

int main()
{
	Solution s;
	{
		cout << s.isValidSerialization("9,3,4,#,#,1,#,#,2,#,6,#,#") << endl; // 1
		cout << s.isValidSerialization("9,3,4,#,#,1,#,#,2,#,6,#,#") << endl; // 1
		cout << s.isValidSerialization("1,#") << endl; // 0
		cout << s.isValidSerialization("9,#,#,1") << endl; // 0
	}

	std::cin.get();
	return 0;
}




