#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	int findMaximizedCapital(int k, int W, vector<int>& Profits, vector<int>& Capital) {
		priority_queue<int> low;
		multiset<pair<int, int>> high;
		for (int i = 0; i < Profits.size(); ++i) {
			if (Profits[i] > 0) {
				if (Capital[i] <= W) low.push(Profits[i]);
				else high.emplace(Capital[i], Profits[i]);
			}
		}
		while (k-- && low.size()) {
			W += low.top(), low.pop();
			for (auto i = high.begin(); high.size() && i->first <= W; i = high.erase(i))
				low.push(i->second);
		}
		return W;
	}
};

int main() {
	{
		vector<int> profits{ 1, 2, 3 };
		vector<int> capital{ 0, 1, 1 };
		cout << Solution().findMaximizedCapital(2, 0, profits, capital) << endl; // 4
	}
	cin.get();
}

