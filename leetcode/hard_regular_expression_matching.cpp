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
	bool isMatch(string s, string p) {
		int slen = s.size();
		int plen = p.size();
		vector<vector<char>> matrix(slen + 1, vector<char>(plen + 1, false));
		matrix[0][0] = true;

		for (int i = 0; i <= slen; ++i)
		{
			for (int j = 1; j <= plen; ++j)
			{
				char pcurrent = p.at(j - 1);
				char scurrent = s.at(i - 1);
				if (pcurrent == '*')
				{
					char plast = p.at(j - 2);
					/* empty or match seq */
					matrix[i][j] = (j > 1 && matrix[i][j - 2]) || (i > 0 && matrix[i - 1][j] && (plast == '.' || plast == scurrent));
				}
				else
				{
					matrix[i][j] = i > 0 && matrix[i - 1][j - 1] && (pcurrent == '.' || scurrent == pcurrent);
				}
			}
		}
		return matrix[slen][plen];
	}
};

int main()
{
	Solution solu;
	{

	}

	std::cin.get();
	return 0;
}

