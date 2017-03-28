#include <iostream>
#include <vector>
#include <string>
#include <numeric>
using namespace std;
class Solution {
public:
    int findMinMoves(vector<int>& machines) {
		int n=machines.size();
		int sum=accumulate(machines.begin(),machines.end(),0);
		if (sum % n != 0) return -1;
		int avg=sum/n;
		vector<int> leftSums(n,0);
		vector<int> rightSums(n,0);
		for (int i=1;i<n;++i) leftSums[i]=leftSums[i-1]+machines[i-1];
		for (int i=n-2;i>=0;--i) rightSums[i]=rightSums[i+1]+machines[i+1];
		int move=0;
		for (int i=0; i<n; ++i) {
			int expLeft=i*avg;
			int expRight=(n-i-1)*avg;
			int left=0;
			int right=0;
			if (expLeft>leftSums[i]) left=expLeft-leftSums[i];
			if (expRight>rightSums[i]) right=expRight-rightSums[i];
			move=max(move,left+right);
		}
		return move;
    }
};
int main() {
	{
		vector<int> machines{1,0,5};
		cout<<Solution().findMinMoves(machines)<<endl; // 3
	}
	{
		vector<int> machines{0,3,0};
		cout<<Solution().findMinMoves(machines)<<endl; // 2
	}
	{
		vector<int> machines{0,2,0};
		cout<<Solution().findMinMoves(machines)<<endl; // -1
	}
}
