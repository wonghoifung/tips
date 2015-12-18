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
	ListNode* swapPairs1(ListNode* head) {
		if (head == NULL || head->next == NULL) return head;
		ListNode* ret = head->next;
		ListNode* p0 = NULL;
		ListNode* p1 = head;
		ListNode* p2 = head->next;
		ListNode* p2n = p2->next;
		while (p1 && p2)
		{
			p2->next = p1;
			p1->next = p2n;
			if (p0) p0->next = p2;
			p0 = p1;
			if (p2n == NULL || p2n->next == NULL) break;
			p1 = p2n;
			p2 = p1->next;
			p2n = p2->next;
		}
		return ret;
	}
	ListNode* swapPairs(ListNode* head) {
		ListNode* newhead = head;
		if (head && head->next)
		{
			newhead = head->next;
			head->next = swapPairs(newhead->next);
			newhead->next = head;
		}
		return newhead;
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
		ListNode* ret = s.swapPairs(&n1);
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

