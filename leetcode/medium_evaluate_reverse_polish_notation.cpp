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
	bool isOperator(const string& s) {
		return s == "+" || s == "-" || s == "*" || s == "/";
	}
	int evalOperator(int op1, int op2, char o) {
		switch (o)
		{
		case '+': return op1 + op2;
		case '-': return op1 - op2;
		case '*': return op1 * op2;
		case '/': return op1 / op2;
		default:
			assert(0);
		}
	}
	int evalRPN(vector<string>& tokens) {
		stack<int> oprands;
		for (int i = 0; i < tokens.size(); ++i)
		{
			if (isOperator(tokens[i])) {
				int oprand2 = oprands.top();
				oprands.pop();
				int oprand1 = oprands.top();
				oprands.pop();
				oprands.push(evalOperator(oprand1, oprand2, tokens[i][0]));
			}
			else {
				oprands.push(stoi(tokens[i]));
			}
		}
		return oprands.top();
	}
};

int main()
{

	std::cin.get();
	return 0;
}




