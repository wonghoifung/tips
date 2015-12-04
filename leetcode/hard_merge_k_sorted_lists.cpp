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
	struct pcmp
	{
		bool operator()(ListNode* l, ListNode* r)
		{
			return l->val > r->val;
		}
	};
	ListNode* mergeKLists(vector<ListNode*>& lists) {
		priority_queue<ListNode*, vector<ListNode*>, pcmp> q;
		for (size_t i = 0; i < lists.size(); ++i)
		{
			if (lists[i])
			{
				q.push(lists[i]);
			}
		}
		ListNode* newHead = NULL;
		ListNode* curNode = NULL;
		ListNode* lastNode = NULL;
		while (q.size())
		{
			curNode = q.top();
			q.pop();

			if (newHead == NULL)
			{
				newHead = curNode;
			}
			else
			{
				lastNode->next = curNode;
			}
			lastNode = curNode;

			if (curNode->next)
			{
				q.push(curNode->next);
			}
		}
		return newHead;
	}
};

int main()
{
	ListNode L1(1);
	ListNode L2(0);
	vector<ListNode*> lists{ &L1, &L2 };
	ListNode* n = Solution().mergeKLists(lists);
	while (n)
	{
		cout << n->val << " ";
		n = n->next;
	}

	std::cin.get();

	return 0;
}

