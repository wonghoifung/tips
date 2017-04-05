#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
void printVec(const vector<string>& v) {
	for (int i=0;i<v.size();++i) cout<<v[i]<<" ";
	cout<<endl;
}
bool complen(const string& a,const string& b) {
	if (a.size() != b.size()) return a.size()>b.size();
	return a < b;
}
class Solution {
public:
    string findLongestWord(string s, vector<string>& d) {
    sort(d.begin(),d.end(),complen);
		//printVec(d);
		for (auto i=0; i<d.size(); ++i) {
			const string& target = d[i];
			size_t pos=0;
			size_t j=0;
			for (j=0; j<target.size(); ++j) {
				auto idx = s.find(target[j],pos);
				if (idx==string::npos) break;
				pos=idx+1;
			}
			if (j==target.size()) return target;
		}
		return "";
    }
};
int main() {
	{
		string s="abpcplea";
		vector<string> d={"ale","apple","monkey","plea"};
		cout<<Solution().findLongestWord(s,d)<<endl;//apple
	}
	{
		string s="abpcplea";
		vector<string> d={"b","a","c"};
		cout<<Solution().findLongestWord(s,d)<<endl;//a
	}
}

