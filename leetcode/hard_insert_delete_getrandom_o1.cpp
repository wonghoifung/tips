#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <array>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

class RandomizedSet {
public:
	/** Initialize your data structure here. */
	RandomizedSet() {

	}

	/** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
	bool insert(int val) {
		if (idxs.find(val) != idxs.end()) return false;
		nums.emplace_back(val);
		idxs[val] = nums.size() - 1;
		return true;
	}

	/** Removes a value from the set. Returns true if the set contained the specified element. */
	bool remove(int val) {
		if (idxs.find(val) == idxs.end()) return false;
		int last = nums.back();
		idxs[last] = idxs[val];
		nums[idxs[val]] = last;
		nums.pop_back();
		idxs.erase(val);
		return true;
	}

	/** Get a random element from the set. */
	int getRandom() {
		return nums[rand() % nums.size()];
	}
private:
	vector<int> nums;
	unordered_map<int, int> idxs;
};

int main()
{
	RandomizedSet obj;
	cout << boolalpha << obj.insert(1) << endl;
	cout << boolalpha << obj.remove(2) << endl;
	cout << obj.getRandom() << endl;
	
	std::cin.get();
	return 0;
}




