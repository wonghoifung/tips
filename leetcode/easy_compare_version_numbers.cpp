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

class Solution {
public:
	int compareVersion(string version1, string version2) {
		int start1 = 0;
		int start2 = 0;

		while (true)
		{
			int end1 = version1.find_first_of('.', start1);
			string v1;
			if (end1 == string::npos) v1 = version1.substr(start1);
			else v1 = version1.substr(start1, end1 - start1 + 1);

			int end2 = version2.find_first_of('.', start2);
			string v2;
			if (end2 == string::npos) v2 = version2.substr(start2);
			else v2 = version2.substr(start2, end2 - start2 + 1);

			int iv1 = stoi(v1);
			int iv2 = stoi(v2);

			if (iv1 == iv2) {
				if (end1 != string::npos && end2 != string::npos) {
					start1 = end1 + 1;
					start2 = end2 + 1;
					continue;
				}
				if (end1 == string::npos && end2 == end1) return 0;
				if (end1 == string::npos) {
					for (int i = end2 + 1; i < version2.size(); ++i) if (version2[i] <= '9' && version2[i] >= '1') return -1;
					return 0;
				}
				else {
					for (int i = end1 + 1; i < version1.size(); ++i) if (version1[i] <= '9' && version1[i] >= '1') return 1;
					return 0;
				}
			}
			else {
				if (iv1 > iv2) return 1;
				else return -1;
			}
		}
		
		return 0; // never reach here
	}
};

int main()
{
	Solution s;
	{
		cout << s.compareVersion("1.0", "1") << endl;
	}

	std::cin.get();
	return 0;
}




