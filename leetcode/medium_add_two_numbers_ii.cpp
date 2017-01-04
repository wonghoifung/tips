#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
	void toStack(ListNode* l, stack<int>& digits) {
		while (l) {
			digits.push(l->val);
			l = l->next;
		}
	}
public:
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		stack<int> digits1, digits2;
		toStack(l1, digits1);
		toStack(l2, digits2);
		auto getDigit = [](stack<int>& digits)->int {
			int d = 0;
			if (!digits.empty()) {
				d = digits.top();
				digits.pop();
			}
			return d;
		};
		int carry = 0;
		ListNode* res = NULL;
		while (!digits1.empty() || !digits2.empty()) {
			int d1 = getDigit(digits1);
			int d2 = getDigit(digits2);
			int d = d1 + d2;
			d += carry;
			carry = d / 10;
			d %= 10;
			if (res == NULL) {
				res = new ListNode(d);
			}
			else {
				ListNode* node = new ListNode(d);
				node->next = res;
				res = node;
			}
		}
		if (carry) {
			ListNode* node = new ListNode(carry);
			node->next = res;
			res = node;
		}
		return res;
	}
};

ostream& operator<<(ostream& o, const ListNode* ln) {
	o << "[ ";
	while (ln) {
		o << ln->val << " ";
		ln = ln->next;
	}
	o << "]" << endl;
	return o;
}

ListNode* createList(initializer_list<int> digits) {
	ListNode* node = NULL;
	ListNode* prev = NULL;
	for (auto d : digits) {
		if (node == NULL) {
			node = new ListNode(d);
			prev = node;
		}
		else {
			prev->next = new ListNode(d);
			prev = prev->next;
		}
	}
	return node;
}

int main() { // no need to delete the list nodes
	{
		ListNode* node1 = createList({ 7, 2, 4, 3 });
		ListNode* node2 = createList({ 5, 6, 4 });
		cout << Solution().addTwoNumbers(node1, node2) << endl; // 7 8 0 7
	}

	{
		ListNode* node1 = createList({ 3, 9, 9, 9, 9, 9, 9, 9, 9, 9 });
		ListNode* node2 = createList({ 7 });
		cout << Solution().addTwoNumbers(node1, node2) << endl; // [4,0,0,0,0,0,0,0,0,6]
	}

	cin.get();
	return EXIT_SUCCESS;
}

