#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <deque>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(NULL);
	}
	~mytimer()
	{
		time_t end = time(NULL);
		cout << end - start_ << "s" << endl;
	}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Codec {
public:
	string int2str(int i) {
		stringstream ss;
		ss << i;
		string s;
		ss >> s;
		return ss.str();
	}

	int str2int(string& s) {
		stringstream ss;
		ss << s;
		int i;
		ss >> i;
		return i;
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

	void trans(string s, vector<string>& treearr, vector<string>& markarr) {
		vector<string> tree_mark;
		tokenize(s, tree_mark, "|");
		tokenize(tree_mark[0], treearr, " ");
		tokenize(tree_mark[1], markarr, " ");
	}

	string serialize(TreeNode* root) {
		if (root == NULL)
		{
			return string();
		}

		vector<string> treearr;
		vector<string> markarr;

		TreeNode* p = root;
		int pidx = 0;

		queue<TreeNode*> q;
		q.push(root);
		while (q.size())
		{
			TreeNode* p = q.front();
			q.pop();
			treearr.push_back(int2str(p->val));
			if (p->left == NULL && p->right == NULL)
			{
				markarr.push_back("E");
			}
			else if (p->left && p->right)
			{
				markarr.push_back("B");
				q.push(p->left);
				q.push(p->right);
			}
			else if (p->left && p->right == NULL)
			{
				markarr.push_back("L");
				q.push(p->left);
			}
			else if (p->left == NULL && p->right)
			{
				markarr.push_back("R");
				q.push(p->right);
			}
		}

		stringstream ss;
		for (size_t i = 0; i < treearr.size(); ++i)
		{
			ss << treearr[i] << " ";
		}
		ss << "| ";
		for (size_t i = 0; i < markarr.size(); ++i)
		{
			ss << markarr[i] << " ";
		}
		return ss.str();
	}

	TreeNode* build_tree(vector<string>& treearr, vector<string>& markarr) {
		TreeNode* root = NULL;
		vector<TreeNode*> trees;
		queue<TreeNode*> q;
		for (size_t i = 0; i < treearr.size(); ++i)
		{
			TreeNode* node = new TreeNode(str2int(treearr[i]));
			trees.push_back(node);
			q.push(node);
		}
		if (trees.size())
		{
			root = trees[0];
		}
		q.pop();
		for (size_t i = 0; i < markarr.size(); ++i)
		{
			if (markarr[i] == "E")
			{
				continue;
			}
			else if (markarr[i] == "B")
			{
				trees[i]->left = q.front();
				q.pop();
				trees[i]->right = q.front();
				q.pop();
			}
			else if (markarr[i] == "L")
			{
				trees[i]->left = q.front();
				q.pop();
			}
			else if (markarr[i] == "R")
			{
				trees[i]->right = q.front();
				q.pop();
			}
		}
		return root;
	}

	TreeNode* deserialize(string data) {
		if (data.empty())
		{
			return NULL;
		}
		vector<string> treearr;
		vector<string> markarr;
		trans(data, treearr, markarr);
		return build_tree(treearr, markarr);
	}
};

void print_tree(TreeNode* tree) {
	if (tree == NULL)
	{
		cout << "null" << endl;
		return;
	}
	cout << tree->val << endl;
	cout << "node " << tree->val << " left: ";
	print_tree(tree->left);
	cout << "node " << tree->val << " right: ";
	print_tree(tree->right);
}

bool is_tree_same(TreeNode* t1, TreeNode* t2) {
	if (t1 == NULL && t2 == NULL)
	{
		return true;
	}
	if (t1 == NULL || t2 == NULL)
	{
		return false;
	}
	bool leftsame = is_tree_same(t1->left, t2->left);
	bool rightsame = is_tree_same(t1->right, t2->right);
	return leftsame && rightsame && t1->val == t2->val;
}

void free_tree(TreeNode* t) {
	if (t == NULL)
	{
		return;
	}
	free_tree(t->left);
	free_tree(t->right);
	delete t;
}

int main()
{
	TreeNode node1(5), node2(2), node3(3), node4(2), node5(4), node6(3), node7(1);
	node1.left = &node2;
	node1.right = &node3;
	node3.left = &node4;
	node3.right = &node5;
	node4.left = &node6;
	node4.right = &node7;
	TreeNode* root = &node1;
	print_tree(root);
	cout << "--------------" << endl;

	Codec codec;
	string s = codec.serialize(root);
	cout << s << endl;
	TreeNode* t = codec.deserialize(s);
	cout << boolalpha << is_tree_same(root, t) << endl;
	cout << "--------------" << endl;
	print_tree(t);

	free_tree(t);
	t = NULL;

	std::cin.get();

	return 0;
}

