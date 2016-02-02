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
	struct item {
		bool isnum;
		union {
			int num;
			char op;
		} u;
	};
	int calculate(string s) {
		if (s.empty()) return 0;
		vector<item> v;
		string curnum;
		for (int i = 0; i < s.size(); ++i)
		{
			if (isspace(s[i])) {
				if (!curnum.empty()) v.push_back(item{ true, { stoi(curnum) } });
				curnum = "";
			}
			else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') {
				if (!curnum.empty()) v.push_back(item{ true, { stoi(curnum) } });
				curnum = "";
				v.push_back(item{ false, { s[i] } });
			}
			else {
				curnum += s[i];
				if (i == s.size() - 1) v.push_back(item{ true, { stoi(curnum) } });
			}
		}

		vector<item> v1;
		for (int i = 0; i < v.size(); ++i)
		{
			if (!v[i].isnum && (v[i].u.op == '*' || v[i].u.op == '/'))
			{
				int num = (v[i].u.op == '*' ? v1.back().u.num * v[i + 1].u.num : v1.back().u.num / v[i + 1].u.num);
				v1.back().isnum = true;
				v1.back().u.num = num;
				i += 1;
			}
			else {
				v1.push_back(v[i]);
			}
		}

		vector<item> v2;
		for (int i = 0; i < v1.size(); ++i)
		{
			if (!v1[i].isnum)
			{
				int num = (v1[i].u.op == '+' ? v2.back().u.num + v1[i + 1].u.num : v2.back().u.num - v1[i + 1].u.num);
				v2.back().isnum = true;
				v2.back().u.num = num;
				i += 1;
			}
			else {
				v2.push_back(v1[i]);
			}
		}

		return v2.back().u.num;
	}
};

int main()
{
	Solution s;
	{
		cout << s.calculate("1+1+1") << endl;
	}

	std::cin.get();
	return 0;
}




