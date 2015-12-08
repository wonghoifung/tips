#include <iostream>
#include <vector>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class Solution {
public:
    struct graph {
        unordered_map<int, vector<int>> allpres;
        vector<int> visited;
    };
    bool dfs(graph& g, int n) {
        g.visited[n] = 1;
        for (auto i : g.allpres[n]) {
            if (g.visited[i] == 1) {
                return false;
            }
            if (g.visited[i] == 0) {
                if (!dfs(g, i)) {
                    return false;
                }
            }
        }
        g.visited[n] = 2;
        return true;
    }
    bool canFinish(int numCourses, vector<pair<int, int>>& prerequisites) {
        if (numCourses < 2) {
            return true;
        }
        graph g;
        g.visited.resize(numCourses, 0);
        for (auto& p : prerequisites) {
            g.allpres[p.first].push_back(p.second);
        }
        for (int i=0; i<numCourses; ++i) {
            if (g.visited[i] == 0) {
                if (!dfs(g, i)) {
                    return false;
                }
            }
        }
        return true;
    }
};

int main(int argc, char** argv)
{
    Solution s;
    {
        int numCourses = 8;
        vector<pair<int, int>> prerequisites{{1,0},{2,6},{1,7},{6,4},{7,0},{0,5}};
        cout << boolalpha << s.canFinish(numCourses, prerequisites) << endl;
    }
    
    {
        int numCourses = 2;
        vector<pair<int, int>> prerequisites{{0,1},{1,0}};
        cout << boolalpha << s.canFinish(numCourses, prerequisites) << endl;
    }
    
    {
        int numCourses = 2;
        vector<pair<int, int>> prerequisites{{1,0}};
        cout << boolalpha << s.canFinish(numCourses, prerequisites) << endl;
    }
    
    {
        int numCourses = 3;
        vector<pair<int, int>> prerequisites{{0,1},{0,2},{1,2}};
        cout << boolalpha << s.canFinish(numCourses, prerequisites) << endl;
    }
    
    return 0;
}

