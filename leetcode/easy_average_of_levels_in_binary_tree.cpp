#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    vector<double> averageOfLevels(TreeNode* root) {
		vector<double> v;
		if (root == NULL) return v;
		queue<TreeNode*> sameLevel;		
		sameLevel.push(root);
		double sum = 0;
		double len = 0;
		while (!sameLevel.empty()) {
			size_t s = sameLevel.size();
			while (s) {
				TreeNode* node = sameLevel.front();
				sameLevel.pop();
				if (node->left) sameLevel.push(node->left);
				if (node->right) sameLevel.push(node->right);
				sum += node->val;
				len += 1;
				s -= 1;
			}
			v.push_back(sum / len);
			sum = 0;
			len = 0;
		}
		return v;
    }
};

int main() {

}
