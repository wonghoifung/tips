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
	/* do it in-place and in one-pass */
	ListNode* reverseBetween(ListNode* head, int m, int n) {
		if (head == NULL || head->next == NULL || m >= n) return head;
		ListNode* p = head;
		ListNode* mpre = NULL;
		int i = 1;
		while (p->next && i < m)
		{
			mpre = p;
			p = p->next;
			i += 1;
		}
		if (i != m || p->next == NULL) return head;
		ListNode* np = p->next; 
		ListNode* nppre = p;
		while (nppre && i < n)
		{
			ListNode* tmp = np->next;
			np->next = nppre;

			nppre = np;
			np = tmp;
			i += 1;
		}
		p->next = np;
		if (mpre) mpre->next = nppre;
		else head = nppre;
		return head;
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
		ListNode* ret = s.reverseBetween(&n1,1,4);
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

