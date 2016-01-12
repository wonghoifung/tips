#include <iostream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    void maxArray(vector<int>& nums, int k, vector<int>& res) {
        int n = (int)nums.size();
        for (int i=0,j=0; i<n; ++i) {
            while (n-i+j>k && j>0 && res[j-1]<nums[i]) j-=1;
            if(j<k) res[j++]=nums[i];
        }
    }
    bool mygreater(vector<int>& nums1, int i, vector<int>& nums2, int j) {
        while (i<nums1.size()&&j<nums2.size()&&nums1[i]==nums2[j]) {
            i+=1; j+=1;
        }
        return j==nums2.size() || (i<nums1.size()&&nums1[i]>nums2[j]);
    }
    void merge(vector<int>& nums1, vector<int>& nums2, int k, vector<int>& res) {
        for (int i=0,j=0,r=0; r<k; ++r) {
            res[r]=mygreater(nums1,i,nums2,j) ? nums1[i++]:nums2[j++];
        }
    }
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        int n=(int)nums1.size();
        int m=(int)nums2.size();
        vector<int> res(k,0);
        for (int i=max(0,k-m); i<=k&&i<=n; ++i) {
            vector<int> arr1(i,0);
            maxArray(nums1, i, arr1);
            
            vector<int> arr2(k-i,0);
            maxArray(nums2, k-i, arr2);
            
            vector<int> mgrarr(k,0);
            merge(arr1, arr2, k, mgrarr);
            
            if(mygreater(mgrarr, 0, res, 0)) swap(res,mgrarr);
        }
        return res;
    }
};

int main(int argc, char** argv)
{
    
    
    return 0;
}

