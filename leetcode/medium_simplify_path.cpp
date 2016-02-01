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
	string simplifyPath(string path) {
		vector<string> vs;
		while (!path.empty())
		{
			int idxstart = path.find_first_not_of('/');
			if (idxstart == string::npos) break;
			int idxend = path.find_first_of('/', idxstart);
			if (idxend == string::npos) {
				string s = path.substr(idxstart);
				if (s != ".") vs.push_back(s);
				break;
			}
			string s = path.substr(idxstart, idxend - idxstart);
			if (s != ".") vs.push_back(s);
			path = path.substr(idxend);
		}
		
		vector<string> vres;
		for (auto it = vs.begin(); it != vs.end(); ++it)
		{
			if (*it == "..") {
				if (!vres.empty()) vres.pop_back();
			}
			else {
				vres.push_back(*it);
			}
		}

		if (vres.empty()) return "/";

		stringstream ss;
		for (const auto& s : vres) ss << "/" << s;
		return ss.str();
	}
};

int main()
{
	Solution s;
	{
		cout << s.simplifyPath("/.") << endl;
	}

	std::cin.get();
	return 0;
}




