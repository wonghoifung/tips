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

class Solution {
private:
	bool isVowel(char c) {
		static char vowels[] = { 'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U' };
		static int cnt = sizeof(vowels) / sizeof(vowels[0]);
		for (int i = 0; i < cnt; ++i) {
			if (vowels[i] == c) return true;
		}
		return false;
	}
public:
	string reverseVowels(string s) {
		if (s.empty()) return s;
		vector<char> letters(s.size());
		int beg = 0, end = s.size() - 1;
		while (1) {
			while (beg < end && !isVowel(s[beg])) {
				letters[beg] = s[beg];
				beg += 1;
			}
			while (beg < end && !isVowel(s[end])) {
				letters[end] = s[end];
				end -= 1;
			}
			if (beg < end) {
				letters[beg] = s[end];
				letters[end] = s[beg];
				beg += 1;
				end -= 1;
			}
			else if (beg == end) {
				letters[beg] = s[beg];
				break;
			}
			else {
				break;
			}
		}
		return string(letters.begin(), letters.end());
	}
};

int main()
{
	Solution s;
	{
		string ss = "hello";
		cout << s.reverseVowels(ss) << endl;
	}
	{
		string ss = "leetcode";
		cout << s.reverseVowels(ss) << endl;
	}
	{
		string ss = ".,";
		cout << s.reverseVowels(ss) << endl;
	}
	{
		string ss = " ";
		cout << s.reverseVowels(ss) << endl;
	}
	{
		string ss = "a.";
		cout << s.reverseVowels(ss) << endl;
	}
	{
		string ss = "a a";
		cout << s.reverseVowels(ss) << endl;
	}
	{
		string ss = "ai";
		cout << s.reverseVowels(ss) << endl;
	}

	std::cin.get();
	return 0;
}




