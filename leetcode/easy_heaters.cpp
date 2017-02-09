#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
public:
	int findRadius(vector<int>& houses, vector<int>& heaters) {
		sort(houses.begin(), houses.end());
		sort(heaters.begin(), heaters.end());
		vector<int> res(houses.size(), INT_MAX);
		for (int i = 0, h = 0; i < houses.size() && h < heaters.size();) {
			if (houses[i] <= heaters[h]) {
				res[i] = heaters[h] - houses[i];
				i += 1;
			}
			else {
				h += 1;
			}
		}
		for (int i = houses.size() - 1, h = heaters.size() - 1; i >= 0 && h >= 0;) {
			if (houses[i] >= heaters[h]) {
				res[i] = min(houses[i] - heaters[h], res[i]);
				i -= 1;
			}
			else {
				h -= 1;
			}
		}
		return *max_element(res.begin(), res.end());
	}
};

int main() {
	{
		vector<int> houses{ 1, 2, 3 }, heaters{ 2 };
		cout << Solution().findRadius(houses, heaters) << endl; // 1
	}
	{
		vector<int> houses{ 1, 2, 3, 4 }, heaters{ 1, 4 };
		cout << Solution().findRadius(houses, heaters) << endl; // 1
	}

	cin.get();
}

