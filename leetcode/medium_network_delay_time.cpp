#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <limits.h>
using namespace std;

class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int N, int K) {
		vector<int> waits(N+1,INT_MAX);
 		map<int,map<int,int>> adj;
		for (auto it=times.begin(); it!=times.end(); ++it) {
			auto& uvw = *it;
			adj[uvw[0]][uvw[1]] = uvw[2];
		}
		queue<int> q;
		q.push(K);
		waits[K] = 0;
		while (!q.empty()) {
			set<int> set;
			for (int n=q.size(); n>0; n--) {
				int u=q.front(); q.pop();
				for (auto it=adj[u].begin(); it!=adj[u].end(); ++it) {
					auto& nb = *it;
					int v=nb.first;
					if (waits[u] + adj[u][v] < waits[v]) {
						if (!set.count(v)) {
							set.insert(v);
							q.push(v);
						}
						waits[v] = waits[u] + adj[u][v];
					}
				}
			}
		}
		int maxwait = 0;
		for (int i=1; i<=N; i++) maxwait = max(maxwait, waits[i]);
		return maxwait == INT_MAX ? -1 : maxwait;
    }
};

int main() {
}
