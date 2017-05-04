#include <iostream>
#include <vector>
#include <limits.h>
using namespace std;
typedef vector<vector<int>> MT;
class Solution {
public:
    MT updateMatrix(MT& matrix) {
        int ROWSIZE = matrix.size();
        int COLSIZE = matrix[0].size();
        MT res(ROWSIZE, vector<int>(COLSIZE,INT_MAX));
        for (int times=0; times<=1; ++times) {
            for (int i=times?ROWSIZE-1:0; times?i>=0:i<ROWSIZE; times?--i:++i) {
                for (int j=times?COLSIZE-1:0; times?j>=0:j<COLSIZE; times?--j:++j) {
                    if (matrix[i][j]==0) res[i][j]=0;
                    else {
                        if (i && res[i-1][j] != INT_MAX && res[i][j] > res[i-1][j] + 1) res[i][j] = res[i-1][j] + 1;
                        if (j && res[i][j-1] != INT_MAX && res[i][j] > res[i][j-1] + 1) res[i][j] = res[i][j-1] + 1;
                        if (i < ROWSIZE-1 && res[i+1][j] != INT_MAX && res[i][j] > res[i+1][j] + 1) res[i][j] = res[i+1][j] + 1;
                        if (j < COLSIZE-1 && res[i][j+1] != INT_MAX && res[i][j] > res[i][j+1] + 1) res[i][j] = res[i][j+1] + 1;
                    }
                }
            }
        }
        return res;
    }
};
int main() {
    {
        MT matrix={{0,0,0},{0,1,0},{0,0,0}};
        MT res=Solution().updateMatrix(matrix);
        for (auto i=0; i<res.size(); ++i) {
            for (auto j=0; j<res[i].size(); ++j) {
                cout << res[i][j] << " ";
            }
            cout << endl;
        }
    }
}
