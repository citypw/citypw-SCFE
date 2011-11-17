/*
 * Doubly circular linked list implementation! It's simple but works.
 *
 * Copyright(C) 2011 HackerFellowship (HFG)
 * Monkey-coders: Shawn the R0ck, <citypw@gmail.com>
 *                [... List your name here ...]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DLIST_H
#define _DLIST_H

#include <stdlib.h>
#include "../../include/debug_msg.h"

/**
 * LIST_FOR_EACH - iterate over a list
 * @start: the start position
 * @end: list head
 * This implementation is copy from GNU/Linux kernel's src: list.h
 */
#define LIST_FOR_EACH(start, end) \
  for(start = end->next; start != end; start = start->next)

typedef unsigned short int u16;

typedef struct dlist{
  struct dlist *next, *prev;
  void *data;
}DLIST;

static unsigned int list_num = 0;

/**
 * dlist_init - initialization of the linked list
 * @dlist: list head
 * @data: specific data
 */
int dlist_init(DLIST *dlist, void *data)
{
  dlist->next = dlist;
  dlist->prev = dlist;
  dlist->data = data;

  list_num++;
  return 0;
}

int __list_add(DLIST *prev, DLIST *next, void *data)
{
  DLIST *dnew = (DLIST*)malloc(sizeof(DLIST));

  if(dnew == NULL){
    DEBUG_MSG("dlist malloc() is failed!\n");
    return -1;
  }

  next->prev = dnew;
  dnew->next = next;
  dnew->prev = prev;
  prev->next = dnew;

  dnew->data = data;

  return 0;
}

/**
 * dlist_add - add a new entry
 * @head: list head
 * @data: data
 *
 * On success, return zero. Otherwise, return -1
 */
inline int dlist_add(DLIST *head, void *data)
{
  return __list_add(head, head->next, data);
}

inline int dlist_add_tail(DLIST *head, void *data)
{
  return __list_add(head->prev, head, data);
}

void __dlist_del(DLIST *head, unsigned short int flag)
{
  DLIST *dpt = NULL;

  if(head->next == head){
    DEBUG_MSG("there's only list head exist! It's no needs to delete!\n");
    exit(1);
  }

  switch(flag){
  case 0:
    dpt = head->next;
  head->next = head->next->next;
  head->next->prev = head;
  free(dpt);
  break;
  case 1:
    dpt = head->prev;
    head->prev->prev->next = head;
    head->prev = head->prev->prev;
    free(dpt);
    break;
  default:
    DEBUG_MSG("WTH, you need add your own implementation here!\n");
    break;
  }

}
void inline dlist_del(DLIST *head)
{
  __dlist_del(head, 0);
}

inline void dlist_del_tail(DLIST *head)
{
  __dlist_del(head, 1);
}

#endif
