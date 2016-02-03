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

class Queue {
public:
	void push(int x) {
		stack<int> tmps;
		moveStack(s, tmps);
		s.push(x);
		moveStack(tmps, s);
	}

	void pop(void) {
		s.pop();
	}

	int peek(void) {
		return s.top();
	}

	bool empty(void) {
		return s.empty();
	}
private:
	void moveStack(stack<int>& from, stack<int>& to) {
		while (!from.empty())
		{
			to.push(from.top());
			from.pop();
		}
	}
	stack<int> s;
};

int main()
{


	std::cin.get();
	return 0;
}




