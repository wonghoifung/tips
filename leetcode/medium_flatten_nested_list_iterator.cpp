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

using namespace std;

class NestedInteger {
private:
	bool isi;
	int i;
	vector<NestedInteger> v;
public:
	void setInteger(int ii) {
		isi = true;
		i = ii;
	}
	void setList(vector<NestedInteger>& l) {
		isi = false;
		v = l;
	}
	void setIntList(int cnt, ...) {
		isi = false;
		v.clear();
		va_list ap;
		va_start(ap, cnt);
		for (int i = 0; i < cnt; ++i)
		{
			NestedInteger tmp;
			tmp.setInteger(va_arg(ap, int));
			v.push_back(tmp);
		}
		va_end(ap);
	}
	bool isInteger() const { return isi; }
	int getInteger() const { return i; }
	const vector<NestedInteger>& getList() const { return v; }
};

class NestedIterator {
private:
	vector<NestedInteger>& v;
	vector<int> nextIdx; // an index contains many sub-indices for every level 

	NestedInteger* getNi() {
		if (nextIdx.empty()) return NULL;
		NestedInteger* ni = &v[nextIdx[0]];
		for (int i = 1; i < nextIdx.size(); ++i)
		{
			ni = (NestedInteger*)&(ni->getList()[nextIdx[i]]);
		}
		return ni;
	}

	void updateNextIdx() {
		if (nextIdx.empty()) return; // failure

		while (1) {
			int i = nextIdx.back();
			nextIdx.pop_back();

			NestedInteger* pni = getNi();
			NestedInteger* ni = NULL;

			i += 1;
			if (pni == NULL) {
				if (i >= v.size()) return; // end
				nextIdx.push_back(i);
				ni = &v[i];
			}
			else {
				if (i >= pni->getList().size()) continue; // this level is done
				nextIdx.push_back(i);
				ni = (NestedInteger*)&(pni->getList()[i]);
			}

			if (ni->isInteger()) break; // success

			while (!ni->isInteger())
			{
				if (ni->getList().empty()) break; // this level is done

				nextIdx.push_back(0);
				ni = (NestedInteger*)&(ni->getList()[0]);
				if (ni->isInteger()) return; // success
			}
		}
	}

public:
	NestedIterator(vector<NestedInteger>& nestedList) :v(nestedList) {
		if (!nestedList.empty()) {
			nextIdx.push_back(-1);
			updateNextIdx();
		}
	}

	int next() {
		assert(!nextIdx.empty());
		int ret;
		NestedInteger* ni = getNi();
		assert(ni->isInteger());
		ret = ni->getInteger();
		updateNextIdx();
		return ret;
	}

	bool hasNext() {
		return !nextIdx.empty();
	}
};

int main()
{
	{
		vector<NestedInteger> nestedList; // [[1,1],2,[1,1]]

		NestedInteger ni1; ni1.setIntList(2, 1, 1); nestedList.push_back(ni1);
		NestedInteger ni2; ni2.setInteger(2); nestedList.push_back(ni2);
		NestedInteger ni3; ni3.setIntList(2, 1, 1); nestedList.push_back(ni3);

		NestedIterator i(nestedList);
		while (i.hasNext()) cout << i.next() << ' ';
		cout << endl;
	}

	{
		vector<NestedInteger> nestedList; // [1,[4,[6]]]

		NestedInteger ni1; ni1.setInteger(1); nestedList.push_back(ni1);

		NestedInteger ni2; ni2.setInteger(4); 
		NestedInteger ni3; ni3.setIntList(1, 6);
		vector<NestedInteger> l1; l1.push_back(ni2); l1.push_back(ni3);
		NestedInteger ni4; ni4.setList(l1);
		nestedList.push_back(ni4);

		NestedIterator i(nestedList);
		while (i.hasNext()) cout << i.next() << ' ';
		cout << endl;
	}

	std::cin.get();
	return 0;
}




