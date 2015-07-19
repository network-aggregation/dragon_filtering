#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "list.h"
#include "utils.h"

unsigned int BitOne(unsigned char position){
	switch(position){
		case 1: return 0x80000000;
		case 2: return 0x40000000;
		case 3: return 0x20000000;
		case 4: return 0x10000000;
		case 5: return 0x08000000;
		case 6: return 0x04000000;
		case 7: return 0x02000000;
		case 8: return 0x01000000;
		case 9: return 0x00800000;
		case 10: return 0x00400000;
		case 11: return 0x00200000;
		case 12: return 0x00100000;
		case 13: return 0x00080000;
		case 14: return 0x00040000;
		case 15: return 0x00020000;
		case 16: return 0x00010000;
		case 17: return 0x00008000;
		case 18: return 0x00004000;
		case 19: return 0x00002000;
		case 20: return 0x00001000;
		case 21: return 0x00000800;
		case 22: return 0x00000400;
		case 23: return 0x00000200;
		case 24: return 0x00000100;
		case 25: return 0x00000080;
		case 26: return 0x00000040;
		case 27: return 0x00000020;
		case 28: return 0x00000010;
		case 29: return 0x00000008;
		case 30: return 0x00000004;
		case 31: return 0x00000002;
		case 32: return 0x00000001;
		default: return 0;
	}
}

char isCompatible(unsigned int pref1, unsigned char len1, unsigned int pref2, unsigned char len2){
	unsigned char len, shift;
	unsigned int first_bits1, first_bits2;
  
	len = len1 < len2 ? len1 : len2;
	shift = 32-len;
  
	first_bits1 = pref1 >> shift;
	first_bits2 = pref2 >> shift;
  
	if(first_bits1 == first_bits2)
		return 1;
	else
		return 0;
}

void InsertPrefix(unsigned int addr, unsigned char length, unsigned short AS, char type){
	entry *cursor, *parent=NULL, *new;
	unsigned int curr_pref,firstbit;
	unsigned char curr_len,child;
  
	curr_pref = (addr & BitOne(1));
	curr_len=1;
	firstbit = curr_pref >> 31;
  
	cursor=PrefixTree[firstbit];
	while(curr_len != length){
		if(cursor==NULL){
			CREATE_NEW_NODE(0);
		}
		
		parent=cursor;
		
		if( isCompatible(addr,length,curr_pref,curr_len+1) ){
			/* Enter child ZERO */
			child=0;
			cursor=cursor->zero;
		}else{
			/* Enter child ONE */
			child=1;
			curr_pref=curr_pref | BitOne(curr_len+1);
			cursor=cursor->one;
		}
		
		curr_len++;
	}
  
	if(cursor==NULL){
		CREATE_NEW_NODE(AS);
	}
  
	if(type=='N'){
		ANNOUNCE(cursor);
		cursor->ASs=AddList(AS,NULL);
		cursor->AS=0;
	}else{
		cursor->AS=AS;
		cursor->ASs=NULL;
	}
}

void RemovePrefix(entry * pref){
  
	if(pref->parent!=NULL){
		if(pref->parent->zero==pref)
			pref->parent->zero=NULL;
		else
			pref->parent->one=NULL;
	}else{
		if(PrefixTree[0]==pref)
			PrefixTree[0]=NULL;
		else
			PrefixTree[1]=NULL;
	}
  
	DestroyList(pref->ASs);
	free(pref);
}

void ReadPrefixFile(char * filename){
	int IP1,IP2,IP3,IP4,len;
	unsigned int IP;
	unsigned short AS;
	char type;
	FILE * fp;
  
	fp = fopen(filename,"r");
	if(fp==NULL){
		fprintf(stderr,"Error opening prefix file.\n");
		exit(-1);
	}
  
	PrefixTree[0]=NULL;
	PrefixTree[1]=NULL;
  
	fscanf(fp,"%d",&all_prefs);
	while( fscanf(fp,"%d.%d.%d.%d/%d\t%hu %c",&IP1,&IP2,&IP3,&IP4,&len,&AS,&type)!=EOF ){
		IP = (IP1 << 24) | (IP2 << 16) | (IP3 << 8) | IP4;
		InsertPrefix(IP,(unsigned char)len,AS,type);
	}
	printf("Done reading all prefixes!\n");
  
	fclose(fp);
}

