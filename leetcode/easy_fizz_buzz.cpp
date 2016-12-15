#include <vector>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#ifdef _WIN32
#define itoa _itoa_s
#define sprintf sprintf_s
#endif

class Solution {
public:
	vector<string> fizzBuzz(int n) {
		vector<string> res;
		char buffer[33];
		for (int i = 1; i <= n; ++i) {
			if (i % 15 == 0) {
				res.push_back("FizzBuzz");
			}
			else if (i % 5 == 0) {
				res.push_back("Buzz");
			}
			else if (i % 3 == 0) {
				res.push_back("Fizz");
			}
			else {
				sprintf(buffer, "%d", i);
				res.push_back(buffer);
			}
		}
		return res;
	}
};

int main() {
	{
		auto v = Solution().fizzBuzz(15);
		for (auto& s : v) {
			cout << s << endl;
		}
		cout << endl;
	}

	cin.get();
	return EXIT_SUCCESS;
}

