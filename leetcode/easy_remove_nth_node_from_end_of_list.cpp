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

void print_list(ListNode* p) {
	while (p)
	{
		cout << p->val << " ";
		p = p->next;
	}
	cout << endl;
}

class Solution {
public:
	ListNode* removeNthFromEndMine(ListNode* head, int n) {
		if (head == NULL) return head;
		ListNode* fast = head;
		int c = 0;
		while (fast && c < n)
		{
			fast = fast->next;
			c += 1;
		}
		if (fast == NULL) return c == n ? head->next : head;
		ListNode* slow = head;
		while (fast->next)
		{
			fast = fast->next;
			slow = slow->next;
		}
		slow->next = slow->next->next;
		return head;
	}

	ListNode* removeNthFromEnd(ListNode* head, int n) {
		ListNode* dummy = new ListNode(-1);
		dummy->next = head;
		ListNode* p1 = dummy;
		ListNode* p2 = dummy;
		cout << "n:" << n << endl;
		while (p1->next)
		{
			p1 = p1->next;
			if (n-- <= 0)
			{
				p2 = p2->next;
				cout << "p2:" << p2->val << ", n:" << n << endl;
			}
		}
		if (p2->next) p2->next = p2->next->next;
		return dummy->next;
	}
};

int main()
{
	Solution s;
	{
		ListNode n1(1), n2(2), n3(3), n4(4), n5(5);
		n1.next = &n2;
		n2.next = &n3;
		n3.next = &n4;
		n4.next = &n5;
		print_list(&n1);
		ListNode* res1 = s.removeNthFromEnd(&n1, 1);
		print_list(res1);
	}

	std::cin.get();
	return 0;
}

