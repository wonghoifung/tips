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
#include <sstream>
#include <algorithm>
#include <bitset>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(NULL);
	}
	~mytimer()
	{
		time_t end = time(NULL);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	typedef pair<int, int> pos_t;
	vector<set<pos_t>> lands;
	set<pos_t> posBeenFound;

	void findLand(pos_t& pos, int w, int h, vector<vector<char>>& gird, set<pos_t>& land) {
		posBeenFound.insert(pos);
		if (pos.first > 0) // not top most
		{
			if (gird[pos.first - 1][pos.second] == '1')
			{
				pos_t np = { pos.first - 1, pos.second };
				land.insert(np);
				if (posBeenFound.find(np) == posBeenFound.end())
				{
					findLand(np, w, h, gird, land);
				}
			}
		}
		if (pos.second > 0) // not left most
		{
			if (gird[pos.first][pos.second - 1] == '1')
			{
				pos_t np = { pos.first, pos.second - 1 };
				land.insert(np);
				if (posBeenFound.find(np) == posBeenFound.end())
				{
					findLand(np, w, h, gird, land);
				}
			}
		}
		if (pos.first < h - 1) // not bottom most
		{
			if (gird[pos.first + 1][pos.second] == '1')
			{
				pos_t np = { pos.first + 1, pos.second };
				land.insert(np);
				if (posBeenFound.find(np) == posBeenFound.end())
				{
					findLand(np, w, h, gird, land);
				}
			}
		}
		if (pos.second < w - 1) // not right most
		{
			if (gird[pos.first][pos.second + 1] == '1')
			{
				pos_t np = { pos.first, pos.second + 1 };
				land.insert(np);
				if (posBeenFound.find(np) == posBeenFound.end())
				{
					findLand(np, w, h, gird, land);
				}
			}
		}
	}
	int numIslands(vector<vector<char>>& grid) {
		if (grid.empty())
		{
			return 0;
		}
		int h = grid.size();
		int w = grid[0].size();
		for (size_t i = 0; i < h; ++i)
		{
			for (size_t j = 0; j < w; ++j)
			{
				if (grid[i][j] == '1')
				{
					pos_t p = { i, j };
					if (posBeenFound.find(p) == posBeenFound.end())
					{
						lands.push_back(set<pos_t>());
						findLand(p, w, h, grid, lands.back());
					}
				}
			}
		}
		return lands.size();
	}
};

int main()
{
	Solution s;
	{

	}
	std::cin.get();

	return 0;
}

