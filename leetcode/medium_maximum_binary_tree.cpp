#include <iostream>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution { 
    TreeNode* helper(vector<int>& nums, int start, int end) {
		if (end <= start || start >= nums.size()) return NULL;
		if (end-start == 1) return new TreeNode(nums[start]);
    	int maxIdx = start;
		int maxNum = nums[maxIdx];
		for (int i=start+1; i<end; ++i) {
			if (nums[i] > maxNum) {
				maxIdx = i;
				maxNum = nums[i];
			}
		}
		TreeNode* node = new TreeNode(maxNum);
		node->left = helper(nums, start, maxIdx);
		node->right = helper(nums, maxIdx+1, end);
		return node;
    }
public:
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
		return helper(nums, 0, nums.size());
    }
};

int main() {
}
