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

class NestedInteger {
public:
	NestedInteger() :isint(false) {

	}
	NestedInteger(int value) :isint(true), ivalue(value) {

	}
	bool isInteger() const {
		return isint;
	}
	int getInteger() const {
		if (isint) return ivalue;
		cout << "undefined, not a single integer" << endl;
		exit(1);
	}
	void setInteger(int value) {
		isint = true;
		ivalue = value;
		ilist.clear();
	}
	void add(const NestedInteger& ni) {
		isint = false;
		ilist.push_back(ni);
	}
	const vector<NestedInteger>& getList() const {
		if (!isint) return ilist;
		cout << "undefined, not a nested list" << endl;
		exit(1);
	}
private:
	bool isint;
	int ivalue;
	vector<NestedInteger> ilist;
};

class Solution {
	bool isDigit(char c) {
		return (c >= '0' && c <= '9') || (c == '-');
	}
	NestedInteger getInt(const string& s, int& idx) {
		int fac = 1;
		if (s[idx] == '-') {
			idx += 1;
			fac = -1;
		}
		int i = s[idx] - '0';
		idx += 1;
		while (idx < s.size() && isDigit(s[idx])) {
			i *= 10;
			i += (s[idx] - '0');
			idx += 1;
		}
		return i * fac;
	}
	NestedInteger getList(const string& s, int& idx) {
		NestedInteger lni;
		idx += 1;
		while (idx < s.size()) {
			if (isDigit(s[idx])) {
				NestedInteger ni = getInt(s, idx);
				lni.add(ni);
				continue;
			}
			if (s[idx] == ',') {
				idx += 1;
				continue;
			}
			if (s[idx] == '[') {
				NestedInteger slni = getList(s, idx);
				lni.add(slni);
				continue;
			}
			if (s[idx] == ']') {
				idx += 1;
				break;
			}
		}
		return lni;
	}
public:
	NestedInteger deserialize(string s) {
		NestedInteger res;
		int idx = 0;
		if (s[idx] == '[') {
			NestedInteger lni = getList(s, idx);
			if (idx >= s.size()) return lni;
			res.add(lni);
		}
		else {
			NestedInteger ni = getInt(s, idx);
			if (idx >= s.size()) return ni;
			res.add(ni);
		}
		while (idx < s.size()) {
			if (s[idx] == '[') {
				NestedInteger lni = getList(s, idx);
				res.add(lni);
				continue;
			}
			if (isDigit(s[idx])) {
				NestedInteger ni = getInt(s, idx);
				res.add(ni);
				continue;
			}
			idx += 1;
		}
		return res;
	}
};

int main()
{
	std::function<void(const NestedInteger&, bool)> print;
	print = [&print](const NestedInteger& ni, bool newline) {
		if (ni.isInteger()) {
			cout << ni.getInteger() << " ";
		}
		else {
			cout << '[';
			auto v = ni.getList();
			for (auto i = 0; i < v.size(); ++i) {
				const NestedInteger& sni = (const NestedInteger&)(v[i]);
				print(sni, false);
			}
			cout << ']';
		}
		if (newline) cout << endl;
	};
	string s("324");
	print(Solution().deserialize(s), true);
	s = "[123,[456,[789]]]";
	print(Solution().deserialize(s), true);
	
	std::cin.get();
	return 0;
}



