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

struct RandomListNode {
	int label;
	RandomListNode *next, *random;
	RandomListNode(int x) : label(x), next(NULL), random(NULL) {}
};

void print_list(RandomListNode* p) {
	while (p)
	{
		cout << p->label << " ";
		p = p->next;
	}
	cout << endl;
}

class Solution {
public:
	RandomListNode* copyRandomList(RandomListNode* head) {
		if (head == nullptr) return nullptr;
		unordered_map<RandomListNode*, RandomListNode*> mnext;
		unordered_map<RandomListNode*, RandomListNode*> mnew;
		RandomListNode* newhead = nullptr;
		RandomListNode* newcurrent = nullptr;
		RandomListNode* current = head;
		while (current)
		{
			RandomListNode* p = new RandomListNode(current->label);
			mnext[current] = current->random;
			mnew[current] = p;
			current = current->next;
			if (newhead == nullptr)
			{
				newhead = p;
				newcurrent = p;
			}
			else
			{
				newcurrent->next = p;
				newcurrent = p;
			}
		}
		newcurrent = newhead;
		current = head;
		while (current)
		{
			if (current->random)
			{
				newcurrent->random = mnew[mnext[current]];
			}
			
			current = current->next;
			newcurrent = newcurrent->next;
		}
		return newhead;
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

