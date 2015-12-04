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

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
	ListNode* reverse(ListNode* start, ListNode* end) {
		ListNode* p = start;
		ListNode* pp = NULL;
		while (p != end)
		{
			ListNode* n = p->next;
			p->next = pp;
			pp = p;
			p = n;
		}
		if (p == end)
		{
			p->next = pp;
			start->next = NULL;
		}
		return end;
	}
	ListNode* reverseKGroup(ListNode* head, int k) {
		if (k <= 1 || head == NULL || head->next == NULL)
		{
			return head;
		}

		ListNode* newHead = NULL;
		ListNode* nextStart = head;
		ListNode* lastReversedGroupEnd = NULL;
		while (nextStart)
		{
			ListNode* start = nextStart;
			ListNode* end = start;
			
			int c = k - 1;
			while (c > 0 && end->next)
			{
				end = end->next;
				c -= 1;
			}

			if (c == 0)
			{
				nextStart = end->next;
				ListNode* newStart = reverse(start, end);
				ListNode* newEnd = start;
				if (newHead == NULL)
				{
					newHead = newStart;
				}
				else
				{
					lastReversedGroupEnd->next = newStart;
				}
				lastReversedGroupEnd = newEnd;
			}
			else
			{
				if (lastReversedGroupEnd)
				{
					lastReversedGroupEnd->next = start;
				}
				break;
			}
		}

		return newHead == NULL ? head : newHead;
	}
};

/*
 * For example,
 * Given this linked list: 1->2->3->4->5
 * For k = 2, you should return: 2->1->4->3->5
 * For k = 3, you should return: 3->2->1->4->5
 * */

void print_list(ListNode* n) {
	while (n)
	{
		cout << n->val << " ";
		n = n->next;
	}
	cout << endl;
}

int main()
{
	ListNode n1(1), n2(2), n3(3), n4(4), n5(5);
	n1.next = &n2;
	n2.next = &n3;
	n3.next = &n4;
	n4.next = &n5;
	print_list(&n1);

	ListNode* n = Solution().reverseKGroup(&n1, 2);
	print_list(n);

	std::cin.get();

	return 0;
}

