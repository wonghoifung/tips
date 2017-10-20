#include <iostream>
#include <string>

using namespace std;

class Solution {
	bool help(const std::string& s, bool& canDel, bool& subOne, int& checkEnd, int i) {
		if (i>=checkEnd) return true;
		int sub = subOne ? 2 : 1;
		if (!canDel && !subOne) sub -= 1;
		if (s[i] == s[s.size() - sub - i]) return help(s,canDel,subOne,checkEnd,i+1);
		if (canDel && i+1 < s.size() && s[i+1] == s[s.size()-1-i]) {
			if (i+1>=checkEnd && checkEnd+1<s.size()) checkEnd += 1;
			canDel = false;
			if (help(s,canDel,subOne,checkEnd,i+2)) return true;
			canDel = true;
		}
		if (canDel && s[i] == s[s.size()-1-i-1]) {
			subOne = true;
			canDel = false;
			return help(s,canDel,subOne,checkEnd,i+1);
		}
		return false;
	}
public:
    bool validPalindrome(string s) {
		bool canDel = true;
		bool subOne = false;
		int checkEnd = s.size() / 2;
		return help(s,canDel,subOne,checkEnd,0);
    }
};

int main() {
	cout << Solution().validPalindrome("aguokepatgbnvfqmgmlcupuufxoohdfpgjdmysgvhmvffcnqxjjxqncffvmhvgsymdjgpfdhooxfuupuculmgmqfvnbgtapekouga") << endl;
}

