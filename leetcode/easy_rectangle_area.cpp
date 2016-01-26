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

using namespace std;

class Solution {
public:
	int computeArea(int A, int B, int C, int D, int E, int F, int G, int H) {
		int a1 = (C - A) * (D - B);
		int a2 = (G - E) * (H - F);
		int overlap = (min(C, G) - max(A, E)) * (min(D, H) - max(B, F));
		if (min(C, G) <= max(A, E) || min(D, H) <= max(B, F)) overlap = 0;
		return a1 + a2 - overlap;
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




