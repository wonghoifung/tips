#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <array>
#include <memory>
#include <sstream>
#include <functional>
#include <algorithm>
#include <bitset>
#include <tuple>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

using namespace std;

class Solution {
public:
	vector<string> fullJustify(vector<string>& words, int maxWidth) {
		if (words.size() <= 1) {
			string s = words[0];
			int spacecnt = maxWidth - s.size();
			if (spacecnt > 0) s += string(spacecnt, ' ');
			return { s };
		}
		if (maxWidth <= 1) {
			return words;
		}

		vector<string> res;
		
		vector<string> currentline;
		currentline.push_back(words[0]);
		int currenttotallen = words[0].size();

		for (int i = 1; i < words.size(); ++i)
		{
			const auto& s = words[i];
			if (currenttotallen + 1 + s.size() > maxWidth) {
				
				int spacecnt = maxWidth - currenttotallen;
				if (currentline.size() == 1) { 
					res.push_back(currentline.back());
					res.back() += string(spacecnt, ' ');
				}
				else { 
					int slotcnt = currentline.size() - 1;
					int avg = (slotcnt + spacecnt) / slotcnt;
					int rem = (slotcnt + spacecnt) % slotcnt;
					stringstream ss;
					ss << currentline[0];
					for (int i = 1; i < currentline.size(); ++i)
					{
						if (i - 1 < rem) ss << string(avg + 1, ' ');
						else ss << string(avg, ' ');
						ss << currentline[i];
					}
					res.push_back(ss.str());
				}
				currentline.clear();
				currentline.push_back(s);
				currenttotallen = s.size();
			}
			else if (currenttotallen + 1 + s.size() == maxWidth) {
				currentline.push_back(s);
				stringstream ss;
				copy(currentline.begin(), currentline.end(), ostream_iterator<string>(ss, " "));
				res.push_back(ss.str().substr(0, ss.str().size() - 1));
				if (i == words.size() - 1) break;
				currentline.clear();
				currentline.push_back(words[i + 1]);
				currenttotallen = words[i + 1].size();
				i += 1;
			}
			else if (currenttotallen + 1 + s.size() < maxWidth) {
				currentline.push_back(s);
				currenttotallen += (1 + s.size());
			}
		}

		if (!currentline.empty()) {
			stringstream ss;
			copy(currentline.begin(), currentline.end(), ostream_iterator<string>(ss, " "));
			res.push_back(ss.str().substr(0, ss.str().size() - 1));
			int spacecnt = maxWidth - res.back().size();
			if (spacecnt > 0) res.back() += string(spacecnt, ' ');
		}

		return res;
	}
};

int main()
{
	Solution s;
	{
		vector<string> words{ "This", "is", "an", "example", "of", "text", "justification." };
		auto vs = s.fullJustify(words, 16);
		copy(vs.begin(), vs.end(), ostream_iterator<string>(cout, "\n"));
	}
	{
		vector<string> words{ "a", "b", "c", "d", "e" };
		auto vs = s.fullJustify(words, 1);
		copy(vs.begin(), vs.end(), ostream_iterator<string>(cout, "\n"));
	}
	{
		vector<string> words{ "Here", "is", "an", "example", "of", "text", "justification." };
		auto vs = s.fullJustify(words, 14);
		copy(vs.begin(), vs.end(), ostream_iterator<string>(cout, "\n"));
	}
	{
		vector<string> words{ "" };
		auto vs = s.fullJustify(words, 2);
		copy(vs.begin(), vs.end(), ostream_iterator<string>(cout, "\n"));
	}
	{
		vector<string> words{ "a", "b", "c", "d", "e" };
		auto vs = s.fullJustify(words, 3);
		copy(vs.begin(), vs.end(), ostream_iterator<string>(cout, "\n"));
	}

	std::cin.get();
	return 0;
}




