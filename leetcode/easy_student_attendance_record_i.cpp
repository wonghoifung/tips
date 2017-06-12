#include <iostream>
#include <string>
using namespace std;
class Solution {
public:
    bool checkRecord(string s) {
        int Acount=0, Lcount=0;
		for (size_t i=0; i<s.size(); ++i) {
			if (s.at(i)=='A') {
				Acount += 1;
				if (Acount > 1) return false;
				Lcount = 0;
			}
			else if (s.at(i)=='L') {
				Lcount += 1;
				if (Lcount > 2) return false;
			}
			else {
				Lcount = 0;
			}
		}
		if (Acount > 1 || Lcount > 2) return false;
		return true;
    }
};
int main() {
	cout << Solution().checkRecord("PPALLP") << endl;
	cout << Solution().checkRecord("PPALLL") << endl;
	cout << Solution().checkRecord("LALL") << endl;
}