void AnalyzePartitions(){
	int i;
	dfs *stack,*tmp_store;  
	unsigned char go;
  
	for(i=0;i<2;i++){
		if(PrefixTree[i]!=NULL){
			stack=AddDFS(PrefixTree[i],0,0,NULL);
			while(stack!=NULL){
				go=0;
				tmp_store=stack;
				if(tmp_store->element->zero!=NULL && NOT_EXPLORED(tmp_store->element->zero)){
					stack=AddDFS(tmp_store->element->zero,0,0,stack);
					go=1;
				}
				if(tmp_store->element->one!=NULL && NOT_EXPLORED(tmp_store->element->one)){
					stack=AddDFS(tmp_store->element->one,0,0,stack);
					go=1;
				}
				if(!go){
					if(stack->element->AS!=0 || stack->element->ASs!=NULL){
						PARTITIONED(stack->element);
					}else if(stack->element->zero!=NULL && stack->element->one!=NULL){
						if( IS_NOT_PARTITIONED(stack->element->zero) || IS_NOT_PARTITIONED(stack->element->one) ){
							NOT_PARTITIONED(stack->element);
						}else{
							PARTITIONED(stack->element);
						}
					}else{
						NOT_PARTITIONED(stack->element);
					}
					stack=RemoveDFS(stack);
				}
			}
		}
	}
}

neigh * RemoveProviders(neigh * ASs, unsigned short cust){
	neigh *cursor,*aux;
  
	while(ASs!=NULL && network[ASs->ID].FIB!=NULL && network[ASs->ID].FIB[cust]==CUSTOMER){
		aux=ASs->next;
		free(ASs);
		ASs=aux;
	}
  
	if(ASs==NULL)
		return NULL;
  
	cursor=ASs;
	while(cursor->next!=NULL){
		if(network[cursor->next->ID].FIB!=NULL && network[cursor->next->ID].FIB[cust]==CUSTOMER){
			aux=cursor->next;
			cursor->next=aux->next;
			free(aux);
		}else{
			cursor=cursor->next;
		}
	}
  
	return ASs;
}

char HasCustomer(neigh * ASs, unsigned short prov){
	neigh * cursor;
  
	if(network[prov].FIB==NULL)
		return 0;
  
	for(cursor=ASs;cursor!=NULL;cursor=cursor->next){
		if(network[prov].FIB[cursor->ID]==CUSTOMER)
			return 1;
	}
  
	return 0;
}

char isValid(unsigned short AS, entry * start_node){
	dfs *stack;
	entry *child0, *child1;
  
	stack=AddDFS(start_node,0,0,NULL);
	while(stack!=NULL){
		if(stack->element->AS!=0 && (stack->element->AS==AS || network[AS].FIB==NULL || network[AS].FIB[stack->element->AS]!=CUSTOMER) ){
			while(stack!=NULL)
				stack=RemoveDFS(stack);
			return 0;
		}
    
		child0=stack->element->zero;
		child1=stack->element->one;
    
		if(stack->element->AS==0){
			stack=RemoveDFS(stack);
      
			if(child0!=NULL)
				stack=AddDFS(child0,0,0,stack);
      
			if(child1!=NULL)
				stack=AddDFS(child1,0,0,stack);
		}else{
			stack=RemoveDFS(stack);
		}
	}
  
	return 1;
}

char isCeiling(unsigned short AS, entry * start_node){
	while(start_node!=NULL){
		if( start_node->AS!=0 ){
			if( network[start_node->AS].FIB!=NULL && start_node->AS!=AS && network[start_node->AS].FIB[AS]==CUSTOMER )
				return 1;
			else
				return 0;
		}else if( start_node->ASs!=NULL ){
			if( network[start_node->ASs->ID].FIB!=NULL && start_node->ASs->ID!=AS && network[start_node->ASs->ID].FIB[AS]==CUSTOMER )
				return 1;
			else
				return 0;
		}
		start_node = start_node->parent;
	}

	return 1;
}

