#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	vector<int> asteroidCollision(vector<int>& asteroids) {
		vector<int> res;
		if (asteroids.empty()) return res;
		bool hasCollision = false;
		res = asteroids;
		do {
			hasCollision = false;
			vector<int> tmp;
			size_t first = 0;
			while (first<res.size() && res[first]==0) ++first; /* ignore 0 */
			if (first>=res.size()) break;
			auto sz = res.size();
			tmp.push_back(res[first]);
			for (auto i=first+1; i<sz; ++i) {
				if (res[i]==0) continue; /* ignore 0 */
				if ((res[i] < 0 && tmp.back() < 0) || 
					(res[i] > 0 && tmp.back() > 0) || 
					(tmp.back() < 0 && res[i] > 0)) {
					/* no collision */
					tmp.push_back(res[i]);
				}
				else {
					if (abs(tmp.back()) > abs(res[i])) /* do nothing, the small one explode */;
					else if (abs(tmp.back()) == abs(res[i])) {
						/* both explode */
						tmp.pop_back();
						if (tmp.empty()) { /* tmp should never empty because we call tmp.back() above as default */
							i += 1;
							if (i < res.size()) tmp.push_back(res[i]);
						}
					}
					else {
						/* the small one explode */
						tmp.pop_back();
						tmp.push_back(res[i]);
					}
					hasCollision = true;
				}
			}
			res.swap(tmp);
		} while (hasCollision && res.size()>1);
		return res;
    }
};

void checkVector(vector<int>& v, const vector<int>& expected) {
	auto sz = v.size();
	cout << "result: ";
	for (auto i=0; i<sz; ++i) cout << v[i] << " ";
	cout << endl;
	if (sz != expected.size()) {
		cout << "failed" << endl;
		return;
	}
	for (auto i=0; i<sz; ++i) {
		if (v[i] != expected[i]) {
			cout << "failed" << endl;
			return;
		}
	}
	cout << "successful" << endl;
}

int main() {
#if 1
	{
		vector<int> asteroids = {5,10,-5};
		auto output = Solution().asteroidCollision(asteroids);
		checkVector(output, {5,10});		
	}
	{
		vector<int> asteroids = {8,-8};
		auto output = Solution().asteroidCollision(asteroids);
		checkVector(output, {});		
	}
	{
		vector<int> asteroids = {10,2,-5};
		auto output = Solution().asteroidCollision(asteroids);
		checkVector(output, {10});		
	}
	{
		vector<int> asteroids = {-2,-1,1,2};
		auto output = Solution().asteroidCollision(asteroids);
		checkVector(output, {-2,-1,1,2});		
	}
	{
		vector<int> asteroids = {-2,-2,1,-2};
		auto output = Solution().asteroidCollision(asteroids);
		checkVector(output, {-2,-2,-2});		
	}
	{
		vector<int> asteroids = {1,-1,-2,-2};
		auto output = Solution().asteroidCollision(asteroids);
		checkVector(output, {-2,-2});		
	}
#endif
}
