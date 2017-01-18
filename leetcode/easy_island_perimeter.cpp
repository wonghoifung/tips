#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
	int islandPerimeter(vector<vector<int>>& grid) {
		int perimeter = 0;
		for (int i = 0; i < grid.size(); ++i) {
			for (int j = 0; j < grid[0].size(); ++j) {
				if (grid[i][j] == 1) {
					perimeter += 4;
					if (i > 0 && grid[i - 1][j] == 1) perimeter -= 1;
					if (i < grid.size() - 1 && grid[i + 1][j] == 1) perimeter -= 1;
					if (j > 0 && grid[i][j - 1] == 1) perimeter -= 1;
					if (j < grid[0].size() - 1 && grid[i][j + 1] == 1) perimeter -= 1;
				}
			}
		}
		return perimeter;
	}
};

int main() {
	vector<vector<int>> grid{ 
		{ 0, 1, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 1, 0, 0 },
		{ 1, 1, 0, 0 } };
	cout << Solution().islandPerimeter(grid) << endl; // 16

	cin.get();
}

