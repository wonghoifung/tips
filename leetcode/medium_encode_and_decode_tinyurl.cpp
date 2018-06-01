#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <functional>
#include <stdlib.h>
using namespace std;

class Solution {
    unordered_map<string,string> decoder;
public:
    Solution() {
        srand(time(NULL));
    }
    string encode(string longUrl) {
        auto r = rand() % 10000;
        auto h = hash<string>{}(longUrl);
        stringstream ss;
        ss << "http://" << h << r;
        cout << ss.str() << endl;
        decoder[ss.str()]=longUrl;
        return ss.str();
    }
    string decode(string shortUrl) {
        return decoder[shortUrl];
    }
};

int main() {
    {
        Solution s;
        cout<<s.decode(s.encode("http://www.google.com"))<<endl;
    } 
}
