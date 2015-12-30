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

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(nullptr);
	}
	~mytimer()
	{
		time_t end = time(nullptr);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	void pushLeftP(vector<string>& res, const string& s, int unMatchLeftcnt, int lpleft, int rpleft) {
		for (int i = lpleft; i > 0; --i)
		{
			string ss(s);
			ss += string(i, '(');
			pushRightP(res, ss, unMatchLeftcnt + i, lpleft - i, rpleft);
		}
	}
	void pushRightP(vector<string>& res, const string& s, int unMatchLeftcnt, int lpleft, int rpleft) {
		if (lpleft == 0)
		{
			string ss(s);
			ss += string(rpleft, ')');
			res.push_back(ss);
			return;
		}
		for (int i = 1; i <= unMatchLeftcnt; ++i)
		{
			string ss(s);
			ss += string(i, ')');
			pushLeftP(res, ss, unMatchLeftcnt - i, lpleft, rpleft - i);
		}
	}
	vector<string> generateParenthesis(int n) {
		vector<string> res;
		pushLeftP(res, "", 0, n, n);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<string> res = solu.generateParenthesis(3);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n"));
	}

	std::cin.get();
	return 0;
}




