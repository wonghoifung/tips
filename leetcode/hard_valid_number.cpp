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
	string trim(string& str)
	{
		if (str.empty()) return str;
		auto r = str.find_first_not_of(' ');
		if (r != string::npos) str.erase(0, r);
		else return "";
		r = str.find_last_not_of(' ');
		if (r != string::npos) str.erase(r + 1); 
		else return "";
		return str;
	}
	bool headValid(char c) {
		return c == '.' || c == '-' || c == '+' || isdigit(c);
	}
	bool tailValid(char c) {
		return c == '.' || isdigit(c);
	}
	bool isNumber(string s) {
		s = trim(s);
		if (s.empty()) return false;
		if (!headValid(s.at(0)) || !tailValid(s.back())) return false;
		if (s.size() == 1 && s[0] == '.') return false;
		int dotcnt = 0;
		int ecnt = 0;
		bool numBeforeE = false;
		bool numAfterE = false;
		bool hasNum = false;
		
		for (int i = 0; i < s.size(); ++i)
		{
			if (i == 0 && (s.at(i) == '-' || s.at(i) == '+')) continue;
			if (s.at(i) == '.') {
				if (ecnt > 0) return false;
				dotcnt += 1;
				if (dotcnt > 1) return false;
				continue;
			}
			if (s.at(i) == 'e' || s.at(i) == 'E') {
				if (!numBeforeE) return false;
				ecnt += 1;
				if (ecnt > 1) return false;
				continue;
			}
			if (!isdigit(s.at(i))) {
				if (i > 0 && (s.at(i - 1) == 'e' || s.at(i - 1) == 'E') && (s.at(i) == '-' || s.at(i) == '+'));
				else return false;
			}
			if (ecnt == 0) numBeforeE = true;
			if (ecnt == 1) numAfterE = true;
			hasNum = true;
		}
		if (ecnt == 1 && numAfterE == false) return false;
		return hasNum;
	}
};

int main()
{
	Solution s;
	{
		cout << boolalpha << s.isNumber("4e+") << endl;
	}

	std::cin.get();
	return 0;
}




