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

class MinStack {
public:
	void push(int x) {
		v.push_back(x);
		if (minv.empty() || x < minv.back()) minv.push_back(x);
		else minv.push_back(minv.back());
	}

	void pop() {
		v.pop_back();
		minv.pop_back();
	}

	int top() {
		return v.back();
	}

	int getMin() {
		return minv.back();
	}
private:
	vector<int> v;
	vector<int> minv;
};

int main()
{
	MinStack ms;
	ms.push(-1);
	cout << ms.top() << endl;
	cout << ms.getMin() << endl;

	std::cin.get();
	return 0;
}




