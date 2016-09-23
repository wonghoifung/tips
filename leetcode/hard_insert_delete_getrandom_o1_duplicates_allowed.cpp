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

class RandomizedCollection {
public:
	/** Initialize your data structure here. */
	RandomizedCollection() {

	}

	/** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
	bool insert(int val) {
		auto result = idxs.find(val) == idxs.end();
		idxs[val].push_back(nums.size());
		nums.push_back(make_pair(val, idxs[val].size() - 1));
		return result;
	}

	/** Removes a value from the set. Returns true if the set contained the specified element. */
	bool remove(int val) {
		dump("remove_before<<<", val);
		auto result = idxs.find(val) != idxs.end();
		if (result) {
			auto nums_idx_to_be_reuse = idxs[val].back();
			auto num_pair_to_be_move = nums.back();
			auto num_to_be_move = num_pair_to_be_move.first;
			auto num_to_be_move_local_pos = num_pair_to_be_move.second;

			/* move the last num to the position of the deleted val */
			idxs[num_to_be_move][num_to_be_move_local_pos] = nums_idx_to_be_reuse;
			nums[nums_idx_to_be_reuse] = num_pair_to_be_move;

			/* remove one val */
			idxs[val].pop_back();
			if (idxs[val].empty()) idxs.erase(val);

			/* remove the num that's already been moved to the deleted pos */
			nums.pop_back();
		}
		dump("remove_after>>>", val);
		return result;
	}

	/** Get a random element from the set. */
	int getRandom() {
		return nums[rand() % nums.size()].first;
	}
private:
	vector<pair<int, int>> nums; // val -> index in idxs[val]
	unordered_map<int, vector<int>> idxs; // val -> all indexes in nums
	void dump(const char* tag, int val) {
		cout << "------------------" << endl;
		cout << tag << ":" << val << " nums: ";
		for (auto ip : nums) {
			cout << ip.first << ":" << ip.second << " ";
		}
		cout << endl;
		cout << tag << ":" << val << " idxs: " << endl;
		for (auto& p : idxs) {
			cout << "  " << p.first << ": ";
			for (auto i : p.second) {
				cout << i << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};

int main()
{
	RandomizedCollection obj;
	cout << boolalpha << obj.insert(10) << endl;
	cout << boolalpha << obj.insert(10) << endl;
	cout << boolalpha << obj.insert(20) << endl;
	cout << boolalpha << obj.insert(20) << endl;
	cout << boolalpha << obj.insert(20) << endl;
	cout << boolalpha << obj.insert(20) << endl;
	cout << boolalpha << obj.insert(30) << endl;
	cout << boolalpha << obj.insert(30) << endl;
	cout << boolalpha << obj.remove(10) << endl;
	cout << boolalpha << obj.remove(10) << endl;
	cout << boolalpha << obj.remove(30) << endl;
	cout << boolalpha << obj.remove(30) << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	cout << obj.getRandom() << endl;
	
	std::cin.get();
	return 0;
}




