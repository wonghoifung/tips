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

class Iterator {
    struct Data;
    Data* data;
public:
    Iterator(const vector<int>& nums);
    Iterator(const Iterator& iter);
    virtual ~Iterator();
    int next();
    bool hasNext() const;
};


class PeekingIterator : public Iterator {
    bool hasnextelement;
    int nextelement;
public:
    PeekingIterator(const vector<int>& nums) : Iterator(nums) {
        setNext();
    }
    
    void setNext() {
        hasnextelement = Iterator::hasNext();
        if (hasnextelement) {
            nextelement = Iterator::next();
        } else {
            nextelement = 0;
        }
    }
    
    int peek() {
        return nextelement;
    }
    
    int next() {
        int res = nextelement;
        setNext();
        return res;
    }
    
    bool hasNext() const {
        return hasnextelement;
    }
};

int main(int argc, char** argv)
{


    return 0;
}

