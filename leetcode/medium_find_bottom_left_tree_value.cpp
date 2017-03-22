#include <iostream>
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
    int findBottomLeftValue(TreeNode* root) {
    queue<TreeNode*> q;
		q.push(root);
		TreeNode* curleft=root;
		while (!q.empty()) {
			int len = q.size();
			TreeNode* newleft=NULL;
			for (int i=0; i<len; ++i) {
				TreeNode* n=q.front();
				q.pop();
				if (n->left) {
					q.push(n->left);
					if (newleft==NULL) newleft=n->left;
				}
				if (n->right) {
					q.push(n->right);
					if (newleft==NULL) newleft=n->right;
				}
			}
			if (newleft) curleft=newleft;
		}
		return curleft->val;
    }
};
int main() {
	{
		TreeNode n1(1),n2(2),n3(3),n4(4),n5(5),n6(6),n7(7);
		n1.left=&n2; n1.right=&n3;
		n2.left=&n4;
		n3.left=&n5; n3.right=&n6;
		n5.left=&n7;
		cout << Solution().findBottomLeftValue(&n1) << endl;
	}
}