void TestAllAgg(){
	int i,j,amount,choice;
	neigh * bestASs, * cursor, * aux;
	dfs *stack=NULL;
	entry *child0, *child1;
  
	for(i=0;i<2;i++){
		if( PrefixTree[i]!=NULL ){
			stack=AddDFS(PrefixTree[i],0,0,NULL);
			while(stack!=NULL){
				if( IS_PARTITIONED(stack->element) && stack->element->AS==0 && stack->element->ASs==NULL ){
					bestASs=NULL;
					for(j=0;j<MAXAS;j++){
						if( NODE_EXISTS(j) && !HasCustomer(bestASs,j) && isValid(j,stack->element) && isCeiling(j,stack->element->parent) ){
							bestASs=RemoveProviders(bestASs,j);
							bestASs=AddList(j,bestASs);
						}
					}
					if(bestASs!=NULL){
						amount=0;
						for(cursor=bestASs;cursor!=NULL;cursor=cursor->next)
							amount++;

						choice = uniform_distribution(1,amount);

						amount=1;
						for(cursor=bestASs; amount!=choice; cursor=cursor->next)
							amount++;

						while(cursor->next!=NULL){
							aux = cursor->next->next;
							free(cursor->next);
							cursor->next = aux;
						}

						while(bestASs!=cursor){
							aux = bestASs->next;
							free(bestASs);
							bestASs = aux;
						}	

						stack->element->ASs=bestASs;
						ANNOUNCE(stack->element);
						added_prefs++;
						if( (added_prefs%LIMPREFS)==0 )
							printf("%u aggregation prefixes added so far.....\n",added_prefs);
					}
				}

				child0=stack->element->zero;
				child1=stack->element->one;

				stack=RemoveDFS(stack);

				if(child0!=NULL)
					stack=AddDFS(child0,0,0,stack);

				if(child1!=NULL)
					stack=AddDFS(child1,0,0,stack);
			}
		}
	}
}

void PrintPrefixes(char * filename){
	FILE * fp;
	unsigned int IP1,IP2,IP3,IP4,pref;
	int length,i;
	dfs *stack=NULL,*tmp_store;
	neigh * cursor;
	unsigned char go;
	int amount,choice;
  
	fp = fopen(filename,"w");
	if( fp==NULL ){
		fprintf(stderr,"Error opening output file.\n");
		exit(-1);
	}
  
	fprintf(fp,"%u\n",all_prefs+added_prefs);
  
	for(i=0;i<2;i++){
		if( PrefixTree[i]!=NULL ){
			pref = i << 31;
			stack=AddDFS(PrefixTree[i],pref,1,stack);
		}
	}
  
	while(stack!=NULL){
    
		go=0;
    
		if( !( IS_CLEAN(stack->element) ) ){
			if( IS_ANNOUNCED(stack->element) ){
				IP1 = ((stack->addr) & 0xFF000000) >> 24;
				IP2 = ((stack->addr) & 0x00FF0000) >> 16;
				IP3 = ((stack->addr) & 0x0000FF00) >> 8;
				IP4 = ((stack->addr) & 0x000000FF);
				length = (int)(stack->level);
	
				if( IS_ORIG_ANN(stack->element) ){
					fprintf(fp,"%d.%d.%d.%d/%d\t%hu O\n",IP1,IP2,IP3,IP4,length,stack->element->AS);
				}else{
	  
					amount=0;
					for(cursor=stack->element->ASs;cursor!=NULL;cursor=cursor->next)
						amount++;
	  
					choice=uniform_distribution(1,amount);
	  
					amount=0;
					for(cursor=stack->element->ASs;cursor!=NULL && amount!=choice;cursor=cursor->next){
						amount++;
						if(amount==choice)
							fprintf(fp,"%d.%d.%d.%d/%d\t%hu N\n",IP1,IP2,IP3,IP4,length,cursor->ID);
					}
				}
			}
      
			CLEAN(stack->element);
		}
    
		if(stack->element->zero==NULL && stack->element->one==NULL){
			RemovePrefix(stack->element);
		}else{
			go=1;
			tmp_store=stack;

			if(tmp_store->element->zero!=NULL)
				stack=AddDFS(tmp_store->element->zero,tmp_store->addr,(tmp_store->level)+1,stack);
      
			if(tmp_store->element->one!=NULL){
				pref = (tmp_store->addr) | BitOne((tmp_store->level)+1);
				stack=AddDFS(tmp_store->element->one,pref,(tmp_store->level)+1,stack);
			}
		}
    
		if(!go)
			stack=RemoveDFS(stack);
	}
  
	fclose(fp);
}
