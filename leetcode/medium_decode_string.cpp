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
	bool isnum(char c) {
		return c >= '0' && c <= '9';
	}
	bool isabc(char c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
	}
	bool isrightbracket(char c) {
		return c == ']';
	}
	int decodeCount(const string& s, int& idx) {
		int cnt = 0;
		for (; idx < s.size(); ++idx) {
			if (isnum(s[idx])) {
				cnt *= 10;
				cnt += (s[idx] - '0');
			}
			else {
				break;
			}
		}
		return cnt;
	}
	string decodePartialString(const string& s, int& idx) {
		stringstream ss;
		while (idx < s.size() && isabc(s[idx])) {
			ss << s[idx];
			idx += 1;
		}
		return ss.str();
	}
public:
	string decodeString(string s) {
		stringstream ss;
		stack<int> cnts;
		stack<string> innerstrs;
		int i = 0;
		while (i < s.size()) {
			char c = s[i];
			if (isnum(c)) {
				int cnt = decodeCount(s, i);
				cnts.push(cnt);
				i += 1; // pass '['
				string part = decodePartialString(s, i);
				innerstrs.push(part);
			}
			else if (isabc(c)) {
				string part = decodePartialString(s, i);
				if (!innerstrs.empty()) {
					innerstrs.top().append(part);
				}
				else {
					ss << part;
				}
			}
			else if (isrightbracket(c)) {
				int cnt = cnts.top();
				string is = innerstrs.top();
				cnts.pop();
				innerstrs.pop();
				stringstream sss;
				for (int j = 0; j < cnt; ++j) sss << is;
				i += 1;
				if (!innerstrs.empty()) {
					innerstrs.top().append(sss.str());
				}
				else {
					ss << sss.str();
				}
			}
		}
		return ss.str();
	}
};

int main()
{
	cout << boolalpha << (Solution().decodeString("3[a]2[bc]") == "aaabcbc") << endl;
	cout << boolalpha << (Solution().decodeString("3[a2[c]]") == "accaccacc") << endl;
	cout << boolalpha << (Solution().decodeString("2[abc]3[cd]ef") == "abcabccdcdcdef") << endl;
	cout << boolalpha << (Solution().decodeString("sd2[f2[e]g]i") == "sdfeegfeegi") << endl;
	
	std::cin.get();
	return 0;
}



