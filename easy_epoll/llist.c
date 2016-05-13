#include <stdlib.h>
#include <string.h>
#include "llist.h"

LLIST* llist_create(void)
{
	LLIST* handle = NULL;
	
	handle = (LLIST*)malloc(sizeof(LLIST));
	if(handle == NULL)
	{
		return NULL;
	}
	
	handle->head.prev = &handle->head;
	handle->head.next = &handle->head;
	handle->num = 0;

	return handle;
}

int llist_append(LLIST* handle, ELE* pELE)
{
	struct lnode* newnode = NULL;
	
	newnode = (struct lnode*)malloc(sizeof(struct lnode));
	if(newnode == NULL)
	{
		return -1;
	}
	newnode->element = (ELE*)malloc(sizeof(ELE));
	if(newnode->element == NULL)
	{
		free(newnode);
		return -1;
	}
	newnode->prev = handle->head.prev;
	newnode->next = &handle->head;
	handle->head.prev->next = newnode;
	handle->head.prev = newnode;
    
	memcpy(newnode->element, pELE, sizeof(ELE));
	handle->num += 1;

	return 0;	
}

int llist_preappend(LLIST* handle, ELE* pELE)
{
	struct lnode* newnode = NULL;
	
	newnode = (struct lnode*)malloc(sizeof(struct lnode));
	if(newnode == NULL)
	{
		return -1;
	}
	newnode->element = (ELE*)malloc(sizeof(ELE));
	if(newnode->element == NULL)
	{
		free(newnode);
		return -1;
	}

	newnode->prev = &handle->head;
	newnode->next = handle->head.next;
	handle->head.next->prev = newnode;
	handle->head.next = newnode;

	memcpy(newnode->element, pELE, sizeof(ELE));
	handle->num += 1;

	return 0;
}

int llist_size(LLIST* handle)
{
	return handle->num;
}

void llist_destroy(LLIST* handle)
{
	struct lnode *tmp = NULL, *next = NULL;
	
	for(tmp = handle->head.next; tmp != &handle->head; tmp = next)
	{
		next = tmp->next;
		free(tmp->element);
		free(tmp);
	}
	free(handle);
}

int llist_delete(LLIST* handle, list_find_p find, void* key)
{
	struct lnode* tmp = NULL, *next = NULL;
	int count = 0;

	for(tmp = handle->head.next; tmp != &handle->head; tmp = next)
	{
		next = tmp->next;
		if(find(tmp->element, key) == 1)
		{
			tmp->prev->next = tmp->next;
			tmp->next->prev = tmp->prev;
			free(tmp->element);
			free(tmp);
			count += 1;
            handle->num -= 1;

			break; 
		}
	}

	return count;
}

int  llist_delete_quick(LLIST* handle,struct lnode* pNode)
{
	if(handle == NULL || pNode == NULL)
		return 0;
	if(pNode->next  == NULL || pNode->prev == NULL)
		return 0;
	pNode->prev->next = pNode->next;
	pNode->next->prev = pNode->prev;
	free(pNode->element);
	free(pNode);

	return 1;
}

void llist_travel(LLIST* handle, list_travel_p trav, void* arg)
{
	struct lnode* tmp = NULL;
	struct lnode* tmp2= NULL;
	for(tmp = handle->head.next; tmp != &handle->head;) 
	{
		tmp2 = tmp->next;
		trav(tmp->element, arg);
		tmp = tmp2;
	}
}
