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
#include <memory>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <numeric>
#include <initializer_list>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>

using namespace std;

class mytimer
{
private:
	time_t start_;
public:
	mytimer()
	{
		start_ = time(nullptr);
	}
	~mytimer()
	{
		time_t end = time(nullptr);
		cout << end - start_ << "s" << endl;
	}
};

class Solution {
public:
	int char2int(char c) {
		switch (c)
		{
		case 'A': return 0;
		case 'C': return 1;
		case 'G': return 2;
		case 'T': return 3;
		default: assert(0);
		}
	}
	vector<string> findRepeatedDnaSequences(string s) {
		vector<string> res;
		if (s.size() <= 10) return res;
		
		bitset<(1 << 20) - 1> once;
		bitset<(1 << 20) - 1> more;

		int seq = 0;
		for (size_t i = 0; i < 10; ++i)
		{
			seq = (seq << 2) | char2int(s.at(i));
		}
		once.set(seq);

		int mask = (1 << 20) - 1;
		for (size_t i = 10; i < s.size(); ++i)
		{
			seq = (seq << 2) & mask | char2int(s.at(i));
			if (more[seq]) continue;
			if (once[seq])
			{
				res.push_back(s.substr(i - 9 , 10));
				more.set(seq);
			}
			else
			{
				once.set(seq);
			}
		}

		return res;
	}
};

int main()
{
	Solution s;
	{
		string dna = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT";
		vector<string> res = s.findRepeatedDnaSequences(dna);
		copy(res.begin(), res.end(), ostream_iterator<string>(cout, " ")); cout << endl;
	}

	std::cin.get();
	return 0;
}

