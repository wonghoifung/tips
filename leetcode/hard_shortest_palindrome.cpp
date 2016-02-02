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
	string shortestPalindrome(string s) { // KMP
		if (s.size() <= 1) return s;
		string tmp = s;
		reverse(tmp.begin(), tmp.end());
		string news = s + "#" + tmp;
		vector<int> pmt(news.size(), 0);
		for (int i = 1; i < pmt.size(); ++i)
		{
			int len = pmt[i - 1];
			while (len > 0 && news[len] != news[i]) len = pmt[len - 1];
			pmt[i] = len + ((news[len] == news[i]) ? 1 : 0);
		}
		tmp = s.substr(pmt[pmt.size() - 1]);
		reverse(tmp.begin(), tmp.end());
		return tmp + s;
	}
};

int main()
{
	Solution s;
	{
		cout << s.shortestPalindrome("aacecaaa") << endl;
		cout << s.shortestPalindrome("abcd") << endl;
		cout << s.shortestPalindrome("aaaa") << endl;
	}

	std::cin.get();
	return 0;
}




