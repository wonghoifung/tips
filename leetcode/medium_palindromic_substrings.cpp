#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Solution {
    void extend(const string& s, int left, int right, int& count) {
        while (left >= 0 && right < s.size() && s[left] == s[right]) {
            count += 1;
            left -= 1;
            right += 1;
        }
    }
public:
    int countSubstrings(string s) {
        if (s.empty()) return 0;
        int count = 0;
        for (int i=0; i<s.size(); ++i) {
            extend(s, i, i, count);
            extend(s, i, i+1, count);
        }
        return count;
    }
};

int main() {

}
