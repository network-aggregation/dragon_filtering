#include <stdio.h>
#include <stdlib.h>
#include "globals.h"

void CreateNetwork(char * filename){
	int i,AS,new_AS,n_customers,n_peers,n_providers;
	unsigned short *customers, *peers, *providers;
	FILE * fp;
  
	fp = fopen(filename,"r");
	if(fp==NULL){
		fprintf(stderr,"Error opening network file.\n");
		exit(-1);
	}
  
	for(i=0;i<MAXAS;i++){
		INITIALIZE_NODE(i);
	}
  
	while( fscanf(fp,"%d %d %d %d",&AS,&n_customers,&n_peers,&n_providers)!=EOF ){
    
		if(n_customers!=0){
			ALLOCATE_FOR(customers,n_customers);
		}
    
		if(n_peers!=0){
			ALLOCATE_FOR(peers,n_peers);
		}
    
		if(n_providers!=0){
			ALLOCATE_FOR(providers,n_providers);
		}
    
		if( n_customers!=0 || n_peers!=0 ){
			network[AS].FIB = (unsigned char*) malloc( MAXAS*sizeof(unsigned char) );
			if( network[AS].FIB==NULL ){
				fprintf(stderr,"Memory allocation error.\n");
				exit(-1);
			}
      
			for(i=0;i<MAXAS;i++)
				network[AS].FIB[i]=3;
      
			network[AS].FIB[AS]=1;
		}
    
		for(i=0;i<n_customers;i++){
			fscanf(fp,"%d",&new_AS);
			network[AS].customers[i]=(unsigned short)new_AS;
		}
    
		for(i=0;i<n_peers;i++){
			fscanf(fp,"%d",&new_AS);
			network[AS].peers[i]=(unsigned short)new_AS;
		}
    
		for(i=0;i<n_providers;i++){
			fscanf(fp,"%d",&new_AS);
			network[AS].providers[i]=(unsigned short)new_AS;
		}
	}
  
	fclose(fp);
}

void DestroyNetwork(){
	int i;
  
	for(i=0;i<MAXAS;i++){
		if( network[i].n_customers!=0 )
			free(network[i].customers);
    
		if( network[i].n_peers!=0 )
			free(network[i].peers);
    
		if( network[i].n_providers!=0 )
			free(network[i].providers);
    
		if( network[i].n_customers!=0 || network[i].n_peers!=0 )
			free(network[i].FIB);
	}
}
