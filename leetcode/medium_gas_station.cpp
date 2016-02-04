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
	int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
		int size = gas.size();
		int sum = 0;
		int start = 0;
		int total = 0;
		for (int i = 0; i < size; ++i) {
			sum += (gas[i] - cost[i]);
			if (sum < 0) {
				total += sum;
				sum = 0;
				start = i + 1;
			}
		}
		total += sum;
		return total < 0 ? -1 : start;
	}
};

int main()
{
	Solution s;
	{
		vector<int> gas{ 6, 1, 4, 3, 5 };
		vector<int> cost{ 3, 8, 2, 4, 2 };
		cout << s.canCompleteCircuit(gas, cost) << endl;
	}

	std::cin.get();
	return 0;
}




