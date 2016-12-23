#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#if 0
This algorithm's idea is:
to iteratively determine what would be each bit of the final result from left to right.
And it narrows down the candidate group iteration by iteration.
e.g.assume input are a, b, c, d, ...z, 26 integers in total.
In first iteration, if you found that a, d, e, h, u differs on the MSB(most significant bit), 
so you are sure your final result's MSB is set. 
Now in second iteration, you try to see if among a, d, e, h, u there are at least two numbers make the 2nd MSB differs, 
if yes, then definitely, the 2nd MSB will be set in the final result. 
And maybe at this point the candidate group shinks from a,d,e,h,u to a, e, h. 
Implicitly, every iteration, you are narrowing down the candidate group, 
but you don't need to track how the group is shrinking, you only cares about the final result.
#endif

class Solution {
public:
	int findMaximumXOR(vector<int>& nums) {
		int max = 0, mask = 0;
		unordered_set<int> t;
#if 0
		search from left to right, find out for each bit is there 
		two numbers that has different value
#endif
		for (int i = 31; i >= 0; i--){
			/* mask contains the bits considered so far */
			mask |= (1 << i);
			t.clear();
			/* store prefix of all number with right i bits discarded */
			for (int n : nums){
				t.insert(mask & n);
			}
#if 0
			now find out if there are two prefix with different i-th bit
			if there is, the new max should be current max with one 1 bit at i-th position, which is candidate
			and the two prefix, say A and B, satisfies:
			A ^ B = candidate
			so we also have A ^ candidate = B or B ^ candidate = A
			thus we can use this method to find out if such A and B exists in the set 
#endif
			int candidate = max | (1 << i);
			for (int prefix : t){
				if (t.find(prefix ^ candidate) != t.end()){
					max = candidate;
					break;
				}

			}
		}
		return max;
	}
};

int main() {
	{
		vector<int> nums = { 3, 10, 5, 25, 2, 8 };
		cout << Solution().findMaximumXOR(nums) << endl; // 28
	}

	cin.get();
	return EXIT_SUCCESS;
}

