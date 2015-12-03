#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
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
	ListNode* removeElements(ListNode* head, int val) {
		ListNode* c = head;
		ListNode* p = NULL;
		while (c)
		{
			if (c->val == val)
			{
				if (p == NULL) // head
				{
					head = c->next;
					delete c;
					c = head;
				}
				else
				{
					p->next = c->next;
					delete c;
					c = p->next;
				}
			}
			else
			{
				p = c;
				c = c->next;
			}
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

