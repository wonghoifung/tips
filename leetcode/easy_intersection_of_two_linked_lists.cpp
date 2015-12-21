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
	ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
		if (headA == nullptr || headB == nullptr) return nullptr;
		ListNode* l1 = headA;
		ListNode* l2 = headB;
		while (l1 != l2)
		{
			l1 = (l1 == nullptr) ? headB : l1->next;
			l2 = (l2 == nullptr) ? headA : l2->next;
		}
		return l1;
	}
	ListNode *getIntersectionNodeMine(ListNode *headA, ListNode *headB) {
		if (headA == nullptr || headB == nullptr) return nullptr;
		if (headA == headB) return headA;
		vector<ListNode*> arrA, arrB;
		while (headA)
		{
			arrA.push_back(headA);
			headA = headA->next;
		}
		while (headB)
		{
			arrB.push_back(headB);
			headB = headB->next;
		}
		ListNode* p = nullptr;
		while (arrA.size() && arrB.size())
		{
			if (arrA.back() == arrB.back())
			{
				p = arrA.back();
				arrA.pop_back();
				arrB.pop_back();
				continue;
			}
			break;
		}
		return p;
	}
};

int main()
{
	Solution s;
	{
		ListNode n1(1), n2(2), n3(3), n4(4);
		n1.next = &n2;
		n2.next = &n3;
		n3.next = &n4;
		ListNode m1(11), m2(22), m3(33);
		m1.next = &m2;
		m2.next = &m3;
		m3.next = &n2;
		ListNode* res = s.getIntersectionNode(&n1, &m1);
		if (res == nullptr)
		{
			cout << "no intersect" << endl;
		}
		else
		{
			print_list(res);
		}
	}

	std::cin.get();
	return 0;
}

