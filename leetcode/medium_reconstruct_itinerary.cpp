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
    void listAdd(string value, vector<string>& list) {
        if (list.empty()) {
            list.push_back(value);
        } else {
            int i = 0;
            while (i < list.size()) {
                if (value.compare(list[i])<=0) {
                    list.insert(list.begin()+i, value);
                    return;
                }
                i+=1;
            }
            list.push_back(value);
        }
    }
    bool itineraryHelper(string cur, map<string, vector<string>>& m, vector<string>& res, int total, int num) {
        if (num>=total) {
            return true;
        }
        if (m.count(cur)==0 || m[cur].size()==0) {
            return false;
        }
        vector<string>& curlist = m[cur];
        int i=0;
        while (i<curlist.size()) {
            string next = curlist[i];
            curlist.erase(curlist.begin()+i);
            res.push_back(next);
            if (itineraryHelper(next, m, res, total, num+1)) {
                return true;
            }
            res.pop_back();
            listAdd(next, curlist);
            i+=1;
        }
        return false;
    }
    vector<string> findItinerary(vector<pair<string, string>> tickets) {
        vector<string> res;
        if (tickets.empty()) {
            return res;
        }
        int total = (int)tickets.size()+1;
        map<string, vector<string>> m;
        for (int i=0; i<tickets.size(); ++i) {
            auto& tmp = m[tickets[i].first];
            listAdd(tickets[i].second, tmp);
        }
        res.push_back("JFK");
        itineraryHelper("JFK", m, res, total, 1);
        return res;
    }
    
};

int main(int argc, char** argv)
{
    
    
    return 0;
}

