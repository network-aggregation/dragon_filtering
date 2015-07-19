#include <stdio.h>
#include <stdlib.h>

int uniform_distribution(int rangeLow, int rangeHigh){
	int range,limit,myRand,copies;
  
	range = rangeHigh - rangeLow + 1;
	copies=RAND_MAX/range;
  
	limit=range*copies;    
	myRand=-1;
	while( myRand<0 || myRand>=limit){
		myRand=rand();  
	}
  
	return myRand/copies+rangeLow;
}
