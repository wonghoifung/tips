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
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
	ListNode* deleteDuplicates(ListNode* head) {
		if (head == NULL || head->next == NULL)
		{
			return head;
		}
		ListNode* n = head;
		ListNode* pn = NULL; bool needdel = false; 
		while (n)
		{
			if (n->next && n->val == n->next->val)
			{
				needdel = true;
				n->next = n->next->next;
				continue;
			}
			if (needdel)
			{
				needdel = false;
				if (pn == NULL)
				{
					head = n->next;
				}
				else
				{
					pn->next = n->next;
				}
			}
			else
			{
				pn = n;
			}
			n = n->next;
		}

		return head;
	}
};

int main()
{
	Solution s;
	{
		ListNode n1(1), n2(1);
		n1.next = &n2;
		ListNode* n = s.deleteDuplicates(&n1);
		while (n)
		{
			cout << n->val << " ";
			n = n->next;
		}
		cout << endl;
	}
	std::cin.get();

	return 0;
}

