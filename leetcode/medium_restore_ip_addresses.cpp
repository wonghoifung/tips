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
	void restoreIpAddressesHelper(const string& s, int start, int ipIdx, const string& current, vector<string>& res, unordered_set<string>& exist) {
		if (start >= s.size()) return;

		if (ipIdx == 3)
		{
			string n = s.substr(start);
			if (stoi(n) <= 255)
			{
				if (n.size() > 1 && n.at(0) == '0') return;
				string ip = current + "." + n;
				if (exist.find(ip) == exist.end())
				{
					res.push_back(ip);
					exist.insert(ip);
				}
			}
			return;
		}

		string n1 = s.substr(start, 1);
		restoreIpAddressesHelper(s, start + 1, ipIdx + 1, ipIdx == 0 ? current + n1 : current + "." + n1, res, exist);
		
		if (start + 1 < s.size())
		{
			string n2 = s.substr(start, 2);
			if (n2.at(0) != '0') restoreIpAddressesHelper(s, start + 2, ipIdx + 1, ipIdx == 0 ? current + n2 : current + "." + n2, res, exist);
		}
		
		if (start + 2 < s.size())
		{
			string n3 = s.substr(start, 3);
			if (stoi(n3) <= 255)
			{
				if (n3.at(0) != '0') restoreIpAddressesHelper(s, start + 3, ipIdx + 1, ipIdx == 0 ? current + n3 : current + "." + n3, res, exist);
			}
		}
	}
	vector<string> restoreIpAddresses(string s) {
		vector<string> res;
		if (s.size() < 4 || s.size() > 12) return res;
		unordered_set<string> exist;
		restoreIpAddressesHelper(s, 0, 0, "", res, exist);
		return res;
	}
};

int main()
{
	Solution solu;
	{
		string s = "010010";
		vector<string> res = solu.restoreIpAddresses(s);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, "\n")); cout << endl;
	}

	std::cin.get();
	return 0;
}

