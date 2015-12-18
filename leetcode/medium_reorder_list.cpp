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
	void reorderList(ListNode* head) {
		if (head == NULL || head->next == NULL || head->next->next == NULL) return;
		ListNode* slow = head;
		ListNode* fast = head;
		while (fast->next && fast->next->next)
		{
			slow = slow->next;
			fast = fast->next->next;
		}
		ListNode* right = slow->next;
		slow->next = NULL;

		ListNode* ppre = NULL;
		ListNode* p = right;
		ListNode* np = p->next;
		while (np)
		{
			ListNode* tmp = np->next;
			np->next = p;
			p->next = ppre;

			ppre = p;
			p = np;
			np = tmp;
		}

		ListNode* n1 = head->next;
		ListNode* n2 = p; 
		ListNode* nn = head;
		while (n1 || n2)
		{
			if (n2)
			{
				nn->next = n2;
				n2 = n2->next;
				nn = nn->next;
			}
			if (n1)
			{
				nn->next = n1;
				n1 = n1->next;
				nn = nn->next;
			}
		}
	}
};

int main()
{
	Solution s;
	{
		ListNode n1(1), n2(2), n3(3), n4(4);
		n1.next = &n2;
		n2.next = &n3;
		n3.next = &n4;
		s.reorderList(&n1);
		print_list(&n1);
	}

	std::cin.get();
	return 0;
}

