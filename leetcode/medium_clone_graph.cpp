#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
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

struct UndirectedGraphNode {
    int label;
    vector<UndirectedGraphNode*> neighbors;
    UndirectedGraphNode(int x) : label(x) {};
};

class Solution {
public:
	UndirectedGraphNode* recursiveClone(UndirectedGraphNode* node, std::map<int, UndirectedGraphNode*>& labels) {
		if (node == NULL)
		{
			return NULL;
		}
		auto it = labels.find(node->label);
		if (it != labels.end())
		{
			return it->second;
		}
		UndirectedGraphNode* n = new UndirectedGraphNode(node->label);
		labels[n->label] = n;
		for (size_t i = 0; i < node->neighbors.size(); ++i)
		{
			UndirectedGraphNode* cn = recursiveClone(node->neighbors[i], labels);
			if (cn)
			{
				n->neighbors.push_back(cn);
			}
		}
		return n;
	}
	UndirectedGraphNode* cloneGraph(UndirectedGraphNode* node) {
		std::map<int, UndirectedGraphNode*> labels;
		return recursiveClone(node, labels);
	}
};

int main()
{
	

	std::cin.get();

	return 0;
}

