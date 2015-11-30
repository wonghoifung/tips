#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <assert.h>
#include <time.h>

using namespace std;

class Solution {
public:
    bool wordPattern(string pattern, string str) {
        unordered_map<char, string> mapping;
        unordered_map<string, char> rmapping;
        istringstream iss(str);
        string value;
        for (size_t i = 0; i < pattern.size(); i++)
        {
            iss >> value;
            if (!iss)
            {
                return false;
            }

            char key = pattern[i];

            unordered_map<char, string>::iterator it = mapping.find(key);
            if (it != mapping.end())
            {
                if (it->second != value)
                {
                    return false;
                }
                unordered_map<string, char>::iterator rit = rmapping.find(value);
                if ((rit != rmapping.end() && rit->second != key) || rit == rmapping.end())
                {
                    return false;
                }
                continue;
            }

            unordered_map<string, char>::iterator rit = rmapping.find(value);
            if (rit != rmapping.end())
            {
                return false;
            }

            mapping[key] = value;
            rmapping[value] = key;
        }
        iss >> value;
        return !iss;
    }
};

int main()
{
    Solution s;

    {
        string pattern = "abba", str = "dog cat cat dog";
        cout << s.wordPattern(pattern, str) << endl;
    }

    {
        string pattern = "abba", str = "dog cat cat fish";
        cout << s.wordPattern(pattern, str) << endl;
    }

    {
        string pattern = "aaaa", str = "dog cat cat dog";
        cout << s.wordPattern(pattern, str) << endl;
    }

    {
        string pattern = "abba", str = "dog dog dog dog";
        cout << s.wordPattern(pattern, str) << endl;
    }

    std::cin.get();

    return 0;
}

