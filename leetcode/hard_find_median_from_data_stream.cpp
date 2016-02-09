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

class MedianFinder {
    vector<int> s;
public:
    void insert_sorted(int num) {
        if (s.empty()) {
            s.push_back(num);
        } else {
            int start = 0;
            int end = (int)s.size() - 1;
            int mid = (end - start) / 2 + start;
            while (start < end) {
                if (s[mid] == num) {
                    s.insert(s.begin()+mid, num);
                    return;
                }
                if (s[mid] < num) {
                    start = mid;
                } else {
                    end = mid;
                }
                int oldmid = mid;
                mid = (end - start) / 2 + start;
                if (oldmid==mid) break;
            }
            for (int i=start; i<=end; ++i) {
                if (s[i]>num) {
                    s.insert(s.begin()+i, num);
                    return;
                }
            }
            s.insert(s.begin()+end+1,num);
        }
    }
    
    void addNum(int num) {
        insert_sorted(num);
    }
    
    double findMedian() {
        if (s.size() % 2) { // odd
            return s[s.size()/2];
        } else { // even
            return ((double)s[s.size()/2] + (double)s[s.size()/2-1])/2;
        }
    }
};

int main(int argc, char** argv)
{
    {
        MedianFinder mf;
        mf.addNum(2);
        mf.addNum(3);
        mf.addNum(4);
        cout<<mf.findMedian()<<endl;
    }
    {
        MedianFinder mf;
        mf.addNum(2);
        mf.addNum(3);
        cout<<mf.findMedian()<<endl;
    }
    {
        MedianFinder mf;
        mf.addNum(1);
        mf.addNum(2);
        cout<<mf.findMedian()<<endl;
        mf.addNum(3);
        cout<<mf.findMedian()<<endl;
    }
    return 0;
}

