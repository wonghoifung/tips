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
	int count(ListNode* head) {
		int r = 0;
		while (head)
		{
			r += 1;
			head = head->next;
		}
		return r;
	}
	ListNode* rotateRight(ListNode* head, int k) {
		if (k <= 0 || head == NULL)
		{
			return head;
		}

		int len = count(head);
		if (k >= len)
		{
			/*return head;*/
			k = k % len;
		}
		
		if (k <= 0 || head == NULL)
		{
			return head;
		}
		
		ListNode* p1 = head;
		ListNode* p2 = p1;
		while (p2 && p2->next && k)
		{
			p2 = p2->next;
			k -= 1;
		}
		if (p2)
		{
			while (p2->next)
			{
				p1 = p1->next;
				p2 = p2->next;
			}
			ListNode* newHead = p1->next;
			p1->next = NULL;
			p2->next = head;
			return newHead;
		}
		return head;
	}
};
/*
 * Given 1->2->3->4->5->NULL and k = 2,
 * return 4->5->1->2->3->NULL.
 * */
void print_list(ListNode* head) {
	while (head)
	{
		cout << head->val << " ";
		head = head->next;
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
	int k = 3;
	print_list(&n1);
	cout << "--------------" << endl;
	ListNode* n = Solution().rotateRight(&n1, k);
	print_list(n);

	std::cin.get();

	return 0;
}

