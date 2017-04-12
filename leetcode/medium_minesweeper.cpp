#include <string.h>
#include <iostream>
#include <vector>
using namespace std;
template <typename VV>
void printVV(const VV& res, const char* tag, int id) {
	cout << tag << " " << id << endl;
	cout << "-----------" << endl;
	for (auto i=0; i<res.size(); ++i) {
		for (auto j=0; j<res[i].size(); ++j) {
			cout << res[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
static int sid = 1;
class Solution {
	int adjMineCount(const vector<int>& pos, const vector<vector<char>>& board, vector<vector<int>>& blanks) {
		int h = board.size();
		int w = board[0].size();    
		int count = 0;
		vector<vector<int>> adjs = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
		for (auto i=0; i<adjs.size(); ++i) {
			int x = pos[0] + adjs[i][0];
			int y = pos[1] + adjs[i][1];
			if (x < h && x >= 0 && y < w && y >= 0) {
				if (board[x][y] == 'M')	count += 1;
				else if (board[x][y] == 'E') blanks.push_back({x,y});
			}
		}
		return count;
	}
	bool helper(vector<vector<char>>& res, vector<vector<char>>& board, vector<int> click) {
		int x = click[0];
		int y = click[1];
		if (res[x][y]=='M') {
			res[x][y]='X';
			return true;
		}
		else if (res[x][y]=='E') {
			vector<vector<int>> blanks;
			int amc = adjMineCount(click, board, blanks);
			if (amc == 0) {
				res[x][y] = 'B';
				for (auto i=0; i<blanks.size(); ++i) {
					auto c = blanks[i];
					if (helper(res, board, c)) return true;
				}
			}
			else {
				res[x][y] = '0' + amc;
			}
		}
		return false;
	}
public:
    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
		auto res = board;
		helper(res, board, click);
		return res;
    }
};
vector<char> toVector(const char* s) {
	auto len = strlen(s);
	vector<char> res; res.reserve(len);
	for (auto i=0; i<len; ++i) res.push_back(s[i]);
	return res;
}
int main() {
	{
	vector<vector<char>> board = {toVector("EEEEE"),toVector("EEMEE"),toVector("EEEEE"),toVector("EEEEE")};
	vector<int> click = {3,0};
	auto res = Solution().updateBoard(board, click);
	printVV(res, "res", 0);
	}
	/*{
	vector<vector<char>> board = {toVector("BBBBBB1E"),toVector("B111BB1M"),toVector("12M1BB11"),toVector("M211BBBB"),toVector("11BBBBBB"),toVector("BBBBBBBB"),toVector("B1221BBB"),toVector("B1MM1BBB")};
	vector<int> click = {0,7};
	auto res = Solution().updateBoard(board, click);
	printVV(res, "res", 0);
	}*/
	/*{
	vector<vector<char>> board = {toVector("MEM"),toVector("EME"),toVector("MEM")};
	vector<int> click = {0,1};
	auto res = Solution().updateBoard(board, click);
	printVV(res, "res", 0);
	}*/
}

