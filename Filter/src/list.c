#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

fwdneigh * AddList(unsigned short new_AS, fwdneigh * list){
	fwdneigh * new;
  
	new = (fwdneigh*) malloc( sizeof(fwdneigh) );
	if( new==NULL ){
		fprintf(stderr,"Memory allocation error.\n");
		exit(-1);
	}
  
	new->ID=new_AS;
	new->next=list;
  
	return new;
}

fwdneigh * RemoveFirst(fwdneigh * list){
	fwdneigh * aux;
  
	if(list!=NULL){
		aux=list->next;
		free(list);
		return aux;
	}
  
	return NULL;
}

distr * AddOrdered(double key, distr * list){
	distr *cursor, *new;
	char found=0;
  
	if(list==NULL || list->key > key){
		new = (distr*) malloc( sizeof(distr) );
		if(new == NULL){
			fprintf(stderr,"Memory allocation error.\n");
			exit(-1);
        }
    
        new->key=key;
        if(list!=NULL)
            new->qty=1+(list->qty);
        else
            new->qty=1;
        new->next=list;
        return new;
	}
  
	(list->qty)++;
  
	if(list->key == key)
		return list;
  
	cursor=list;
	while(cursor->next!=NULL && found==0){
		if(cursor->next->key == key){
			(cursor->next->qty)++;
			return list;
		}
		if(cursor->next->key > key){
			found=1;
		}else{
			(cursor->next->qty)++;
			cursor=cursor->next;
		}
	}
  
	new = (distr*) malloc( sizeof(distr) );
	if(new==NULL){
		fprintf(stderr,"Memory allocation error.\n");
		exit(-1);
	}
	new->key=key;
	if(cursor->next!=NULL)
		new->qty=1+(cursor->next->qty);
	else
		new->qty=1;
	new->next=cursor->next;
	cursor->next=new;
  
	return list;
}

void DestroyOrdered(distr * list){
	distr * aux;
  
	while(list!=NULL){
		aux=list->next;
		free(list);
		list=aux;
	}
}
