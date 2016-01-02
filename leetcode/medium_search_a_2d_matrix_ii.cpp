#include <iostream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty()) return false;
        int r = 0;
        int c =(int)matrix[r].size() - 1;
        while (r<matrix.size() && c>=0) {
            if (matrix[r][c] == target) {
                return true;
            } else if (matrix[r][c] < target) {
                r += 1;
            } else {
                c -= 1;
            }
        }
        return false;
    }
    
};

int main(int argc, char** argv)
{
    Solution s;
    (void)s;
    
    return 0;
}

