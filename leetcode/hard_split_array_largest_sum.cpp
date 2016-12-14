#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
using namespace std;

#if 0
q : looks like this is good solution, but I have one question about that. 
	how did you make sure there result return from binary search function is reachable?

	such as, you are returning 18 from function binary, but how did you make sure 18 is a sum of subarray ?

	i'm not sure if it is possible : every sum of subarray is small than 18 but NO equals.

a : If none sum of the subarrays is 18, then there exists a sum that is the largest sum of these subarrays, say, 17. 
	The binary search returns the FIRST one that is feasible, so 17 will be returned instead.
#endif

class Solution {
	bool valid(long target, vector<int>& nums, int m) {
		int count = 1;
		long total = 0;
		for (int num : nums) {
			total += num;
			if (total > target) { // try our best to make sure that the sum of numbers between each two cuts is large enough but still less than target
				total = num;
				count += 1;
				if (count > m) return false; // target too small, we've already tried our best to make sure each part holds as many numbers as we can but we still have numbers left
			}
		}
		return true; // it is either we successfully divide the array into m parts and the sum of each part is less than target, or we used up all numbers before we reach m 
	}
public:
	int splitArray(vector<int>& nums, int m) {
		int max = 0;
		long sum = 0;
		for (int num : nums) {
			max = std::max(num, max);
			sum += num;
		}
		if (m == 1) return (int)sum;
		long l = max;
		long r = sum;
		while (l <= r) {
			long mid = (l + r) / 2;
			if (valid(mid, nums, m)) {
				r = mid - 1;
			}
			else {
				l = mid + 1; // mid too small
			}
		}
		return (int)l;
	}
};

int main() {
	{
		vector<int> nums = { 7, 2, 5, 10, 8 };
		int m = 2;
		cout << Solution().splitArray(nums, m) << endl; // 18
	}

	cin.get();
	return EXIT_SUCCESS;
}

