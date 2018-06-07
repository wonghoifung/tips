#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>

using namespace std;

#define MAX_NODE_SZ 26
class TrieNode {
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
};

class Solution {
public:
    string replaceWords(vector<string>& dict, string sentence) {
        TrieNode trie;
        for (int i=0; i<dict.size(); ++i) {
            trie.insert(dict[i].c_str());
        }
        stringstream ss;
        int start=0;
        for (int i=0; i<=sentence.size(); ++i) {
            if (i<sentence.size() && sentence[i] != ' ') continue;

            std::string sub = sentence.substr(start, i - start);
            if (!sub.empty()) {
                bool found = false;
                for (int len=1; len<=sub.size(); ++len) {
                    bool prefix = false;
                    std::string s = sub.substr(0,len);
                    if (trie.find(s.c_str(), &prefix)) {
                        if (start) ss << ' ';
                        ss << s;
                        found = true;
                        break;
                    }
                    if (!prefix) break;
                }
                if (!found) {
                    if (start) ss << ' ';
                    ss << sub;
                }
            }

            start = i + 1;
            while (start<sentence.size() && sentence[start]==' ') start+=1;
            if (start >= sentence.size()) break;
        }
        return ss.str();
    }
};

int main() {
    vector<string> dict;
    dict.push_back("cat");
    dict.push_back("bat");
    dict.push_back("rat");
    string sentence = "the cattle was r rattled by the battery";
    cout << Solution().replaceWords(dict,sentence) << endl;
}
