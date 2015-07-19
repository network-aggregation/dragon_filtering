#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "list.h"

char Filters(unsigned short p, unsigned short q, unsigned short AS, char cons_type){
	int i,lim;
	unsigned short p_rt,q_rt,*vect,neigh;
  
	if( network[AS].FIB!=NULL ){
		p_rt = network[AS].FIB[p];
		q_rt = network[AS].FIB[q];
	}else{
		p_rt = AS==p ? 1 : 3;
		q_rt = AS==q ? 1 : 3;
	}
  
	if(AS!=p && p_rt==q_rt){
    
		if(cons_type && network[AS].FIB!=NULL && q_rt!=3){
			switch(q_rt){
				case 1:
					lim=network[AS].n_customers;
					vect=network[AS].customers;
					break;
				case 2:
					lim=network[AS].n_peers;
					vect=network[AS].peers;
					break;
			}
      
			for(i=0;i<lim;i++){
				neigh=vect[i];
				if( neigh==q || (network[neigh].FIB!=NULL && network[neigh].FIB[q]==1) ){
					if( !( neigh==p || (network[neigh].FIB!=NULL && network[neigh].FIB[p]==1) ) )
						return 0;
				}
			}
		}
    
		return 1;
    
	}else{
    	return 0;
	}
}

void PrintReport(char * fib_pref, unsigned char cons_type){
	int i,n_nodes=0,n_nonstubs=0;
	FILE * fib_fp;
	entry *top;
	unsigned int fib_entries, announced_by_i[MAXAS];
	/* Stats variables */
	double perc,max_fib_perc=0.0,min_fib_perc=1.0;
	distr *fib_perclist=NULL, *fib_perclistNOSTUB=NULL, *cursor;
	char fib_filename[MAXSTR];

	memset(fib_filename,'\0',MAXSTR);

	sprintf(fib_filename,"%s.txt",fib_pref);
	OPEN_FILE_W(fib_fp,fib_filename)
  
	for(i=0;i<MAXAS;i++){
		if( NODE_EXISTS(i) ){
			n_nodes++;
			if( network[i].n_customers!=0 )
				n_nonstubs++;
			fib_entries=0;
			announced_by_i[i]=0;
			top=PrefixTree;
			while(top!=NULL){
	
				if(top->AS==i){
					if(top->type!='N')
						announced_by_i[i]++;
				}else{
					if( top->less_specs==NULL || !(Filters(top->less_specs->AS,top->AS,(unsigned short)i,cons_type)) ){
						fib_entries++;
					}
				}

				CHOOSE_NEXT_PREFIX(top)
			}
      
			if( (n_nodes%LIMNODES)==0 )
				printf("Nodes checked: %d\n",n_nodes);
      
			perc=1.0-((double)fib_entries)/((double)(all_prefs-agg_prefs-announced_by_i[i]));
			if(perc > max_fib_perc)
				max_fib_perc=perc;
			if(perc < min_fib_perc)
				min_fib_perc=perc;
      
			fib_perclist=AddOrdered(perc,fib_perclist);
			if( network[i].n_customers!=0 ){
				fib_perclistNOSTUB=AddOrdered(perc,fib_perclistNOSTUB);
				fprintf(fib_fp,"AS%d: %u -> %u, %f\n",i,all_prefs-agg_prefs-announced_by_i[i],fib_entries,perc);
			}else{
				fprintf(fib_fp,"AS%d (STUB): %u -> %u, %f\n",i,all_prefs-agg_prefs-announced_by_i[i],fib_entries,perc);
			}
		}
	}
  
	fprintf(fib_fp,"Maximum reduction: %f\nMinimum reduction: %f\n", max_fib_perc, min_fib_perc);
  
	fprintf(fib_fp,"---Distribution of reduction rates (all ASs)---\n");
	for(cursor=fib_perclist;cursor!=NULL;cursor=cursor->next){
		fprintf(fib_fp,"%f\t%u\n",cursor->key,cursor->qty);
	}
  
	fprintf(fib_fp,"---Distribution of reduction rates (non-stubs)---\n");
	for(cursor=fib_perclistNOSTUB;cursor!=NULL;cursor=cursor->next){
		fprintf(fib_fp,"%f\t%u\n",cursor->key,cursor->qty);
	}
  
	DestroyOrdered(fib_perclist);
	DestroyOrdered(fib_perclistNOSTUB);
  
	fclose(fib_fp);
}
  
