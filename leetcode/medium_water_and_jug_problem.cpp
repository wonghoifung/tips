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

using namespace std;
/*
 * Bézout's identity (also called Bézout's lemma) is a theorem in the elementary theory of numbers:
 * let a and b be nonzero integers and let d be their greatest common divisor. Then there exist integers x and y such that ax+by=d
 * In addition, the greatest common divisor d is the smallest positive integer that can be written as ax + by
 * every integer of the form ax + by is a multiple of the greatest common divisor d.
 * */
class Solution {
	int gcd(int a, int b) {
		while (b) {
			int tmp = b;
			b = a % b;
			a = tmp;
		}
		return a;
	}
public:
	bool canMeasureWater(int x, int y, int z) { // Bézout's identity
		if (x + y < z) return false;
		if (x == z || y == z || x + y == z) return true;
		return z % gcd(x, y) == 0;
	}
};

int main()
{
	cout << boolalpha << Solution().canMeasureWater(3, 5, 4) << endl; // true
	cout << boolalpha << Solution().canMeasureWater(2, 6, 5) << endl; // false

	std::cin.get();
	return 0;
}




