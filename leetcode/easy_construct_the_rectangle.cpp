#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>s
#include <algorithm>
#include <numeric>
#include <stdint.h>

using namespace std;

class Solution {
public:
	vector<int> constructRectangle(int area) {
		int sr = sqrt(area);
		cout << sr << endl;
		while (area % sr != 0) sr -= 1;
		return { area / sr, sr };
	}
};

int main() {
	{
		auto res = Solution().constructRectangle(4);
		for (auto i : res) {
			cout << i << " ";
		}
		cout << endl;
	}

	cin.get();
}

