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
	struct token
	{
		bool isoperator;
		union {
			int voperand;
			char voperator;
		} val;
		token(int v, bool b) { val.voperand = v; isoperator = b; }
	};
	int eval(vector<token>& tokens) {
		vector<token> s;
		for (size_t i = 0; i < tokens.size(); ++i)
		{
			if (tokens[i].isoperator && tokens[i].val.voperator == '*')
			{
				auto& n = s.back();
				n.val.voperand *= tokens[++i].val.voperand;
				continue;
			}
			s.push_back(tokens[i]);
		}
		int res = s[0].val.voperand;
		for (size_t i = 1; i < s.size(); ++i)
		{
			if (s[i].isoperator)
			{
				if (s[i].val.voperator == '+')
				{
					res += s[++i].val.voperand;
				}
				else
				{
					res -= s[++i].val.voperand;
				}
			}
		}
		return res;
	}
	void addOperatorsHelper(vector<token>& currenttokens, string left, string expr, vector<string>& res, int target) {
		if (eval(currenttokens) == target && left.empty() && expr.size()) { res.push_back(expr); return; }
		if (left.empty()) return;
		int operand = 0;
		for (size_t i = 0; i < left.size(); ++i)
		{
			operand = operand * 10 + left[i] - '0';

			currenttokens.push_back({ '+', true });
			currenttokens.push_back({ operand, false });
			addOperatorsHelper(currenttokens, left.substr(i + 1), expr + "+" + left.substr(0, i + 1), res, target);
			currenttokens.pop_back();
			currenttokens.pop_back();

			currenttokens.push_back({ '-', true });
			currenttokens.push_back({ operand, false });
			addOperatorsHelper(currenttokens, left.substr(i + 1), expr + "-" + left.substr(0, i + 1), res, target);
			currenttokens.pop_back();
			currenttokens.pop_back();

			currenttokens.push_back({ '*', true });
			currenttokens.push_back({ operand, false });
			addOperatorsHelper(currenttokens, left.substr(i + 1), expr + "*" + left.substr(0, i + 1), res, target);
			currenttokens.pop_back();
			currenttokens.pop_back();

			if (left[0] == '0') break;
		}
	}
	vector<string> addOperatorsSlow(string num, int target) {
		vector<string> res;
		int operand = 0;
		vector<token> currenttokens;
		for (size_t i = 0; i < num.size(); ++i)
		{
			operand = operand * 10 + num[i] - '0';

			currenttokens.push_back({ operand, false });
			addOperatorsHelper(currenttokens, num.substr(i + 1), num.substr(0, i + 1), res, target);
			currenttokens.pop_back();

			if (num[0] == '0') break;
		}
		return res;
	}

	vector<string> addOperators(string num, int target) {
		vector<string> res;
		if (num.empty()) return res;
		dfs(res, "", num, target, 0, 0, 0);
		return res;
	}
	void dfs(vector<string>& res, const string& path, const string& num, int target, int pos, long eval, long multed) {
		if (pos == num.size()) {
			if (target == eval) res.push_back(path);
			return;
		}
		for (int i = pos; i < num.size(); ++i) {
			string scur = num.substr(pos, i - pos +1);
			long cur = stol(scur);
			if (pos == 0)
			{
				dfs(res, path + scur, num, target, i + 1, cur, cur);
			}
			else
			{
				dfs(res, path + "+" + scur, num, target, i + 1, eval + cur, cur);
				dfs(res, path + "-" + scur, num, target, i + 1, eval - cur, -cur);
				dfs(res, path + "*" + scur, num, target, i + 1, eval - multed + multed * cur, multed * cur);
			}
			if (num.at(pos) == '0') break;
		}
	}
};

int main()
{
	Solution solu;
	{
		vector<string> res = solu.addOperators("123", 6);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n"));
		cout << endl;
	}
	{
		vector<string> res = solu.addOperators("232", 8);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n"));
		cout << endl;
	}
	{
		vector<string> res = solu.addOperators("105", 5);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n"));
		cout << endl;
	}
	{
		vector<string> res = solu.addOperators("00", 0);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n"));
		cout << endl;
	}
	{
		vector<string> res = solu.addOperators("123456789", 45);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n"));
		cout << endl;
	}

	std::cin.get();
	return 0;
}




