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
	bool hasCycle(ListNode *head) {
		if (head == NULL) return false;
		ListNode* slow = head;
		ListNode* fast = head;
		while (slow && fast && fast->next)
		{
			slow = slow->next;
			fast = fast->next->next;
			if (slow == fast) return true;
		}
		return false;
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

