#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
	bool tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
	{
		std::string::size_type lastpos = str.find_first_not_of(delimiters, 0);
		std::string::size_type pos = str.find_first_of(delimiters, lastpos);
		while (std::string::npos != pos || std::string::npos != lastpos)
		{
			tokens.push_back(str.substr(lastpos, pos - lastpos));
			lastpos = str.find_first_not_of(delimiters, pos);
			if (std::string::npos != lastpos && lastpos != pos + 1) return false;
			pos = str.find_first_of(delimiters, lastpos);
		}
		return true;
	}
	bool isIPv4(const string& ip) {
		int len = ip.size();
		if (len < 7 || len > 15) return false;
		if (ip[0] == '.' || ip[ip.size() - 1] == '.') return false;
		vector<string> nums;
		if (!tokenize(ip, nums, ".")) return false;
		if (nums.size() != 4) return false;
		for (int i = 0; i < nums.size(); ++i) {
			string& s = nums[i];
			if (s.empty()) return false;
			int num = 0;
			for (int j = 0; j < s.size(); ++j) {
				if (j == 0 && s[j] == '0') {
					if (s.size() > 1) return false;
					if (s.size() == 1 && i == 0) return false;
				}
				if (s[j] < '0' || s[j] > '9') return false;
				num *= 10;
				num += (s[j] - '0');
				if (num > 255) return false;
			}
		}
		return true;
	}
	bool isIPv6(const string& ip) {
		int len = ip.size();
		if (len < 15 || len > 39) return false;
		if (ip[0] == ':' || ip[ip.size() - 1] == ':') return false;
		vector<string> nums;
		if (!tokenize(ip, nums, ":")) return false;
		if (nums.size() != 8) return false;
		for (int i = 0; i < nums.size(); ++i) {
			string& s = nums[i];
			if (s.size() < 1 || s.size() > 4) return false;
			for (auto c : s) {
				if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) continue;
				else return false;
			}
		}
		return true;
	}
public:
	string validIPAddress(string IP) {
		if (isIPv4(IP)) return "IPv4";
		if (isIPv6(IP)) return "IPv6";
		return "Neither";
	}
};

int main() {
	cout << Solution().validIPAddress("172.16.254.1") << endl;
	cout << Solution().validIPAddress("2001:0db8:85a3:0:0:8A2E:0370:7334") << endl;
	cout << Solution().validIPAddress("256.256.256.256") << endl;
	cout << Solution().validIPAddress("2001:0db8:85a3:0:0:8A2E:0370:7334:") << endl;
	cout << Solution().validIPAddress("2001:db8:85a3:0:0::8a2E:0370:7334") << endl;

	cin.get();
}

