#ifndef _LIST_H_
#define _LIST_H_

dfs * AddDFS(entry * new_el, unsigned int addr, unsigned char new_lev, dfs * list);
dfs * RemoveDFS(dfs * list);
neigh * AddList(unsigned short new_AS, neigh * list);
neigh * RemoveFirst(neigh * list);
neigh * RemoveElement(neigh * list, neigh * element);
void DestroyList(neigh * list);

#endif