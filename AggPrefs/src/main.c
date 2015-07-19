/************************************************
 *                   AGGPREFS                   *
 *                                              *
 *      Author:  Andre' Correia Magalhaes Sousa *
 *        Date:  16-July-2015                   *
 * Affiliation:  Instituto de Telecomunicacoes  *
 ************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "network.h"
#include "routing.h"
#include "prefixes.h"

int main(int argc, char * argv[]){
  
    if(argc < 3){
        fprintf(stderr,"Usage: %s <Network File> <Prefix File>\n",argv[0]);
        exit(-1);
    }
  
    srand(time(NULL));
  
    CreateNetwork(argv[1]);
    ReadPrefixFile(argv[2]);
    RouteDetectGlobal();
    AnalyzePartitions();
    TestAllAgg();
    PrintPrefixes("prefixes_with_agg.txt");
    DestroyNetwork();
  
    return 0;
}
