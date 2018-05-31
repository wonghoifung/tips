#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
		map<string, int> counts;
		for (int i=0; i<words.size(); ++i) {
			counts[words[i]] += 1;
		}
		map<int, set<string>, greater<int> > rcounts;
		for (map<string, int>::iterator it=counts.begin(); it!=counts.end(); ++it) {
			rcounts[it->second].insert(it->first);
		}
		vector<string> res;
		int c = 0;
		for (map<int, set<string>, greater<int> >::iterator it=rcounts.begin(); it!=rcounts.end(); ++it) {
			if (c == k) break;
			for (set<string>::iterator it2=it->second.begin(); it2!=it->second.end(); ++it2) {
				if (c == k) break;
				res.push_back(*it2);
				c += 1;
			}
		}
		return res;
    }
};

int main() {

}
