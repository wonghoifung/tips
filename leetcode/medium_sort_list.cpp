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
#include <memory>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

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
	ListNode* merge(ListNode* n1, ListNode* n2) {
		if (n1 == NULL) return n2;
		if (n2 == NULL) return n1;
		if (n1->val < n2->val)
		{
			n1->next = merge(n1->next, n2);
			return n1;
		}
		n2->next = merge(n1, n2->next);
		return n2;
	}
	ListNode* sortList(ListNode* head) {
		if (head == NULL || head->next == NULL) return head;
		ListNode* n1 = head; ListNode* pre = NULL;
		ListNode* n2 = head;
		while (n2 && n2->next)
		{
			pre = n1;
			n1 = n1->next;
			n2 = n2->next->next;
		}
		ListNode* mid = n1;
		pre->next = NULL;
		ListNode* left = sortList(head);
		ListNode* right = sortList(mid);
		return merge(left, right);
	}
};

int main()
{
	Solution s;
	{
		ListNode n1(4), n2(3), n3(2), n4(1);
		n1.next = &n2;
		n2.next = &n3;
		n3.next = &n4;
		ListNode* ret = s.sortList(&n1);
		ListNode* p = ret;
		while (p)
		{
			cout << p->val << " ";
			p = p->next;
		}
		cout << endl;
	}

	std::cin.get();
	return 0;
}

