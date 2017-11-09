#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
	int areaOfIsland(vector<vector<int> >& grid, int i, int j) {
		if (i >= 0 && i < grid.size() && j >= 0 && j < grid[0].size() && grid[i][j] == 1) {
			grid[i][j] = 0;
			return 1 + areaOfIsland(grid, i+1, j) + 
                       areaOfIsland(grid, i-1, j) +
                       areaOfIsland(grid, i, j-1) +
                       areaOfIsland(grid, i, j+1);
		}
		return 0;
	}
public:
    int maxAreaOfIsland(vector<vector<int> >& grid) {
		int maxArea = 0;
		for (int i=0; i<grid.size(); ++i) {
			for (int j=0; j<grid[i].size(); ++j) {
				if (grid[i][j]==1) maxArea = max(maxArea, areaOfIsland(grid,i,j));
			}
		}
		return maxArea;
    }
};

int main() {
/*
	vector<vector<int> > grid = {{1,1,0,0,0},
                                 {1,1,0,0,0},
                                 {0,0,0,1,1},
                                 {0,0,0,1,1}};
	cout << Solution().maxAreaOfIsland(grid) << endl;
*/
	vector<vector<int> > grid = {{1,1},
                                 {1,0}};
	cout << Solution().maxAreaOfIsland(grid) << endl;
}

