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

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
	ListNode* oddEvenList(ListNode* head) {
		if (head == NULL || head->next == NULL) return head;
		ListNode* odd = head;
		ListNode* evenStart = head->next;
		ListNode* even = head->next;
		ListNode* p = even->next;
		bool isOdd = true;
		while (p)
		{
			if (isOdd) {
				odd->next = p;
				odd = p;
				even->next = NULL;
			}
			else{
				even->next = p;
				even = p;
				odd->next = NULL;
			}
			p = p->next;
			isOdd = !isOdd;
		}
		odd->next = evenStart;
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




