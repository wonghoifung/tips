#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <stdlib.h>
#include <limits.h>
using namespace std;
class Solution {
    int maxmin;
    int minmin;
    int toMin(const string& tp) {
        int del = tp.find(':');
        int hour = atoi(tp.substr(0,del).c_str());
        int min = atoi(tp.substr(del+1).c_str());
        return hour*60+min;
    }
    int diff(int ntp1, int ntp2) {
        int nbigger=max(ntp1,ntp2);
        int nsmaller=min(ntp1,ntp2);
        int diff1=nbigger-nsmaller;
        int diff2=maxmin-nbigger+1+nsmaller-minmin;
        return min(diff1,diff2);        
    }
    bool toUMap(const vector<string>& timePoints, unordered_map<string,int>& m) {
        for (auto i=0; i<timePoints.size(); ++i) {
            if (m.count(timePoints[i])>0) return false;
            m[timePoints[i]]=toMin(timePoints[i]);
        }
        return true;
    }
public:
    Solution() {
        maxmin=toMin("23:59");
        minmin=0;
    }
    int findMinDifference(vector<string>& timePoints) {
        int ret=INT_MAX;
        unordered_map<string,int> m;
        if (!toUMap(timePoints,m)) return 0; // there are same time points, minimum diff is 0
        for (auto i=0; i<timePoints.size(); ++i) {
            for (auto j=i+1; j<timePoints.size(); ++j) {
                int d=diff(m[timePoints[i]],m[timePoints[j]]);
                if (d<ret) ret=d;
                if (ret==1) return 1; // minimum diff is 1, no need to continue the loop
            }
        }
        return ret;
    }
};
int main() {
    {
        vector<string> timePoints{"23:59","00:00"};
        cout << Solution().findMinDifference(timePoints) << endl;//1
    }
    {
        vector<string> timePoints{"11:07","08:31","02:56","12:35","23:55","03:11","09:06"};
        cout << Solution().findMinDifference(timePoints) << endl;
    }
}

