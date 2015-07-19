#ifndef _LIST_H_
#define _LIST_H_

fwdneigh * AddList(unsigned short new_AS, fwdneigh * list);
fwdneigh * RemoveFirst(fwdneigh * list);
distr * AddOrdered(double key, distr * list);
void DestroyOrdered(distr * list);

#endif