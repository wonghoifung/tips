#include <stdlib.h>
#include <stdio.h>
#include "bsdqueue.h"

struct list_item_name(qq) 
{
  int id;
  const char* name;
  list_next_ptr(qq);
};

list_def(qq)

int main()
{
  list_new(qq, hp);

  list_item_new(qq, ip)
  ip->id = 1;
  ip->name = "wong";

  //list_item_init(qq, ip)
  list_lock(hp);

  list_unlock(hp);

  printf("size:%u\n",list_size(hp));

  list_push(hp, ip);
  
  printf("size:%u\n",list_size(hp));

  list_item_new(qq, ip2)
  ip2->id = 2;
  ip2->name = "hoi";
  list_unshift(hp, ip2);
  
  printf("size:%u\n",list_size(hp));

  list_item_new(qq, ip3)
  ip3->id = 3;
  ip3->name = "fung";
  list_insert(hp, ip2, ip3);
  
  printf("size:%u\n",list_size(hp));

  //list_item_ptr(qq) ip4;
  //list_shift(hp, ip4);
  //list_pop(hp, ip4);
  //printf("%d %s\n",ip4->id,ip4->name);
  //printf("size:%u\n",list_size(hp));

  //list_item_ptr(qq) ip4 = list_first(hp);
  list_item_ptr(qq) ip4 = list_last(hp);
  printf("%d %s\n",ip4->id,ip4->name);
  printf("size:%u\n",list_size(hp));

  list_item_ptr(qq) ip5 = NULL;
  list_foreach(hp, ip5)  
  {
    printf("===> %d %s\n",ip5->id,ip5->name);
  }

  list_item_ptr(qq) ip6;
  list_addr(hp,0,ip6); 
  if (ip6==NULL) printf("---> none\n");
  else printf("---> %d %s\n",ip6->id,ip6->name);
  list_addr(hp,1,ip6); 
  if (ip6==NULL) printf("---> none\n");
  else printf("---> %d %s\n",ip6->id,ip6->name);
  list_addr(hp,2,ip6); 
  if (ip6==NULL) printf("---> none\n");
  else printf("---> %d %s\n",ip6->id,ip6->name);
  list_addr(hp,3,ip6); 
  if (ip6==NULL) printf("---> none\n");
  else printf("---> %d %s\n",ip6->id,ip6->name);

  list_new(qq, hp2);
  list_item_new(qq, ip20);
  ip20->id=20; ip20->name="huang";
  list_item_new(qq, ip21);
  ip21->id=21; ip21->name="hai";
  list_item_new(qq, ip22);
  ip22->id=22; ip22->name="feng";
  list_push(hp2,ip20);
  list_push(hp2,ip21);
  list_push(hp2,ip22);

  list_cat(hp,hp2);
  list_delete_head(hp);
  list_foreach(hp, ip5)  
  {
    printf(">>>>> %d %s\n",ip5->id,ip5->name);
  }

  list_clear(hp);
  list_destroy(hp);
  list_destroy(hp2);
}

