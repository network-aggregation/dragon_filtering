#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

dfs * AddDFS(entry * new_el, unsigned int addr, unsigned char new_lev, dfs * list){
	dfs * new;
  
	new = (dfs*) malloc( sizeof(dfs) );
	if(new==NULL){
		fprintf(stderr,"Memory allocation error.\n");
		exit(-1);
	}
	new->element=new_el;
	new->addr=addr;
	new->level=new_lev;
	new->next=list;
  
	return new;
}

dfs * RemoveDFS(dfs * list){
	dfs * aux = NULL;
  
	if(list!=NULL){
		aux=list->next;
		free(list);
	}

	return aux;
}

neigh * AddList(unsigned short new_AS, neigh * list){
	neigh * new;
  
	new = (neigh*) malloc( sizeof(neigh) );
	if( new==NULL ){
		fprintf(stderr,"Memory allocation error.\n");
		exit(-1);
	}
  
	new->ID=new_AS;
	new->next=list;
	new->prev=NULL;
	if(list!=NULL)
		list->prev=new;
  
	return new;
}

neigh * RemoveFirst(neigh * list){
	neigh * aux = NULL;
  
	if(list!=NULL){
		aux=list->next;
		free(list);
		if(aux!=NULL)
			aux->prev=NULL;
	}
  
	return aux;
}

neigh * RemoveElement(neigh * list, neigh * element){
  
	if(element!=NULL){
		if(list==element)
			return RemoveFirst(list);
    
		if(element->next!=NULL)
			element->next->prev=element->prev;
    
		element->prev->next=element->next;
    
		free(element);
    
		return list;
	}
  
	return NULL;

}

void DestroyList(neigh * list){
	while(list!=NULL)
		list=RemoveFirst(list);
}
