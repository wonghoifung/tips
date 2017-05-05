#include <limits.h>
#include <iostream>
#include <sstream>
#include <string>
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
    int maxdiameter;
    Solution():maxdiameter(0) {}

    int height(TreeNode* root){        
        if (root == NULL) return 0;
        int lh = height(root->left);
        int rh = height(root->right);
        if(lh + rh > maxdiameter) maxdiameter = lh + rh;
        return max(lh +1, rh + 1);     
    }
    
    int diameterOfBinaryTree(TreeNode* root) {        
        height(root);
        return maxdiameter;
    }
};

TreeNode* toTree(const vector<int>& vt) {
    TreeNode* root = new TreeNode(vt[0]);
    struct {
        int nextlen;
        int idx;
        vector<TreeNode*> last;
        vector<TreeNode*> curr;
        int i;
        bool left;
        void init(TreeNode* n) {
            i=0;
            left=true;
            last.push_back(n);
            nextlen=last.size()*2;
            idx=1;
        }
        void next() {
            if (left) left=false;
            else {i+=1;left=true;}
        }
        void exchange() {
            i=0;
            left=true;
            last=curr;
            curr.clear();
            nextlen=last.size()*2;
        }
        void add(TreeNode* n) {
            if (left) last[i]->left=n;
            else last[i]->right=n;
            curr.push_back(n);
        }
    } status;
    status.init(root);
    while (status.idx < vt.size()) {
        int nextend = status.idx+status.nextlen;
        for (; status.idx<nextend && status.idx<vt.size(); ++status.idx) {
            if (vt[status.idx]==INT_MIN) {status.next();continue;}
            status.add(new TreeNode(vt[status.idx]));
            status.next();
        }
        status.exchange();
    }
    return root; 
}

void printTree(TreeNode* root) {
    if (root==NULL) return;
    int i=0;
    stringstream ss;
    queue<TreeNode*> q;
    q.push(root); ss<<root->val<<"("<<i<<") "; i+=1;
    while (q.empty()==false) {
        auto n = q.front();
        q.pop();
        if (n->left) {
            q.push(n->left);
            ss<<n->left->val<<"("<<i<<") "; i+=1;
        } else ss<<"NULL ";
        if (n->right) {
            q.push(n->right);
            ss<<n->right->val<<"("<<i<<") "; i+=1;
        } else ss<<"NULL ";
    }
    string s=ss.str();
    while (s.size()>5 && s.substr(s.size()-5)=="NULL ") {
        s=s.substr(0,s.size()-5);
    }
    cout<<s<<endl;
}

int main() {
    {
        vector<int> vt{4,-7,-3,INT_MIN,INT_MIN,-9,-3,9,-7,-4,INT_MIN,6,INT_MIN,-6,-6,INT_MIN,INT_MIN,0,6,5,INT_MIN,9,INT_MIN,INT_MIN,-1,-4,INT_MIN,INT_MIN,INT_MIN,-2};
        TreeNode* root=toTree(vt);
        printTree(root);
        cout<<Solution().diameterOfBinaryTree(root)<<endl;

        /*[4,-7,-3,null,null,-9,-3,9,-7,-4,null,6,null,-6,-6,null,null,0,6,5,null,9,null,null,-1,-4,null,null,null,-2]*/
        TreeNode n1(4),n2(-7),n3(-3),n4(-9),n5(-3),n6(9),n7(-7),n8(-4),n9(6),n10(-6),n11(-6),n12(0),n13(6),n14(5),n15(9),n16(-1),n17(-4),n18(-2);
        n1.left=&n2, n1.right=&n3;
        n3.left=&n4, n3.right=&n5;
        n4.left=&n6, n4.right=&n7, n5.left=&n8;
        n6.left=&n9, n7.left=&n10, n7.right=&n11;
        n9.left=&n12, n9.right=&n13, n10.left=&n14, n11.left=&n15;
        n12.right=&n16, n13.left=&n17, n15.left=&n18;
        printTree(&n1);
        cout<<Solution().diameterOfBinaryTree(&n1)<<endl;
    }
}
