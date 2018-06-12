#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string.h>

using namespace std;

#define MAX_NODE_SZ 26
class TrieNode {
    bool existed;
    struct TrieNode* next[MAX_NODE_SZ];
    set<int> weights;
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
    void insert(const char* s, int beg, int end, int step, int weight) {
        if (s == NULL || *s == '\0') return;
        int i;
        TrieNode* p = this;
        for (int i=beg; i!=end; i+=step) {
            int idx = s[i] - 'a';
            if (p->next[idx] == NULL) {
                TrieNode* n = new TrieNode();
                p->next[idx] = n;
            }
            p = p->next[idx];
            p->weights.insert(weight);
        }
        p->existed = true;
    }
    bool find(const char* s, int beg, int end, int step, bool* prefix = NULL) {
        TrieNode* p = this;
        for (int i=beg; i!=end && p!=NULL; i+=step) {
            int idx = s[i] - 'a';
            p = p->next[idx];
        }
        if (prefix) *prefix = p;
        return p && p->existed;
    }
    set<int>* findWeight(const char* s, int beg, int end, int step) {
        TrieNode* p = this;
        for (int i=beg; i!=end && p!=NULL; i+=step) {
            int idx = s[i] - 'a';
            p = p->next[idx];
        }
        if (p) return &(p->weights);
        return NULL;
    }
};

class WordFilter {
    TrieNode prefixTrie;
    TrieNode suffixTrie;
public:
    WordFilter(vector<string> words) {
        for (int i=0; i<words.size(); ++i) {
            prefixTrie.insert(words[i].c_str(), 0, words[i].size(), 1, i);
            suffixTrie.insert(words[i].c_str(), words[i].size()-1, -1, -1, i);
        }  
    }
    
    int f(string prefix, string suffix) {
        if (prefix == "") {
            if (suffix == "") return 0;
            set<int>* s2 = suffixTrie.findWeight(suffix.c_str(), suffix.size()-1, -1, -1);
            if (s2) return *(s2->rbegin());
        }
        set<int>* s1 = prefixTrie.findWeight(prefix.c_str(), 0, prefix.size(), 1);
        if (s1) {
            if (suffix == "") return *(s1->rbegin());
            set<int>* s2 = suffixTrie.findWeight(suffix.c_str(), suffix.size()-1, -1, -1);
            if (s2) {
                for (set<int>::reverse_iterator it=s1->rbegin(); it!=s1->rend(); ++it) {
                    if (s2->find(*it) != s2->end()) return *it;
                }
            }
        }
        return -1;
    }
};

int main() {
    vector<string> words={"apple"};
    WordFilter wf(words);
    cout << wf.f("a", "e") << endl;
    cout << wf.f("b", "") << endl;
}
