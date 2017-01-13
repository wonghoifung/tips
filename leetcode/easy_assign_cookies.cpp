#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Solution {
public:
	int findContentChildren(vector<int>& g, vector<int>& s) { // children cookies
		sort(g.begin(), g.end());
		sort(s.begin(), s.end());
		int res = 0;
		for (int i = 0; res < g.size() && i < s.size(); ++i) {
			if (g[res] <= s[i]) res += 1;
		}
		return res;
	}
};

int main() {
	{
		vector<int> g{ 1, 2, 3 }, s{ 1, 1 };
		cout << Solution().findContentChildren(g, s) << endl; // 1
	}
	{
		vector<int> g{ 1, 2 }, s{ 1, 2, 3 };
		cout << Solution().findContentChildren(g, s) << endl; // 2
	}

	cin.get();
	return EXIT_SUCCESS;
}

