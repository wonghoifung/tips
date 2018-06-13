#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;

class RangeModule {
    typedef set<pair<int,int>> t_mpi;
    t_mpi ranges_;
public:
    RangeModule() {
        
    }
    
    void addRange(int left, int right) {
        if (ranges_.empty()) {
            ranges_.insert(make_pair(left,right));
            return;
        }
        t_mpi::iterator it = ranges_.upper_bound(make_pair(left, left));
        if (it != ranges_.begin() && (--it)->second < left) ++it;
        while (it != ranges_.end() && it->first <= right) {
            if (left > it->first) left = it->first;
            if (right < it->second) right = it->second;
            ranges_.erase(it++);
        }
        if (left < right) ranges_.insert(make_pair(left,right));
    }
    
    bool queryRange(int left, int right) {
        if (ranges_.empty()) return false;
        t_mpi::iterator it = ranges_.lower_bound(make_pair(left, left));
        if (it != ranges_.begin() && (--it)->second < left) ++it;
        while (it != ranges_.end() && it->first <= right) {
            if (left >= it->first) left = it->second;
            if (right <= it->second) right = it->first;
            ++it;
        }
        return right <= left;
    }
    
    void removeRange(int left, int right) {
        if (ranges_.empty()) return;
        t_mpi::iterator it = ranges_.lower_bound(make_pair(left, left));
        if (it != ranges_.begin() && (--it)->second < left) ++it;
        vector<pair<int,int>> tmp;
        while (it != ranges_.end() && it->first <= right) {
            if (left > it->first) tmp.push_back(make_pair(it->first, left));
            if (right < it->second) tmp.push_back(make_pair(right, it->second));
            ranges_.erase(it++);
        }
        for (int i=0; i<tmp.size(); ++i) {
            ranges_.insert(tmp[i]);
        }
    }

    void dump() {
        for (t_mpi::iterator it=ranges_.begin(); it!=ranges_.end(); ++it) {
            cout << "[" << it->first << ", " << it->second << ")" << endl;
        }
        cout << "-------------" << endl;
    }
};

int main() {
    RangeModule rm;
    rm.addRange(10,20);
    rm.dump();
    rm.removeRange(14,16);
    rm.dump();
    cout << rm.queryRange(10,14) << endl; // 1
    cout << rm.queryRange(13,15) << endl; // 0
    cout << rm.queryRange(16,17) << endl; // 1
}

