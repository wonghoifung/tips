#include <iostream>
#include <sstream>
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
	int findPoisonedDuration(vector<int>& timeSeries, int duration) {
		int result = 0;
		int pre = INT_MIN;
		for (int t : timeSeries) {
			int end = t + duration;
			result += end - max(t, pre);
			pre = end;
		}
		return result;
	}
};

int main() {
	{
		vector<int> timeSeries{ 1, 4 };
		cout << Solution().findPoisonedDuration(timeSeries, 2) << endl; // 4
	}
	{
		vector<int> timeSeries{ 1, 2 };
		cout << Solution().findPoisonedDuration(timeSeries, 2) << endl; // 3
	}
	{
		vector<int> timeSeries{ 1, 2, 3, 4, 5 };
		cout << Solution().findPoisonedDuration(timeSeries, 5) << endl; // 9
	}

	cin.get();
}

