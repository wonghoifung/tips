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

class Stack {
public:
	void push(int x) { // make push O(n) can be more efficient because then pop and top will be O(1)
		q.push(x);
	}

	void pop() {
		queue<int> tmpq;
		while (q.size() > 1)
		{
			tmpq.push(q.front());
			q.pop();
		}
		q.swap(tmpq);
	}

	int top() {
		queue<int> tmpq;
		while (q.size() > 1)
		{
			tmpq.push(q.front());
			q.pop();
		}
		int last = q.front();
		tmpq.push(last);
		q.swap(tmpq);
		return last;
	}

	bool empty() {
		return q.size() == 0;
	}
private:
	queue<int> q;
};

int main()
{
	Stack ms;
	ms.push(1);
	cout << ms.top() << endl;
	cout << ms.empty() << endl;

	std::cin.get();
	return 0;
}




