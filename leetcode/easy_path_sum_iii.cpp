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

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
	int countCanFormSum(TreeNode* start, int sum) {
		int c = 0;
		if (start->val == sum) c += 1;
		if (start->left) c += countCanFormSum(start->left, sum - start->val);
		if (start->right) c += countCanFormSum(start->right, sum - start->val);
		return c;
	}
public:
	int pathSum(TreeNode* root, int sum) {
		if (root == NULL) return 0;
		int res = 0;
		queue<TreeNode*> q;
		q.push(root);
		while (!q.empty()) {
			TreeNode* n = q.front(); 
			q.pop();
			if (n->left) q.push(n->left);
			if (n->right) q.push(n->right);
			int c = countCanFormSum(n, sum);
			if (c) res += c;
		}
		return res;
	}
};

int main() {
	{
		TreeNode root(10);
		TreeNode n1(5), n2(-3);
		root.left = &n1, root.right = &n2;
		TreeNode n3(3), n4(2), n5(11);
		n1.left = &n3, n1.right = &n4, n2.right = &n5;
		TreeNode n6(3), n7(-2), n8(1);
		n3.left = &n6, n3.right = &n7, n4.right = &n8;
		cout << Solution().pathSum(&root, 8) << endl; // 3
	}
	{
		TreeNode root(1);
		TreeNode n1(-2), n2(-3);
		root.left = &n1, root.right = &n2;
		TreeNode n3(1), n4(3), n5(-2);
		n1.left = &n3, n1.right = &n4, n2.left = &n5;
		TreeNode n6(-1);
		n3.left = &n6;
		cout << Solution().pathSum(&root, -1) << endl; // 4
	}
	cin.get();
	return EXIT_SUCCESS;
}

