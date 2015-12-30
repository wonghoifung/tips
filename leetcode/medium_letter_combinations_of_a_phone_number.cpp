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
	void letterCombinationsHelper(const unordered_map<char, string>& m, const string& digits, const string& current, vector<string>& res) {
		if (digits.empty()) {
			res.push_back(current); 
			return;
		}
		char c = digits[0];
		auto it = m.find(c);
		if (it == m.end()) return;
		const string& s = it->second;
		for (string::size_type i = 0; i < s.size(); ++i)
		{
			string newcurrent = current + s[i];
			letterCombinationsHelper(m, digits.substr(1), newcurrent, res);
		}
	}
	vector<string> letterCombinations(string digits) {
		unordered_map<char, string> m{ { '2', "abc" }, { '3', "def" }, { '4', "ghi" }, { '5', "jkl" }, 
									   { '6', "mno" }, { '7', "pqrs" }, { '8', "tuv" }, { '9', "wxyz" } };
		vector<string> res;
		if (digits.empty()) return res;
		letterCombinationsHelper(m, digits, "", res);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		vector<string> res = solu.letterCombinations("23");
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}




