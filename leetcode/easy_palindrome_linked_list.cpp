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
	ListNode* reverse(ListNode* head) {
		if (head == NULL || head->next == NULL)
		{
			return head;
		}
		ListNode* n = head;
		ListNode* nn = head->next;
		head->next = NULL;
		while (nn)
		{
			ListNode* tmp = nn->next;
			nn->next = n;
			n = nn;
			nn = tmp;
		}
		return n;
	}
	bool isPalindromeMine(ListNode* head) {
		if (head == NULL || head->next == NULL) return true;
		ListNode* fast = head;
		ListNode* slow = head;
		while (fast->next && fast->next->next)
		{
			fast = fast->next->next;
			slow = slow->next;
		}
		ListNode* mid = slow->next;
		ListNode* end = fast;
		while (end->next) end = end->next;
		ListNode* newmid = reverse(mid);
		while (newmid)
		{
			if (head->val != newmid->val)
			{
				return false;
			}
			head = head->next;
			newmid = newmid->next;
		}
		reverse(end);
		return true;
	}
	bool isPalindrome(ListNode* head) {
		return isPalindromeHelper(head, &head);
	}
	bool isPalindromeHelper(ListNode* head, ListNode** headpar) {
		if (head == NULL) return true;
		bool isp = isPalindromeHelper(head->next, headpar);
		if ((*headpar)->val == head->val)
		{
			(*headpar) = (*headpar)->next;
			return isp;
		}
		return false;
	}
};

int main()
{
	Solution s;
	{
		ListNode n1(1), n2(2), n3(3), n4(3), n5(2), n6(1);
		n1.next = &n2;
		n2.next = &n3;
		n3.next = &n4;
		n4.next = &n5;
		n5.next = &n6;
		print_list(&n1);
		cout << boolalpha << s.isPalindrome(&n1) << endl;
		print_list(&n1);
	}

	std::cin.get();
	return 0;
}

