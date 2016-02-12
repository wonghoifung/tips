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
public:
    void wiggleSort(vector<int>& nums) {
        int n = (int)nums.size();
        
        auto midptr = nums.begin() + n / 2;
        nth_element(nums.begin(), midptr, nums.end());
        int mid = *midptr;
        
        #define A(i) nums[(1+2*(i)) % (n|1)]
        
        int i = 0, j = 0, k = n - 1;
        while (j <= k) {
            if (A(j) > mid)
                swap(A(i++), A(j++));
            else if (A(j) < mid)
                swap(A(j), A(k--));
            else
                j++;
        }
    }
};

int main(int argc, char** argv)
{
    Solution s;
    {
        vector<int> nums{1,3,2,2,3,1};
        s.wiggleSort(nums);
        copy(nums.begin(), nums.end(), ostream_iterator<int>(cout," "));
        cout<<endl;
    }
    return 0;
}

