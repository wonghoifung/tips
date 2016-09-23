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
#include <array>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

using namespace std;

struct ListNode {
	int val;
	ListNode* next;
	ListNode(int x) :val(x), next(NULL) {}
};

class Solution {
public:
	/** @param head The linked list's head.
 * 	Note that the head is guaranteed to be not null, so it contains at least one node. */
	Solution(ListNode* head) :head_(head) {
		srand(time(NULL));
	}

	/** Returns a random node's value. */
	int getRandom() {
		int ret = head_->val;
		ListNode* current = head_;
		for (int len = 1; current; ++len, current = current->next) {
			if (rand() % len == 0) { // possibility  of 1/len
				ret = current->val;
			}
		}
		return ret;
	}
private:
	ListNode* head_;
};

int main()
{
	ListNode head(1);
	ListNode node1(11);
	ListNode node2(22);
	ListNode node3(33);
	ListNode node4(44);
	ListNode node5(55);
	head.next = &node1;
	node1.next = &node2;
	node2.next = &node3;
	node3.next = &node4;
	node4.next = &node5;
	Solution s(&head);
	cout << s.getRandom() << endl;
	cout << s.getRandom() << endl;
	cout << s.getRandom() << endl;
	cout << s.getRandom() << endl;
	cout << s.getRandom() << endl;
	cout << s.getRandom() << endl;
	
	std::cin.get();
	return 0;
}




