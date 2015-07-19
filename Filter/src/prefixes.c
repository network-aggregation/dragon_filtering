#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

char isCompatible(unsigned int prefix1, unsigned char length1, unsigned int prefix2, unsigned char length2){
	unsigned char length, shift;
	unsigned int first_bits1, first_bits2;
  
	length = length1 < length2 ? length1 : length2;
	shift=32-length;
  
	first_bits1 = prefix1 >> shift;
	first_bits2 = prefix2 >> shift;
  
	if(first_bits1 == first_bits2)
		return 1;
	else
		return 0;
}

void InsertPrefix(unsigned int IP, unsigned char length, unsigned short AS, char type){
	entry *cursor, *cursor_prev=NULL, *new, *base, *aux, *list_specs=NULL;
	unsigned char curs_len, in_specs=0, allocated=0;
  
	cursor=PrefixTree;
	base=PrefixTree;
	while(cursor!=NULL){
		curs_len = cursor->length;
		if( isCompatible(cursor->addr,cursor->length,IP,length) ){
			if( curs_len < length ){
				cursor_prev=NULL;
				base=cursor;
				cursor=cursor->more_specs;
				in_specs=1;
			}
			if( curs_len == length )
				return;
			if( curs_len > length ){
				if(!allocated){
					CREATE_NEW_PREFIX;
				}
	
				cursor->less_specs=new;
				if(cursor_prev!=NULL)
					cursor_prev->next=cursor->next;
				if(cursor==PrefixTree)
					PrefixTree=PrefixTree->next;
				if(cursor==base->more_specs)
					base->more_specs=base->more_specs->next;
				aux=cursor->next;
				cursor->next=list_specs;
				list_specs=cursor;
				cursor=aux;
			}
		}else{
			cursor_prev=cursor;
			cursor=cursor->next;
		}
	}
  
	if(!allocated){
		CREATE_NEW_PREFIX;
	}
  
	new->more_specs=list_specs;
	if(!in_specs){
		new->less_specs=NULL;
		new->next=PrefixTree;
		PrefixTree=new;
	}else{
		new->less_specs=base;
		new->next=base->more_specs;
		base->more_specs=new;
	}
  
}

void CreatePrefixTree(char * filename){
	int IP1, IP2, IP3, IP4, len, ASn, k=0;
	FILE * fp;
	unsigned int IP;
	unsigned char length;
	unsigned short AS;
	char type;
  
	fp = fopen(filename,"r");
	if(fp==NULL){
		fprintf(stderr,"Error opening prefix file.\n");
		exit(-1);
	}
  
	all_prefs=0;
	PrefixTree=NULL;
  
	fscanf(fp,"%d",&agg_prefs);
	while( fscanf(fp,"%d.%d.%d.%d/%d %d %c",&IP1,&IP2,&IP3,&IP4,&len,&ASn,&type)!=EOF ){
		AS = (unsigned short)ASn;
		length = (unsigned char)len;
		IP = (IP1 << 24) | (IP2 << 16) | (IP3 << 8) | IP4;
		InsertPrefix(IP,length,AS,type);
		k++;
		if( (k%LIMPREF)==0 ){
			printf("%d prefixes read so far...\n",k);
		}
	}
	printf("All prefixes have been read!\n");
  
	fclose(fp);
}

void DestroyPrefixTree(){
	entry *cursor, *aux;
  
	while(PrefixTree!=NULL){
		if(PrefixTree->more_specs!=NULL){
			for(cursor=PrefixTree->more_specs;cursor->next!=NULL;cursor=cursor->next);
			cursor->next=PrefixTree->next;
			aux=PrefixTree->more_specs;
		}else{
			aux=PrefixTree->next;
		}
    
		free(PrefixTree);
		PrefixTree=aux;
	}
}
