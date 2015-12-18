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
	ListNode* partition(ListNode* head, int x) {
		if (head == NULL || head->next == NULL) return head;
		ListNode* ebx = NULL; ListNode* ebxnext = NULL;
		ListNode* lx = NULL; ListNode* lxnext = NULL;
		while (head) {
			if (head->val < x) {
				if (lx == NULL) {
					lx = head;
					lxnext = head;
				}
				else {
					lxnext->next = head;
					lxnext = lxnext->next;
				}
			}
			else {
				if (ebx == NULL) {
					ebx = head;
					ebxnext = ebx;
				}
				else {
					ebxnext->next = head;
					ebxnext = ebxnext->next;
				}
			}
			head = head->next;
		}
		if (ebxnext) ebxnext->next = NULL;
		if (lxnext) lxnext->next = NULL;
		if (lx) {
			lxnext->next = ebx;
			return lx;
		}
		return ebx;
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

