#ifndef LLIST_HEADER
#define LLIST_HEADER

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int i;
	int j;
	void* k;
} ELE;

struct lnode {
	struct lnode *prev, *next;
	ELE* element;	
};

typedef struct {
	struct lnode head;
	int num;
} LLIST;

typedef int(*list_find_p)(ELE* pELE, void* key);
typedef void(*list_travel_p)(ELE* pELE, void* key);

LLIST* lhandle;
list_travel_p travel_p;
list_find_p find_p;

LLIST* llist_create(void);

int llist_append(LLIST* handle, ELE* pnode);

int llist_preappend(LLIST* handle, ELE* pnode);

int llist_size(LLIST* handle);

void llist_destroy(LLIST* handle);

int llist_delete(LLIST* handle, list_find_p find, void* key);

void llist_travel(LLIST* handle, list_travel_p trav, void* arg);

#ifdef __cplusplus
}
#endif

#endif 
