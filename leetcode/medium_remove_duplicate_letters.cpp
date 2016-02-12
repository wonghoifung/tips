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
    string removeDuplicateLetters(string s) {
        int res[26] = {0};
        bool visited[26] = {false};
        for (char c:s) res[c-'a']+=1;
        stack<char> st;
        int idx;
        for (char c:s) {
            idx = c-'a';
            res[idx]-=1;
            if (visited[idx]) {
                continue;
            }
            while (!st.empty() && c<st.top() && res[st.top()-'a']!=0) {
                visited[st.top()-'a']=false;
                st.pop();
            }
            st.push(c);
            visited[idx]=true;
        }
        string ans;
        while (!st.empty()) {
            ans = st.top() + ans;
            st.pop();
        }
        return ans;
    }
    
};

int main(int argc, char** argv)
{
    Solution s;
    {
        cout << s.removeDuplicateLetters("cbacdcbc") << endl; // acdb
    }
    return 0;
}

