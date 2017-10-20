#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <stdlib.h> 

using namespace std;

class Solution {
	bool toSum(const std::string& s) { return s.size() == 1 && s[0] == '+'; }
	bool toDouble(const std::string& s) { return s.size() == 1 && s[0] == 'D'; }
	bool toRemove(const std::string& s) { return s.size() == 1 && s[0] == 'C'; }
	int toInt(const std::string& s) { stringstream ss; ss << s; int i = 0; ss >> i; return i; }
public:
    int calPoints(vector<string>& ops) {
		deque<int> q;
		int sum=0;
		for (int i=0; i<ops.size(); ++i) {
			if (toSum(ops[i])) {
				if (q.size() >= 2) {
					int n = q[q.size()-2] + q[q.size()-1];
					sum += n;
					q.push_back(n);
				} else abort();
			}
			else if (toDouble(ops[i])) {
				if (q.size()) {
					int n = q.back() * 2;
					sum += n;
					q.push_back(n);
				} else abort();
			}
			else if (toRemove(ops[i])) {
				if (q.size()) {
					int n = q.back();
					sum -= n;
					q.pop_back();
				} else abort();
			}
			else {
				int n = toInt(ops[i]);
				sum += n;
				q.push_back(n);
			}
		}
		return sum;        
    }
};

int main() {
	vector<string> arr = {"5","-2","4","C","D","9","+","+"};	
	cout << Solution().calPoints(arr) << endl;
}
