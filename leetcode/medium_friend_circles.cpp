#include <vector>
#include <iostream>
using namespace std;

class Solution {
	int find(int x, vector<int>& parents) {
		return parents[x]==x ? x : find(parents[x],parents);		
	}
public:
    int findCircleNum(vector<vector<int>>& M) {
		if (M.empty()) return 0;
		int n = M.size();
		vector<int> leads(n,0);
		for (int i=0; i<n; ++i) leads[i]=i;
		int groups = n;
		for (int i=0; i<n; ++i) {
			for (int j=i+1; j<n; ++j) {
				if (M[i][j]) {
					int lead1 = find(i,leads);
					int lead2 = find(j,leads);
					if (lead1 != lead2) {
						leads[lead1] = lead2;
						groups -= 1;
					}
				}
			}
		}
		return groups;
    }
};

int main() {
	{
		vector<vector<int>> M{{1,1,0},
							  {1,1,0},
							  {0,0,1}};
		cout << Solution().findCircleNum(M) << endl;//2
	}
	{
		vector<vector<int>> M{{1,1,0},
							  {1,1,1},
							  {0,1,1}};
		cout << Solution().findCircleNum(M) << endl;//1
	}
}

// https://discuss.leetcode.com/topic/85038/c-clean-code-dfs-unionfind/2
