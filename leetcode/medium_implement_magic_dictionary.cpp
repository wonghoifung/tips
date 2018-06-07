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

class MagicDictionary {
    TrieNode* pTrie;

    void reset() {
        if (pTrie) {
            delete pTrie;
            pTrie = NULL;
        }
        pTrie = new TrieNode();
    }

public:
    MagicDictionary():pTrie(NULL) {
        
    }
    
    void buildDict(vector<string> dict) {
        reset();
        for (int i=0; i<dict.size(); ++i) {
            pTrie->insert(dict[i].c_str());
        }
    }
    
    /** Returns if there is any word in the trie that equals to the given word after modifying exactly one character */
    bool search(string word) {
        for (int i=0; i<word.size(); ++i) {
            for (int j=0; j<26; ++j) {
                char newChar = 'a' + j;
                if (newChar == word[i]) continue;
                string temp = word;
                temp[i] = newChar;
                if (pTrie->find(temp.c_str())) return true;
            }
        }
        return false;
    }
};

int main() {
    vector<string> dict = { "hello","leetcode" };
    MagicDictionary md;
    md.buildDict(dict);
    cout << md.search("hello") << endl;
    cout << md.search("hhllo") << endl;
    cout << md.search("hhllo") << endl;
    cout << md.search("leetcoded") << endl;
}
