#include <iostream>
#include <deque>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
 
class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int v, int d) {
    	if (d < 1 || root == NULL) return root;
        if (d == 1) {
        	TreeNode* ret = new TreeNode(v);
        	ret->left = root;
        	return ret;
        }
        int currentDep = 1;
        deque<TreeNode*> row = { root };
        while (currentDep < d - 1) {
        	auto sz = row.size();
        	for (auto i=0; i<sz; ++i) {
        		TreeNode* node = row[0];
        		row.pop_front();
        		if (node->left) row.push_back(node->left);
        		if (node->right) row.push_back(node->right);
        	}
        	currentDep += 1;
        }
        for (auto i=0; i<row.size(); ++i) {
        	TreeNode* lnode = new TreeNode(v);
        	TreeNode* rnode = new TreeNode(v);
        	lnode->left = row[i]->left;
        	rnode->right = row[i]->right;
        	row[i]->left = lnode;
        	row[i]->right = rnode;
        }
        return root;
    }
};

int main() {

}
