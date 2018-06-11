#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <sstream>
#include <string.h>

using namespace std;

#define MAX_NODE_SZ 26
class TrieNode {
    string word;
    bool existed;
    struct TrieNode* next[MAX_NODE_SZ];
public:
    TrieNode():existed(false) {
        memset(next, 0, sizeof(next));
    }
    ~TrieNode() {
        for (int i=0;i<MAX_NODE_SZ;++i) {
            if (next[i]) {
                delete next[i];
                next[i] = NULL;
            }
        }
    }
    void insert(const char* s) {
        if (s == NULL || *s == '\0') return;
        const char* org = s;
        int i;
        TrieNode* p = this;
        while (*s != '\0') {
            int idx = *s - 'a';
            if (p->next[idx] == NULL) {
                TrieNode* n = new TrieNode();
                p->next[idx] = n;
                p = p->next[idx];
            } else {
                p = p->next[idx];
            }
            s++;
        }
        p->existed = true;
        p->word = org;
    }
    bool find(const char* s, bool* prefix = NULL) {
        TrieNode* p = this;
        while (p != NULL && *s != '\0') {
            p = p->next[*s - 'a'];
            s++;
        }
        if (prefix) *prefix = p;
        return p && p->existed;
    }
    string dfsFindLongestWord() {
        string res(this->word);
        for (int i=0; i<MAX_NODE_SZ; ++i) {
            if (this->next[i] && this->next[i]->existed) {
                const string& curRes = this->next[i]->dfsFindLongestWord();
                if (curRes.size() > res.size() || (curRes.size() == res.size() && curRes < res)) {
                    res = curRes;
                }
            }  
        }
        return res;
    }
};

class Solution {
public:
    string longestWord(vector<string>& words) {
        TrieNode trie;
        for (int i=0; i<words.size(); ++i) {
            trie.insert(words[i].c_str());
        }
        return trie.dfsFindLongestWord();
    }
};

int main() {
    vector<string> words = {"w","wo","wor","worl","world"};
    cout << Solution().longestWord(words) << endl;
}
