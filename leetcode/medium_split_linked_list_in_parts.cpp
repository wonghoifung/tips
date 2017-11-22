#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
	int listSize(ListNode* root) {
		int ret = 0;
		while (root) {
			ret += 1;
			root = root->next;
		}
		return ret;
	}
	ListNode* cutSize(int num, ListNode** root) {
		ListNode* ret = *root;
		ListNode* last = ret;
		do {
			last = *root;
			*root = (*root)->next;
		} while (--num);
		last->next = NULL;
		return ret;
	}
public:
    vector<ListNode*> splitListToParts(ListNode* root, int k) {
		vector<ListNode*> vec;
		if (k <= 0) return vec;
		if (k == 1) {vec.push_back(root);return vec;}
		int size = listSize(root);
		if (size <= k) {
			for (int i=0; i<size; ++i) {
				ListNode* node = root;
				root = root->next;
				node->next = NULL;
				vec.push_back(node);
			}
			for (int i=0; i<(k-size); ++i) vec.push_back(NULL);
		} else {
			int num = size / k;
			int cnt = size % k;
			for (int i=0; i<cnt; ++i) { // num + 1
				ListNode* node = cutSize(num + 1, &root);
				vec.push_back(node);
			}
			for (int i=0; i<(k-cnt); ++i) { // num
				ListNode* node = cutSize(num, &root);
				vec.push_back(node);
			}
		}
		return vec;
    }
};

int main() {

}
