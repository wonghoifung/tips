#include <iostream>
#include <vector>
#include <map>
#include <set>
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
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        int M = 1000000007;
        int OPEN = 1;
        int CLOSE = -1;
        vector<vector<int>> events(rectangles.size() * 2, vector<int>());
        set<int> Xvals;
        int t = 0;
        for (int i=0; i<rectangles.size(); ++i) {
            vector<int>& rec = rectangles[i];
            events[t++] = {rec[1], OPEN, rec[0], rec[2]};
            events[t++] = {rec[3], CLOSE, rec[0], rec[2]};
            Xvals.insert(rec[0]);
            Xvals.insert(rec[2]);
        }

        // TODO

        return 0;
    }
};

int main() {

}
/*
class Solution {
    public int rectangleArea(int[][] rectangles) {
        int OPEN = 1, CLOSE = -1;
        int[][] events = new int[rectangles.length * 2][];
        Set<Integer> Xvals = new HashSet();
        int t = 0;
        for (int[] rec: rectangles) {
            events[t++] = new int[]{rec[1], OPEN, rec[0], rec[2]};
            events[t++] = new int[]{rec[3], CLOSE, rec[0], rec[2]};
            Xvals.add(rec[0]);
            Xvals.add(rec[2]);
        }

        Arrays.sort(events, (a, b) -> Integer.compare(a[0], b[0]));

        Integer[] X = Xvals.toArray(new Integer[0]);
        Arrays.sort(X);
        Map<Integer, Integer> Xi = new HashMap();
        for (int i = 0; i < X.length; ++i)
            Xi.put(X[i], i);

        Node active = new Node(0, X.length - 1, X);
        long ans = 0;
        long cur_x_sum = 0;
        int cur_y = events[0][0];

        for (int[] event: events) {
            int y = event[0], typ = event[1], x1 = event[2], x2 = event[3];
            ans += cur_x_sum * (y - cur_y);
            cur_x_sum = active.update(Xi.get(x1), Xi.get(x2), typ);
            cur_y = y;

        }

        ans %= 1_000_000_007;
        return (int) ans;
    }
}

class Node {
    int start, end;
    Integer[] X;
    Node left, right;
    int count;
    long total;

    public Node(int start, int end, Integer[] X) {
        this.start = start;
        this.end = end;
        this.X = X;
        left = null;
        right = null;
        count = 0;
        total = 0;
    }

    public int getRangeMid() {
        return start + (end - start) / 2;
    }

    public Node getLeft() {
        if (left == null) left = new Node(start, getRangeMid(), X);
        return left;
    }

    public Node getRight() {
        if (right == null) right = new Node(getRangeMid(), end, X);
        return right;
    }

    public long update(int i, int j, int val) {
        if (i >= j) return 0;
        if (start == i && end == j) {
            count += val;
        } else {
            getLeft().update(i, Math.min(getRangeMid(), j), val);
            getRight().update(Math.max(getRangeMid(), i), j, val);
        }

        if (count > 0) total = X[end] - X[start];
        else total = getLeft().total + getRight().total;

        return total;
    }
}
*/
