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
	void reverseWords(string &s) {
		vector<string> vs;
		string current;
		for (int i = 0; i < s.size(); ++i)
		{
			if (isspace(s[i])) {
				if (!current.empty()) vs.push_back(current);
				current = "";
			}
			else {
				current += s[i];
			}
		}
		if (!current.empty()) vs.push_back(current);
		stringstream ss;
		reverse(vs.begin(), vs.end());
		bool isfirst = true;
		for (const auto& s : vs) {
			if (isfirst) {
				ss << s;
				isfirst = false;
			}
			else {
				ss << " " << s;
			}
		}
		s = ss.str();
	}
};

int main()
{
	Solution s;
	{
		string str = "the sky is blue";
		s.reverseWords(str);
		cout << str << endl;
	}

	std::cin.get();
	return 0;
}




