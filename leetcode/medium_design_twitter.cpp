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
#include <stdarg.h>

using namespace std;

class Twitter {
	unordered_map<int, unordered_set<int>> followers;
	vector<pair<int, int>> tweets;

public:
	/** Initialize your data structure here. */
	Twitter() {

	}

	/** Compose a new tweet. */
	void postTweet(int userId, int tweetId) {
		tweets.push_back({ userId, tweetId });
	}

	/** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
	vector<int> getNewsFeed(int userId) {
		vector<int> res;
		for (int i = ((int)tweets.size()) - 1; i >= 0; --i) {
			auto tweetuser = tweets[i].first;
			if (tweetuser == userId || followers[tweetuser].count(userId)) {
				res.push_back(tweets[i].second);
				if (res.size() == 10) break;
			}
		}
		return res;
	}

	/** Follower follows a followee. If the operation is invalid, it should be a no-op. */
	void follow(int followerId, int followeeId) {
		followers[followeeId].insert(followerId);
	}

	/** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
	void unfollow(int followerId, int followeeId) {
		followers[followeeId].erase(followerId);
	}
};

int main()
{
	auto printIntVector = [](const vector<int>& vec) {
		cout << "[";
		for (auto i : vec) {
			cout << i << " ";
		}
		if (vec.empty())
			cout << "]" << endl;
		else
			cout << "\b]" << endl;
	};

	{
		/*
  		["Twitter","postTweet","getNewsFeed","follow","getNewsFeed","unfollow","getNewsFeed"]
  		       [[],      [1,1],          [1],   [2,1],          [2],     [2,1],         [2]]
  		*/
		Twitter twitter;
		
		twitter.postTweet(1, 1);

		auto v = twitter.getNewsFeed(1);
		printIntVector(v); // [1]

		twitter.follow(2, 1);

		v = twitter.getNewsFeed(2);
		printIntVector(v); // [1]

		twitter.unfollow(2, 1);

		v = twitter.getNewsFeed(2);
		printIntVector(v); // []

		cout << endl;
	}

	Twitter twitter;

	/* User 1 posts a new tweet (id = 5). */
	twitter.postTweet(1, 5);

	/* User 1's news feed should return a list with 1 tweet id -> [5]. */
	vector<int> newsfeed = twitter.getNewsFeed(1);
	printIntVector(newsfeed);

	/* User 1 follows user 2. */
	twitter.follow(1, 2);

	/* User 2 posts a new tweet (id = 6). */
	twitter.postTweet(2, 6);

	/* User 1's news feed should return a list with 2 tweet ids -> [6, 5].
  	   Tweet id 6 should precede tweet id 5 because it is posted after tweet id 5. */
	newsfeed = twitter.getNewsFeed(1);
	printIntVector(newsfeed);

	/* User 1 unfollows user 2. */
	twitter.unfollow(1, 2);

	/* User 1's news feed should return a list with 1 tweet id -> [5],
  	   since user 1 is no longer following user 2. */
	newsfeed = twitter.getNewsFeed(1);
	printIntVector(newsfeed);

	std::cin.get();
	return 0;
}




