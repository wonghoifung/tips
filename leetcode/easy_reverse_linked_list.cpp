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
#include <sstream>
#include <algorithm>
#include <bitset>
#include <numeric>
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
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
	ListNode* reverseList(ListNode* head) {
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
};

int main()
{
	Solution s;
	{

	}
	std::cin.get();

	return 0;
}

