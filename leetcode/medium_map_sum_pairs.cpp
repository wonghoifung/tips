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
    bool existed;
    int value;
    struct TrieNode* next[MAX_NODE_SZ];
public:
    TrieNode():existed(false),value(0) {
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
    void insert(const char* s, int value=0) {
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
        p->value = value;
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
    typedef void (*ValueCallback)(int value);
    void visitAllByPrefix(const char* prefix, ValueCallback cb) {
        TrieNode* p = this;
        while (p != NULL && *prefix != '\0') {
            p = p->next[*prefix - 'a'];
            prefix++;
        }
        if (p) {
            queue<TrieNode*> Q;
            Q.push(p);
            while (!Q.empty()) {
                TrieNode* node = Q.front();
                Q.pop();
                if (node->existed) cb(node->value);
                for (int i=0;i<MAX_NODE_SZ;++i) {
                    if (node->next[i]) {
                        Q.push(node->next[i]);
                    }
                }
            }
        }
    }
};

class MapSum {
    TrieNode trie;
    static int value_sum;
    static void value_callback(int value) {
        value_sum += value;
    }
public:
    MapSum() {
        
    }
    
    void insert(string key, int val) {
        trie.insert(key.c_str(), val);
    }

    int sum(string prefix) {
        value_sum = 0;
        trie.visitAllByPrefix(prefix.c_str(), value_callback);
        return value_sum;
    }
};
int MapSum::value_sum = 0;

int main() {
    MapSum ms;
    ms.insert("apple", 3);
    cout << ms.sum("ap") << endl;
    ms.insert("app", 2);
    cout << ms.sum("ap") << endl;
}
