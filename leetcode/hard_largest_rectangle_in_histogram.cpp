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
	void printStack(stack<int> s) {
		cout << "current stack: ";
		while (s.size())
		{
			cout << s.top() << " ";
			s.pop();
		}
		cout << endl;
	}
	int largestRectangleArea(vector<int>& height) {
		int n = height.size();
		stack<int> s;
		int maxa = 0;
		int tp;
		int tpa;
		int i = 0;
		while (i < n)
		{
			cout << "i:" << i << ", height[i]:" << height[i] << endl;
			if (s.empty() || height[s.top()] <= height[i]) s.push(i++), cout << "push" << endl;
			else {
				tp = s.top();
				s.pop();
				cout << " tp:" << tp << ", height[tp]:" << height[tp] << endl;
				if (!s.empty()) cout << " A " << " i:" << i << ", mul:" << i - s.top() - 1 << endl;
				else cout << " B mul:" << i << endl;
				tpa = height[tp] * (s.empty() ? i : i - s.top() - 1);
				maxa = max(maxa, tpa);
				cout << "tpa:" << tpa << ", maxa:" << maxa << endl;
			}
			printStack(s);
			cout << endl;
		}
		cout << endl;
		while (!s.empty())
		{
			tp = s.top();
			s.pop();
			cout << " tp:" << tp << ", height[tp]:" << height[tp] << endl;
			if (!s.empty()) cout << " A" << " i:" << i << ", mul:" << i - s.top() - 1 << endl;
			else cout << " B mul:" << i << endl;
			tpa = height[tp] * (s.empty() ? i : i - s.top() - 1);
			maxa = max(maxa, tpa);
			cout << "tpa:" << tpa << ", maxa:" << maxa << endl;
			printStack(s);
			cout << endl;
		}
		return maxa;
	}
};

int main()
{
	Solution s;
	{
		vector<int> height{ 6, 2, 5, 5, 4, 5, 1, 6 };
		cout << s.largestRectangleArea(height) << endl;
	}

	std::cin.get();
	return 0;
}




