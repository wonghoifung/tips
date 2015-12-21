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
		start_ = time(nullptr);
	}
	~mytimer()
	{
		time_t end = time(nullptr);
		cout << end - start_ << "s" << endl;
	}
};

struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x) : val(x), next(nullptr) {}
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
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		if (l1 == nullptr) return l2;
		if (l2 == nullptr) return l1;
		ListNode* head = new ListNode(l1->val + l2->val);
		int carry = head->val / 10;
		head->val %= 10;
		l1 = l1->next;
		l2 = l2->next;
		ListNode* current = head;
		while (l1 || l2)
		{
			ListNode* p = new ListNode(carry);

			if (l1)
			{
				p->val += l1->val;
				l1 = l1->next;
			}

			if (l2)
			{
				p->val += l2->val;
				l2 = l2->next;
			}

			carry = p->val / 10;
			p->val %= 10;
			current->next = p;
			current = p;
		}
		if (carry > 0)
		{
			ListNode* p = new ListNode(carry);
			current->next = p;
		}
		return head;
	}
};

int main()
{
	Solution s;
	{
		
	}

	std::cin.get();
	return 0;
}

