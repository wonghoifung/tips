#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
	vector<string> binaryTreePaths(TreeNode* root) {
		vector<string> paths;
		if (root && root->left == NULL && root->right == NULL)
		{
			stringstream ss;
			ss << root->val;
			paths.push_back(ss.str());
		}
		if (root)
		{
			vector<string> lsubpaths = binaryTreePaths(root->left);
			for (size_t i = 0; i < lsubpaths.size(); i++)
			{
				stringstream ss;
				ss << root->val << "->" << lsubpaths[i];
				paths.push_back(ss.str());
			}
			vector<string> rsubpaths = binaryTreePaths(root->right);
			for (size_t i = 0; i < rsubpaths.size(); i++)
			{
				stringstream ss;
				ss << root->val << "->" << rsubpaths[i];
				paths.push_back(ss.str());
			}
		}
		return paths;
	}
};

int main()
{
	Solution s;

	TreeNode root(1), node1(2), node2(3), node3(5);
	root.left = &node1;
	root.right = &node2;
	node1.right = &node3;

	vector<string> paths = s.binaryTreePaths(&root);

	for (size_t i = 0; i < paths.size(); i++)
	{
		cout << paths[i] << endl;
	}

	std::cin.get();

	return 0;
}

