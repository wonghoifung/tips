#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int removeBoxes(vector<int>& boxes) {
		int n = boxes.size();
		//int*** dp = new3Darray(n,n,n);
		int dp[100][100][100] = {0};
		int ret = removeBoxesSub(boxes,0,n-1,0,dp);
		//del3Darray(dp,n,n);
		return ret;
    }
private:
	//int removeBoxesSub(vector<int>& boxes, int i, int j, int k, int*** dp) {
	int removeBoxesSub(vector<int>& boxes, int i, int j, int k, int dp[100][100][100]) {
		if (i > j) return 0;
		if (dp[i][j][k] > 0) return dp[i][j][k];
		int res = (k+1)*(k+1)+removeBoxesSub(boxes,i+1,j,0,dp);
		for (int m=i+1; m<=j; m++) {
			if (boxes[i]==boxes[m]) {
				res = max(res, removeBoxesSub(boxes, i+1, m-1, 0, dp) + removeBoxesSub(boxes, m, j, k+1, dp));
			}
		}
		dp[i][j][k] = res;
		return res;
	}
	int*** new3Darray(int n1, int n2, int n3)
	{
		int*** p = (int***)malloc(n1 * sizeof(int**));	
		for (int i=0; i<n1; ++i) 
		{
			*(p+i) = (int**)malloc(n2 * sizeof(int*));
		}
		for (int i=0; i<n1; ++i)
		{
			for (int j=0; j<n2; ++j) 
			{
				*(*(p+i)+j) = (int*)malloc(n3 * sizeof(int));
				memset(*(*(p+i)+j), 0, n3 * sizeof(int));
			}
		}
		return p;
	}
	void del3Darray(int*** p, int n1, int n2)
	{
		for (int i=0; i<n1; ++i)
		{
			for (int j=0; j<n2; ++j)
			{
				free(p[i][j]);
			}
		}
		for (int i=0; i<n1; ++i)
		{
			free(p[i]);
		}
		free(p);
	}
};

int main()
{
	{
		vector<int> boxes = {1, 3, 2, 2, 2, 3, 4, 3, 1};
		cout << Solution().removeBoxes(boxes) << endl; // 23
	}
}

