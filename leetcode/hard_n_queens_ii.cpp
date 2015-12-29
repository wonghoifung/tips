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

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(nullptr);
	}
	~mytimer()
	{
		time_t end = time(nullptr);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	int N;
	vector<bool> rowflag;
	vector<bool> colflag;
	vector<bool> rightUp;
	vector<bool> rightDown;
	void initBoard(int n) {
		N = n;
		rowflag.resize(n, false);
		colflag.resize(n, false);
		int hypotCnt = (n - 1) + (n - 1) + 1;
		rightUp.resize(hypotCnt, false);
		rightDown.resize(hypotCnt, false);
	}
	bool isValid(int row, int col) {
		if (rowflag[row]) return false;
		if (colflag[col]) return false;
		if (rightUp[row + col]) return false;
		if (rightDown[col - row + N - 1]) return false;
		return true;
	}
	void totalNQueensHelper(int rownum, int& res) {
		if (rownum == N)
		{
			res += 1;
			return;
		}
		for (int i = 0; i < N; ++i)
		{
			if (isValid(rownum, i))
			{
				rowflag[rownum] = true; colflag[i] = true; 
				rightUp[rownum + i] = true; rightDown[i - rownum + N - 1] = true;
				totalNQueensHelper(rownum + 1, res);
				rowflag[rownum] = false; colflag[i] = false;
				rightUp[rownum + i] = false; rightDown[i - rownum + N - 1] = false;
			}
		}
	}
	int totalNQueens(int n) {
		int res = 0;
		initBoard(n);
		totalNQueensHelper(0, res);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		cout << solu.totalNQueens(5) << endl;
	}

	std::cin.get();
	return 0;
}

