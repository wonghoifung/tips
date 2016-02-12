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

int main(int argc, char** argv)
{
    vector<int> nums{0,9,8,3,5,5,5,7,2,1};
    int n=(int)nums.size();
    
    vector<int>::iterator nth = next(nums.begin(), n / 2);
    nth_element(nums.begin(), nth, nums.end());
    int median = *nth;
    cout<<"median:"<<median<<endl;
    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout," "));
    cout<<endl;
    
    int first = 0, mid = 0, last = n - 1;
    cout<<"first:"<<first<<",mid:"<<mid<<",last:"<<last<<endl<<endl;
    while (mid <= last) {
        if (nums[mid] > median) {
            copy(nums.begin(), nums.end(), ostream_iterator<int>(cout," "));
            cout<<endl;
            swap(nums[first], nums[mid]);
            ++first;
            ++mid;
            copy(nums.begin(), nums.end(), ostream_iterator<int>(cout," "));
            cout<<endl;
            cout<<">>>>>>>first:"<<first<<",mid:"<<mid<<",last:"<<last<<endl<<endl;
        }
        else if (nums[mid] < median) {
            swap(nums[mid], nums[last]);
            --last;
            copy(nums.begin(), nums.end(), ostream_iterator<int>(cout," "));
            cout<<endl;
            cout<<"<<<<<<<first:"<<first<<",mid:"<<mid<<",last:"<<last<<endl<<endl;
        }
        else {
            ++mid;
            copy(nums.begin(), nums.end(), ostream_iterator<int>(cout," "));
            cout<<endl;
            cout<<"=======first:"<<first<<",mid:"<<mid<<",last:"<<last<<endl<<endl;
        }
    }
    
    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout," "));
    cout<<endl;
    
    return 0;
}

