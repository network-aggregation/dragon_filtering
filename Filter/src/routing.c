#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "list.h"

void RouteDetectSrc(unsigned short src){
	int i;
	unsigned short curr,cursor_ID;
	fwdneigh * queue=NULL;
  
	for(i=0;i<network[src].n_customers;i++){
		curr=network[src].customers[i];
		network[src].FIB[curr]=1;
		queue=AddList(curr,queue);
    
	}
  
	while(queue!=NULL){
		curr=queue->ID;
		queue=RemoveFirst(queue);
    
		for(i=0;i<network[curr].n_customers;i++){
			cursor_ID = network[curr].customers[i];
			if( network[src].FIB[cursor_ID] > 1 ){
				network[src].FIB[cursor_ID]=1;
				queue=AddList(cursor_ID,queue);
	
			}
		}
	}
  
	for(i=0;i<network[src].n_peers;i++){
		curr=network[src].peers[i];
		if( network[src].FIB[curr] > 2 ){
			network[src].FIB[curr]=2;
			queue=AddList(curr,queue);
		}
	}
  
	while(queue!=NULL){
		curr=queue->ID;
		queue=RemoveFirst(queue);
    
		for(i=0;i<network[curr].n_customers;i++){
			cursor_ID = network[curr].customers[i];
			if( network[src].FIB[cursor_ID] > 2 ){
				network[src].FIB[cursor_ID]=2;
				queue=AddList(cursor_ID,queue);
			}
		}
	}
}

void RouteDetectGlobal(){
	int i;
  
	for(i=0;i<MAXAS;i++){
		if( network[i].n_customers!=0 || network[i].n_peers!=0 ){
			RouteDetectSrc((unsigned short)i);
		}
	}
}
