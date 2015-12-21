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
	ListNode* insertionSortList(ListNode* head) {
		if (head == nullptr || head->next == nullptr) return head;
		ListNode* newhead = head;
		ListNode* newp = newhead;
		head = head->next;
		newhead->next = nullptr;
		while (head)
		{
			ListNode* newp_parent = nullptr;
			while (newp && newp->val < head->val)
			{
				newp_parent = newp;
				newp = newp->next;
			}
			if (newp == nullptr || newp_parent)
			{
				newp_parent->next = head;
				head = head->next;
				newp_parent->next->next = newp;
				newp = newhead;
			}
			else
			{
				ListNode* tmp = newhead; 
				newhead = head; 
				head = head->next;
				newhead->next = tmp;
				newp = newhead;
			}
		}
		return newhead;
	}
};

int main()
{
	Solution s;
	{
		ListNode n1(4), n2(2), n3(1), n4(3);
		n1.next = &n2;
		n2.next = &n3;
		n3.next = &n4;
		ListNode* res = s.insertionSortList(&n1);
		print_list(res);
	}

	std::cin.get();
	return 0;
}

