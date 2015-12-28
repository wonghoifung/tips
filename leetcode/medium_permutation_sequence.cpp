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
	int factorial(int n) {
		int res = 1;
		for (int i = 2; i <= n; ++i) res *= i;
		return res;
	}
	string getPermutation(int n, int k) {
		string s = string("123456789").substr(0, n);
		string res(n, '*');
		for (int i = 0; i < n; ++i)
		{
			int f = factorial(s.size() - 1); // cout << "f:" << f << " ";
			int idx = ceil((double)k / f) - 1; // cout << "idx:" << idx << " ";
			char c = s[idx]; // cout << "c:" << c << endl;
			s.erase(idx, 1);
			k -= (idx * f); 

			res[i] = c;
		}
		return res;
	}

	string toString(vector<int>& v) {
		string res;
		for (size_t i = 0; i < v.size(); ++i)
		{
			char c = '0' + v[i];
			res += c;
		}
		return res;
	}
	string getPermutationSTL(int n, int k) {
		vector<int> arr;
		for (size_t i = 1; i <= n; ++i)
		{
			arr.push_back(i);
		}
		while (k-- > 1)
		{
			next_permutation(arr.begin(), arr.end());
		}
		return toString(arr);
	}
};

int main()
{
	Solution solu;
	{
		cout << solu.getPermutation(4, 17) << endl;
	}

	std::cin.get();
	return 0;
}

