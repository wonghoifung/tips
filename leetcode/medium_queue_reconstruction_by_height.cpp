#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

class Solution {
public:
	vector<pair<int, int>> reconstructQueue(vector<pair<int, int>>& people) {
		sort(people.begin(), people.end(), [](const pair<int, int>& p1, const pair<int, int>& p2) {
			return p1.first > p2.first || (p1.first == p2.first && p1.second < p2.second);
		});
		vector<pair<int, int>> res;
		for (auto& p : people) {
			res.insert(res.begin() + p.second, p);
		}
		return res;
	}
};

int main() {
	{
		vector<pair<int, int>> people = { { 7, 0 }, { 4, 4 }, { 7, 1 }, { 5, 0 }, { 6, 1 }, { 5, 2 } };
		auto output = Solution().reconstructQueue(people); // [[5,0], [7,0], [5,2], [6,1], [4,4], [7,1]]
		for (auto& p : output) {
			cout << p.first << ", " << p.second << endl;
		}
	}
	cin.get();
	return EXIT_SUCCESS;
}

