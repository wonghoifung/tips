#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <limits.h>

using namespace std;

class Solution {
public:
	vector<int> dailyTemperatures(vector<int>& temperatures) {
		map<int,int> temIndex;
		vector<int> res(temperatures.size(), 0);
		for (int i=temperatures.size()-1; i>=0; --i) {
			int tem = temperatures[i];
			int diff = INT_MAX;
			for (int bigger=tem+1; bigger<101; ++bigger) {
				map<int,int>::iterator it = temIndex.find(bigger);
				if (it != temIndex.end()) {
					diff = min(diff, it->second - i);
				}
			}
			if (diff != INT_MAX) res[i] = diff;
			temIndex[temperatures[i]] = i;
		}
		return res;
	}
};

int main() {

}
