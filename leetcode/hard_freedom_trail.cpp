#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <limits.h>
using namespace std;
class Solution {
public:
    int findRotateSteps(string ring, string key) {
		int n = ring.size();
		int m = key.size();
		vector<vector<int> > dp(m+1,vector<int>(n,0));
		for (int i=m-1; i>=0; i--) {
			for (int j=0; j<n; j++) {
				dp[i][j]=INT_MAX;
				for (int k=0; k<n; k++) {
					if (ring[k]==key[i]) {
						int diff=abs(j-k);
						int step=min(diff,n-diff);
						dp[i][j]=min(dp[i][j],step+dp[i+1][k]);
					}
				}
			}
		}
		return dp[0][0]+m;
    }
};
int main() {
	cout << Solution().findRotateSteps("godding", "gd") << endl; // 4
	cout << Solution().findRotateSteps("abcde", "ade") << endl; // 6
	cout << Solution().findRotateSteps("edcba", "abcde") << endl; // 10
	cout << Solution().findRotateSteps("nyngl","yyynnnnnnlllggg") << endl; // 19
}
