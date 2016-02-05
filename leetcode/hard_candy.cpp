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
	int candy(vector<int>& ratings) {
		if (ratings.empty()) return 0;
		if (ratings.size() == 1) return 1;
		vector<int> candys(ratings.size(), 1);
		for (int i = 1; i < ratings.size(); ++i)
		{
			if (ratings[i] > ratings[i - 1]) candys[i] = candys[i - 1] + 1;
		}
		for (int i = ratings.size() - 2; i >= 0; --i)
		{
			if (ratings[i] > ratings[i + 1]) {
				if (candys[i] <= candys[i + 1]) candys[i] = candys[i + 1] + 1;
			}
		}
		int sum = 0;
		sum = accumulate(candys.begin(), candys.end(), sum);
		return sum;
	}
};

int main()
{
	Solution s;
	{
		vector<int> ratings{ 1, 2, 2 };
		cout << s.candy(ratings) << endl;
	}

	std::cin.get();
	return 0;
}




