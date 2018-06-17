#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <assert.h>
#include <limits.h>
using namespace std;

class Node {
    int start;
    int end;
    vector<int> X;
    Node* left;
    Node* right;
    int count;
    long total;
public:
    Node(int s, int e, vector<int> x)
        :start(s),end(e),X(x),left(NULL),right(NULL),count(0),total(0) {

    }
    ~Node() {
        if (left) {
            delete left;
            left = NULL;
        }
        if (right) {
            delete right;
            right = NULL;
        }
    }
    int getRangeMid() {
        return start + (end - start) / 2;
    }
    Node* getLeft() {
        if (left == NULL) left = new Node(start, getRangeMid(), X);
        return left;
    }
    Node* getRight() {
        if (right == NULL) right = new Node(getRangeMid(), end, X);
        return right;
    }
    long update(int i, int j, int val) {
        if (i >= j) return 0;
        if (start == i && end == j) {
            count += val;
        } else {
            getLeft()->update(i, min(getRangeMid(), j), val);
            getRight()->update(max(getRangeMid(), i), j, val);
        }
        if (count > 0) total = X[end] - X[start];
        else total = getLeft()->total + getRight()->total;
        return total;
    }
};

class Solution {
    struct Event {
        int y;
        int state;
        int xBeg;
        int xEnd;
        Event():y(0),state(0),xBeg(0),xEnd(0){}
        Event(int aY,int aState,int aXBeg,int aXEnd):y(aY),state(aState),xBeg(aXBeg),xEnd(aXEnd){}
        bool operator<(const Event& other) const { return y < other.y; }
    };
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        int M = 1000000007;
        int OPEN = 1;
        int CLOSE = -1;
        vector<Event> events(rectangles.size() * 2, Event());
        set<int> Xvals;
        int t = 0;
        for (int i=0; i<rectangles.size(); ++i) {
            vector<int>& rec = rectangles[i];
            events[t++] = Event(rec[1], OPEN, rec[0], rec[2]);
            events[t++] = Event(rec[3], CLOSE, rec[0], rec[2]);
            Xvals.insert(rec[0]);
            Xvals.insert(rec[2]);
        }
        sort(events.begin(), events.end());
        /* events order by y, Xvals order by x */
        vector<int> X; X.reserve(Xvals.size());
        map<int,int> Xi; t = 0;
        for (set<int>::iterator it=Xvals.begin(); it!=Xvals.end(); ++it) {
            Xi[*it] = t++;
            X.push_back(*it);
        }
        /* X and Xi order by x */
        Node active(0, X.size()-1, X);
        long ans = 0;
        long cur_x_sum = 0;
        int cur_y = events[0].y;
        for (int i=0; i<events.size(); ++i) {
            int y = events[i].y;
            int state = events[i].state;
            int xBeg = events[i].xBeg;
            int xEnd = events[i].xEnd;
            ans += cur_x_sum * (y - cur_y);
            cur_x_sum = active.update(Xi[xBeg], Xi[xEnd], state);
            cur_y = y;
        }
        ans %= M;
        return (int)ans;
    }
};

int main() {

}
