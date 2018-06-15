#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <assert.h>
#include <limits.h>
using namespace std;

class MyCalendarThree {
    typedef map<pair<int,int>,int> t_mpi; // {start,end} -> k
    t_mpi ranges_;     // all ranges
    set<int> points_;  // all points
    int k_;            // max k
public:
    MyCalendarThree():k_(0) {
        ranges_[make_pair(INT_MIN,INT_MAX)] = 0;
        points_.insert(INT_MIN);
        points_.insert(INT_MAX);
    }
    
    int book(int start, int end) {
        set<int>::iterator itStart = points_.lower_bound(start);
        set<int>::iterator itEnd = points_.lower_bound(end);
        if (itStart == itEnd) { 
            // in the same range
            int e = *itStart;
            --itStart;
            int s = *itStart;
            t_mpi::iterator it = ranges_.find(make_pair(s,e));
            int k = it->second;
            ranges_.erase(it);
            ranges_[make_pair(s,start)] = k;
            if (e == end) {
                ranges_[make_pair(start,e)] = k+1;
                points_.insert(start);
            } else {
                int small = min(e,end);
                int big = max(e,end);
                ranges_[make_pair(start,small)] = k+1;
                ranges_[make_pair(small,big)] = k;
                points_.insert(start);
                points_.insert(small);
            }
            k_ = max(k_, k+1);
        } else {
            // in different range
            if (*itStart != start) {
                int e = *itStart;
                --itStart;
                int s = *itStart;
                t_mpi::iterator it = ranges_.find(make_pair(s,e));
                int k = it->second;
                ranges_.erase(it);
                ranges_[make_pair(s,start)] = k;
                ranges_[make_pair(start,e)] = k+1;
                points_.insert(start);
                itStart = points_.find(e);
                k_ = max(k_, k+1);
            }
            if (*itEnd != end) {
                int e = *itEnd;
                --itEnd;
                int s = *itEnd;
                t_mpi::iterator it = ranges_.find(make_pair(s,e));
                int k = it->second;
                ranges_.erase(it);
                ranges_[make_pair(s,end)] = k+1;
                ranges_[make_pair(end,e)] = k;
                points_.insert(end);
                itEnd = points_.find(s);
                k_ = max(k_, k+1);
            }
            set<int>::iterator it=itStart;
            while (it!=itEnd) {
                int s = *it;
                ++it;
                int e = *it;
                ranges_[make_pair(s,e)] += 1;
                k_ = max(k_, ranges_[make_pair(s,e)]);
            }
        }
        return k_;
    }

    void dump() {
        for (t_mpi::iterator it=ranges_.begin(); it!=ranges_.end(); ++it) {
            cout << "[" << it->first.first << ", " << it->first.second << ") k:" << it->second << endl;
        }
        for (set<int>::iterator it=points_.begin(); it!=points_.end(); ++it) {
            cout << *it << " ";
        }
        cout << "\n-------------\n";
    }
};

int main() {
    MyCalendarThree mct;
    cout << mct.book(10,20) << endl; // 1
    cout << mct.book(50,60) << endl; // 1
    // mct.dump();
    cout << mct.book(10,40) << endl; // 2
    cout << mct.book(5,15) << endl;  // 3
    cout << mct.book(5,10) << endl;  // 3
    cout << mct.book(25,55) << endl; // 3
}

