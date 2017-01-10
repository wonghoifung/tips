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

class Solution {
public:
	string frequencySort(string s) {
		map<char, int> counts;
		for (char c : s) counts[c] += 1;
		set <pair<int, char>, greater<pair<int, char>>> q;
		for (auto& p : counts) {
			q.insert(make_pair(p.second, p.first));
		}
		stringstream ss;
		for (auto& p : q) {
			ss << string(p.first, p.second);
		}
		return ss.str();
	}
};

int main() {

	cin.get();
	return EXIT_SUCCESS;
}

