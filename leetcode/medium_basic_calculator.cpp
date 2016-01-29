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
	int calculate(string s) {
		int len = s.size();
		int sign = 1;
		int result = 0;
		stack<int> st;
		for (int i = 0; i < len; ++i) {
			if (isdigit(s[i])) {
				int sum = s[i] - '0';
				while (i + 1 < len && isdigit(s[i + 1]))
				{
					sum = sum * 10 + s[i + 1] - '0';
					i += 1;
				}
				result += sum * sign;
			}
			else if (s[i] == '+') {
				sign = 1;
			}
			else if (s[i] == '-') {
				sign = -1;
			}
			else if (s[i] == '(') {
				st.push(result);
				st.push(sign);
				result = 0;
				sign = 1;
			}
			else if (s[i] == ')') {
				result *= st.top();
				st.pop();
				result += st.top();
				st.pop();
			}
		}
		return result;
	}
};

int main()
{
	Solution s;
	{
		cout << s.calculate("1-(3+5-2+(3+19-(3-1-4+(9-4-(4-(1+(3)-2)-5)+8-(3-5)-1)-4)-5)-4+3-9)-4-(3+2-5)-10") << endl;
	}

	std::cin.get();
	return 0;
}




