/************************************************
 *                    FILTER                    *
 *                                              *
 *      Author:  Andre' Correia Magalhaes Sousa *
 *        Date:  16-July-2015                   *
 * Affiliation:  Instituto de Telecomunicacoes  *
 ************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "network.h"
#include "routing.h"
#include "prefixes.h"
#include "filter.h"

int main(int argc, char * argv[]){
  
	if( argc < 3 ){
		fprintf(stderr,"Usage: %s <Network File> <Prefix File>\n",argv[0]);
		exit(-1);
	}
  
	CreateNetwork(argv[1]);
	CreatePrefixTree(argv[2]);
	RouteDetectGlobal();
	PrintReport("filtering_efficiency",0);
	printf("Finished!\n");
	DestroyNetwork();
	DestroyPrefixTree();
  
	return 0;
}
