#include <iostream>
#include <vector>
using namespace std;
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x):val(x),left(NULL),right(NULL){}
};
class Solution {
    void tovector(TreeNode* root, vector<TreeNode*>& vec) {
        if (root && root->left) tovector(root->left, vec);
        if (root) vec.push_back(root);
        if (root && root->right) tovector(root->right, vec);
    }
    void add(vector<TreeNode*>& vec) {
        if (vec.size() < 2) return;
        for (int i=vec.size()-2; i>=0; --i) {
            vec[i]->val += vec[i+1]->val;
        }
    }
public:
    TreeNode* convertBST(TreeNode* root) {
        vector<TreeNode*> vec;
        tovector(root,vec);
        add(vec);
        return root;
    }
};
int main() {
}
