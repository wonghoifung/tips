#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
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

class Codec {
public:
	void serialize(TreeNode* root, ostringstream& out)
	{
		if ( !root ) return;
		out << root->val << ",";
		serialize(root->left, out);
		serialize(root->right, out);
	}

	string serialize(TreeNode* root) {
		ostringstream ss;
		serialize(root, ss);
		return ss.str();
	}

	TreeNode* deserialize(const string& s, int lower, int upper, int& pos )
	{
		if ( pos == s.size() ) return nullptr;
		int cur_pos = pos;
		int number = 0;
		while( s[cur_pos] != ',')
		{
			number = number * 10 + s[cur_pos] - '0';
			++cur_pos;
		}
		++cur_pos; // ,

		if ( number < lower || number > upper ) return nullptr; // not in this subtree

		TreeNode* root = new TreeNode( number );
		pos = cur_pos;
		root->left =  deserialize( s, lower, root->val, pos );
		root->right = deserialize( s, root->val, upper, pos );
		return root;
	}

	TreeNode* deserialize(string data) {
		int pos = 0;
		return deserialize( data, INT_MIN, INT_MAX, pos );

	}
};

#if 0
class Codec {
	void preorder_(TreeNode* root, stringstream& ss) {
		if (root) {
			ss << root->val << " ";
			preorder_(root->left, ss);
			preorder_(root->right, ss);
		}
	}
	string preorder(TreeNode* root) {
		if (root == NULL) return "";
		stringstream ss;
		preorder_(root, ss);
		return ss.str();
	}
	void inorder_(TreeNode* root, stringstream& ss) {
		if (root) {
			inorder_(root->left, ss);
			ss << root->val << " ";
			inorder_(root->right, ss);
		}
	}
	string inorder(TreeNode* root) {
		if (root == NULL) return "";
		stringstream ss;
		inorder_(root, ss);
		return ss.str();
	}
	void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
	{
		std::string::size_type lastpos = str.find_first_not_of(delimiters, 0);
		std::string::size_type pos = str.find_first_of(delimiters, lastpos);
		while (std::string::npos != pos || std::string::npos != lastpos)
		{
			tokens.push_back(str.substr(lastpos, pos - lastpos));
			lastpos = str.find_first_not_of(delimiters, pos);
			pos = str.find_first_of(delimiters, lastpos);
		}
	}
public:
	string serialize(TreeNode* root) {
		return preorder(root) + "\n" + inorder(root);
	}
	TreeNode* deserialize(const string& data) {
		vector<string> s;
		tokenize(data, s, "\n");
		vector<string> preorderVals;
		vector<string> inorderVals;
		tokenize(s[0], preorderVals, " ");
		tokenize(s[1], inorderVals, " ");
		/* TODO */
	}
};
#endif

int main() {

	cin.get();
	return EXIT_SUCCESS;
}

