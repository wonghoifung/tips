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
	vector<int> diffWaysToCompute(string input) {
		vector<int> res;
		if (input.empty()) return res;
		for (size_t i = 0; i < input.size(); ++i)
		{
			char c = input[i];
			if (c == '+' || c == '-' || c == '*')
			{
				string a = input.substr(0, i);
				string b = input.substr(i + 1);
				vector<int> ares = diffWaysToCompute(a);
				vector<int> bres = diffWaysToCompute(b);
				for (int x : ares)
				{
					for (int y : bres)
					{
						switch (c)
						{
						case '+': res.push_back(x + y); break;
						case '-': res.push_back(x - y); break;
						case '*': res.push_back(x * y); break;
						}
					}
				}
			}
		}
		if (res.empty()) res.push_back(stoi(input));
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<int> res = solu.diffWaysToCompute("2*3-4*5");
		copy(res.begin(), res.end(), ostream_iterator<int>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}




