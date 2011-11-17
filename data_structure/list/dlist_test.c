#include <stdio.h>
#include "dlist.h"



int main()
{
  DLIST d1, *dp, *dpt;
  unsigned int x = 30, y= 40, z = 50, w = 60;

  dlist_init(&d1, &x);

  dlist_add_tail(&d1, &y);
  dlist_add_tail(&d1, &z);
  dlist_add_tail(&d1, &w);

  dp = &d1;
  dlist_del_tail(&d1);
  LIST_FOR_EACH(dpt, dp) {
   printf("%d\n", *(unsigned int *)dpt->data);
   }

    return 0;
}
