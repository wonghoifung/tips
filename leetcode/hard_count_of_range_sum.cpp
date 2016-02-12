#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <set>
#include <map>

using namespace std;

class Solution {
private:
    int mergeSort(vector<long long>&sum, int left, int right, int lower, int upper)
    {
        int mid, i, res, j, k;
        if(left>right) return 0;
        if(left==right) return ((sum[left]>=lower) && (sum[left]<=upper)) ? 1 : 0;
        else
        {
            vector<long long> temp(right-left+1,0);
            mid = (left+right)/2;
            res = mergeSort(sum, left, mid, lower, upper) + mergeSort(sum, mid+1, right, lower, upper);
            for(i=left, j=k=mid+1; i<=mid; ++i)
            {
                while(j<=right && sum[j]-sum[i]<lower) ++j;
                while(k<=right && sum[k]-sum[i]<=upper) ++k;
                res += (k-j);
            }
            for(i=k=left, j=mid+1; k<=right; ++k)
                temp[k-left] = ((i<=mid) && (j>right || sum[i]<sum[j])) ? sum[i++] : sum[j++];
            for(k=left; k<=right; ++k)
                sum[k] = temp[k-left];
            return res;
        }
    }
public:
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int len = (int)nums.size(), i;
        vector<long long> sum(len+1, 0);
        for(i=1; i<=len; ++i) sum[i] = sum[i-1]+nums[i-1];
        return mergeSort(sum, 1, len, lower, upper);
    }
};

int main(int argc, char** argv)
{
    
    
    return 0;
}

