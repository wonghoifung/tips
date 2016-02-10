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

class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        int reml=0, remr=0;
        unordered_set<string> res;
        for(int i=0; i<s.size(); i++){
            if(s[i]=='(') reml+=1;
            else if(s[i]==')'){
                if(reml > 0) reml-=1;
                else remr+=1;
            }
        }
        help(0, 0, reml, remr, s, "", res);
        return vector<string>(res.begin(), res.end());
    }
    
    void help(int pair, int idx, int reml, int remr, const string& s, string cur, unordered_set<string>& res){
        if(idx==s.size()){
            if(pair==0 && reml==0 && remr==0) res.insert(cur);
            return;
        }
        if(s[idx]=='('){
            if(reml > 0) help(pair, idx+1, reml-1, remr, s, cur, res);
            help(pair+1, idx+1, reml, remr, s, cur+s[idx], res);
        }
        else if(s[idx]==')'){
            if(remr > 0) help(pair, idx+1, reml, remr-1, s, cur, res);
            if(pair > 0) help(pair-1, idx+1, reml, remr, s, cur+s[idx], res);
        }
        else{
            help(pair, idx+1, reml, remr, s, cur+s[idx], res);
        }
    }
};

int main(int argc, char** argv)
{
    Solution s;
    {
        auto v = s.removeInvalidParentheses("()())()");
        cout<<endl;
        copy(v.begin(), v.end(), ostream_iterator<string>(cout, "\n"));
    }
    return 0;
}

