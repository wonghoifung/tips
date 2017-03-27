#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Solution {
public:
	int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<vector<int> > mem(n,vector<int>(n));
        return longestPalindromeSubseq(0,n-1, s,mem); 
    }
    int longestPalindromeSubseq(int l, int r, string &s, vector<vector<int> >& mem) {
        if(l==r) return 1;
        if(l>r) return 0;
        if(mem[l][r]) return mem[l][r];
        return mem[l][r] = s[l]==s[r] ? 2 + longestPalindromeSubseq(l+1,r-1, s,mem) : 
            max(longestPalindromeSubseq(l+1,r, s,mem),longestPalindromeSubseq(l,r-1, s,mem)); 
        
    }
};
int main() {
	cout << Solution().longestPalindromeSubseq("bbbab") << endl; // 4
	cout << Solution().longestPalindromeSubseq("cbbd") << endl; // 2
}
