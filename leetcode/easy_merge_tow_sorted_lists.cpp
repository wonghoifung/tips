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
	ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
		if (l1 == NULL) return l2;
		if (l2 == NULL) return l1;
		ListNode* head = NULL;
		ListNode* current = NULL;
		while (l1 && l2)
		{
			if (l1->val < l2->val)
			{
				if (head == NULL)
				{
					head = l1;
				}
				else
				{
					current->next = l1;
				}
				current = l1;
				l1 = l1->next;
			}
			else
			{
				if (head == NULL)
				{
					head = l2;
				}
				else
				{
					current->next = l2;
				}
				current = l2;
				l2 = l2->next;
			}
		}
		if (l1)
		{
			current->next = l1;
		}
		else
		{
			current->next = l2;
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

