#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    string getHint(string secret, string guess) {
        vector<int> idxes(10,0);
        for (int i=0; i<secret.size(); ++i) {
            idxes[secret.at(i)-'0']+=1;
        }
        int acnt=0;
        int bcnt=0;
        for (int i=0; i<guess.size(); ++i) {
            if (guess.at(i)==secret.at(i)) {
                acnt+=1;
                if (idxes[secret.at(i)-'0']==0) {
                    bcnt-=1;
                }
                else {
                    idxes[secret.at(i)-'0']-=1;
                }
            }
            else if (idxes[guess.at(i)-'0']>0) {
                bcnt+=1;
                idxes[guess.at(i)-'0']-=1;
            }
        }
        stringstream ss;
        ss<<acnt<<"A"<<bcnt<<"B";
        return ss.str();
    }
    
};

int main(int argc, char** argv)
{
    Solution s;
    {
        cout<<s.getHint("1807", "7810")<<endl;
        cout<<s.getHint("1123", "0111")<<endl;
    }
    
    return 0;
}

