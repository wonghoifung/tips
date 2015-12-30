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
	void flipBit(int& n, int pos) {
		if (n & (1 << pos)) {
			n &= ~(1 << pos);
		}
		else {
			n |= (1 << pos);
		}
	}
	vector<int> grayCode(int n) {
		vector<int> res;
		int num = 0;
		res.push_back(num);
		vector<bool> flag(pow(2, n), false);
		flag[num] = true;
		bool change = true;
		while (change)
		{
			change = false;
			for (int i = 0; i < n; i++)
			{
				flipBit(num, i);
				if (!flag[num])
				{
					res.push_back(num);
					flag[num] = true;
					change = true;
					break;
				}
				else
				{
					flipBit(num, i);
				}
			}
		}
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<int> res = solu.grayCode(3);
		for (int i : res) {
			cout << i << ": " << bitset<3>(i) << endl;
		}
	}

	std::cin.get();
	return 0;
}




