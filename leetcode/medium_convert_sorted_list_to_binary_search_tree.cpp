#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <deque>
#include <stack>
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

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
	ListNode* midListNode(ListNode* head, ListNode* end) {
		if (head == end)
		{
			return NULL;
		}
		ListNode* p1 = head;
		ListNode* p2 = head;
		while (p2 != end && p2->next != end)
		{
			p1 = p1->next;
			p2 = p2->next->next;
		}
		return p1;
	}
	TreeNode* helpBuild(ListNode* node, ListNode* end) {
		ListNode* mid = midListNode(node, end);
		if (mid == NULL)
		{
			return NULL;
		}
		TreeNode* tn = new TreeNode(mid->val);
		tn->left = helpBuild(node, mid);
		tn->right = mid->next != end ? helpBuild(mid->next, end) : NULL;
		return tn;
	}
	TreeNode* sortedListToBST(ListNode* head) {
		if (head == NULL)
		{
			return NULL;
		}
		return helpBuild(head, NULL);
	}
};

void print_tree(TreeNode* tn) {
	if (tn)
	{
		print_tree(tn->left);
		cout << tn->val << " ";
		print_tree(tn->right);
		cout << endl;
	}
}

int main()
{
	Solution s;
	ListNode node(0);
	TreeNode* tn = s.sortedListToBST(&node);
	print_tree(tn);

	std::cin.get();

	return 0;
}

