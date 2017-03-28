#include <iostream>
#include <string>
using namespace std;
class Solution {
public:
    bool detectCapitalUse(string word) {
        bool firstCap=false;
		bool secondCap=false;
		for (int i=0;i<word.size();++i) {
			if (i==0 && word[i]<='Z' && word[i]>='A') firstCap=true;
			else {
				if (i==1 && word[i]<='Z' && word[i]>='A') secondCap=true;
				if (firstCap && secondCap && !(word[i] <= 'Z' && word[i] >= 'A')) return false;
				if (firstCap && !secondCap && word[i] <= 'Z' && word[i] >= 'A') return false;
				if (!firstCap && word[i] <= 'Z' && word[i] >= 'A') return false;
			}
		}
		return true;
    }
};
int main() {
	cout<<Solution().detectCapitalUse("USA")<<endl;//1
	cout<<Solution().detectCapitalUse("FlaG")<<endl;//0
}

