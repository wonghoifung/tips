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
#include <sstream>
#include <algorithm>
#include <bitset>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(NULL);
	}
	~mytimer()
	{
		time_t end = time(NULL);
		cout << end - start_ << "s" << endl;
	}
};

class NumArray {
public:
	vector<int> orig;
	int* segTree;
	int n0;

	void allocSegTree() {
		if (n0 > 0 && segTree == NULL)
		{
			int h = (int)ceil(log2(n0)); // root height 0
			int len = 2 * pow(2, h) - 1;
			segTree = (int*)malloc(len*sizeof(int));
			memset(segTree, 0, len*sizeof(int));
		}
	}

	void fillSegTree(vector<int>& leaves, int sl, int sr, int rootidx) {
		if (sl == sr)
		{
			segTree[rootidx] = leaves[sl];
			return;
		}
		int mid = (sl + sr + 1) / 2;
		fillSegTree(leaves, sl, mid - 1, 2 * rootidx + 1);
		fillSegTree(leaves, mid, sr, 2 * rootidx + 2);
		segTree[rootidx] = segTree[2 * rootidx + 1] + segTree[2 * rootidx + 2];
	}

	int getSum(int rootidx, int sl, int sr, int ql, int qr) {
		if (ql <= sl && qr >= sr)
		{
			return segTree[rootidx];
		}
		if (ql > sr || qr < sl)
		{
			return 0;
		}
		int mid = (sl + sr + 1) / 2;
		return getSum(2 * rootidx + 1, sl, mid - 1, ql, qr) +
			   getSum(2 * rootidx + 2, mid, sr, ql, qr);
	}

	void updateSum(int rootidx, int sl, int sr, int i, int diff) {
		if (i < sl || i > sr)
		{
			return;
		}
		segTree[rootidx] += diff;
		if (sl != sr)
		{
			int mid = (sl + sr + 1) / 2;
			updateSum(2 * rootidx + 1, sl, mid - 1, i, diff);
			updateSum(2 * rootidx + 2, mid, sr, i, diff);
		}
	}

	void update(int i, int val) {
		if (i < 0 || i >= n0)
		{
			return;
		}
		int diff = val - orig[i];
		orig[i] = val;
		updateSum(0, 0, n0 - 1, i, diff);
	}

	int sumRange(int i, int j) {
		if (i < 0 || j >= n0 || i > j)
		{
			assert(false);
		}
		return getSum(0, 0, n0 - 1, i, j);
	}

	NumArray(vector<int>& nums) :orig(nums), segTree(NULL), n0(nums.size()) {
		allocSegTree();
		if (segTree)
		{
			fillSegTree(nums, 0, n0 - 1, 0);
		}
	}

	~NumArray() {
		if (segTree)
		{
			free(segTree);
		}
	}
};

int main()
{
	vector<int> nums{ 1, 3, 5 };
	NumArray numArray(nums);
	cout << numArray.sumRange(0, 1) << endl;
	numArray.update(1, 10);
	cout << numArray.sumRange(1, 2) << endl;

	std::cin.get();
	return 0;
}

