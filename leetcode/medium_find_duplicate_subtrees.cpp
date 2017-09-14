#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
	typedef std::map<std::string,std::vector<TreeNode*> > t_dups;
	std::string toString(int val) {
		std::stringstream ss;
		ss << val;
		return ss.str();
	}
	std::string serialize(TreeNode* node, t_dups& m) {
		if (!node) return "";
		std::string s = "(" + 
						serialize(node->left,m) + 
						toString(node->val) + 
						serialize(node->right,m) + 
						")";
		m[s].push_back(node);
		return s;
	}
public:
    std::vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
		t_dups m;
		std::vector<TreeNode*> dups;
		serialize(root,m);
		for (t_dups::iterator it = m.begin(); it != m.end(); ++it) {
			if (it->second.size() > 1) dups.push_back(it->second[0]);
		}
		return dups;
    }
};

int main()
{
}

