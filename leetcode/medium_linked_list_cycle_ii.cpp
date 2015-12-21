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
	ListNode* detectCycle(ListNode* head) {
		if (head == nullptr) return nullptr;
		ListNode* slow = head;
		ListNode* fast = head;
		ListNode* checkponint = head;
		bool found = false;
		while (slow && fast && fast->next)
		{
			slow = slow->next;
			fast = fast->next->next;
			if (slow == fast)
			{
				found = true;
				break;
			}
		}
		if (found == false) return nullptr;
#if 0
		while (true)
		{
			slow = slow->next;
			if (slow == checkponint) return checkponint;
			if (slow == fast) checkponint = checkponint->next;
		}
#else
		fast = head;
		while (slow != fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
		return slow;
#endif
		return nullptr;
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

