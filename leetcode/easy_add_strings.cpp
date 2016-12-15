#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class Solution {
public:
	string addStrings(string num1, string num2) {
		int carry = 0;
		string* shorter = NULL;
		string* longer = NULL;
		int len = 0;
		if (num1.size() > num2.size()) {
			shorter = &num2;
			longer = &num1;
			len = shorter->size();
		}
		else {
			shorter = &num1;
			longer = &num2;
			len = shorter->size();
		}
		stringstream ss;
		for (int i = 0; i < len; ++i) {
			int n1 = shorter->operator[](shorter->size() - (i + 1)) - '0';
			int n2 = longer->operator[](longer->size() - (i + 1)) - '0';
			int s = n1 + n2 + carry;
			carry = s / 10;
			ss << (s % 10);
		}
		for (int i = len; i < longer->size(); ++i) {
			int n2 = longer->operator[](longer->size() - (i + 1)) - '0';
			int s = n2 + carry;
			carry = s / 10;
			ss << (s % 10);
		}
		if (carry) ss << 1;
		string res = ss.str();
		reverse(res.begin(), res.end());
		return res;
	}
};

int main() {


	cin.get();
	return EXIT_SUCCESS;
}

