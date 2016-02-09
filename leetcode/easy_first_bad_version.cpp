#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <set>
#include <map>

using namespace std;

bool isBadVersion(int version);

class Solution {
public:
    int firstBadVersion(int n) {
        int start = 1;
        int end = n;
        int mid = (end - start) / 2 + start;
        while (start < end) {
            if (isBadVersion((int)mid)) {
                end = mid;
            } else {
                start = mid;
            }
            int oldmid = mid;
            mid = (end - start) / 2 + start;
            if (oldmid == mid) {
                break;
            }
        }
        if (isBadVersion((int)mid)) {
            return (int)mid;
        } else {
            return (int)mid + 1;
        }
    }
    
};

bool isBadVersion(int version) {
    if (version >= 1702766719) return true;
    return false;
}

int main(int argc, char** argv)
{
    Solution s;
    {
        cout << s.firstBadVersion(2126753390) << endl;
    }
    return 0;
}

