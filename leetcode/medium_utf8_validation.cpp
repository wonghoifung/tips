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
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

class Solution {
	bool isBitZero(int i, int idx) { return ((i >> idx) & 1) == 0; }
	int countBit(int i) 
	{
		if (isBitZero(i, 7)) return 1;
		if (isBitZero(i, 6)) return -1; // invalid
		if (isBitZero(i, 5)) return 2;
		if (isBitZero(i, 4)) return 3;
		if (isBitZero(i, 3)) return 4;
		return -1; // invalid
	}
	bool byteValid(int b) { return (b & 128) && isBitZero(b, 6); }
public:
	bool validUtf8(vector<int>& data) 
	{
		int i = 0;
		while (i < data.size())
		{
			int byte = data[i];
			int cnt = countBit(byte);
			if (cnt == -1) return false;
			for (int j = 1; j <= cnt - 1; j++)
			{
				if (i + j >= data.size()) return false;
				if (!byteValid(data[i + j])) return false;
			}
			i += cnt;
		}
		return true;
	}
};

int main()
{
	{
		vector<int> data = { 197, 130, 1 };
		cout << boolalpha << Solution().validUtf8(data) << endl; // true
	}

	{
		vector<int> data = { 235, 140, 4 };
		cout << boolalpha << Solution().validUtf8(data) << endl; // false
	}
	
	std::cin.get();
	return 0;
}



