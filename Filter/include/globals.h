#ifndef _GLOBALS_H_
#define _GLOBALS_H_

/******* All defines *******/
#define INITIALIZE_NODE(n) \
network[n].n_customers=0; \
network[n].customers=NULL; \
network[n].n_peers=0; \
network[n].peers=NULL; \
network[n].n_providers=0; \
network[n].providers=NULL; \
network[n].FIB=NULL

#define ALLOCATE_FOR(neigh,n_neigh) \
neigh = (unsigned short*) malloc( n_neigh*sizeof(unsigned short) ); \
if(neigh == NULL){ \
	fprintf(stderr,"Memory allocation error.\n"); \
	exit(-1); \
} \
network[AS].n_neigh=n_neigh; \
network[AS].neigh=neigh

#define CREATE_NEW_PREFIX \
all_prefs++; \
new = (entry*) malloc( sizeof(entry) ); \
if( new==NULL ){ \
	fprintf(stderr,"Memory allocation error.\n"); \
	exit(-1); \
} \
new->addr=IP; \
new->length=length; \
new->AS=AS; \
new->type=type; \
allocated=1

#define NODE_EXISTS(n) network[n].n_customers!=0 || \
network[n].n_peers!=0 || \
network[n].n_providers!=0

#define CHOOSE_NEXT_PREFIX(el) \
if(el->more_specs!=NULL){ \
	el=el->more_specs; \
}else if(el->next!=NULL){ \
	el=el->next; \
}else{ \
	while(el->less_specs!=NULL && el->less_specs->next==NULL) \
		el=el->less_specs; \
	el = el->less_specs==NULL ? NULL : el->less_specs->next; \
} \

#define OPEN_FILE_W(f,name) \
f = fopen(name,"w"); \
if( f==NULL ){ \
	fprintf(stderr,"Error opening output file.\n"); \
	exit(-1); \
}

#define MAXAS 65536
#define LIMPREF 20000
#define LIMNODES 5000
#define MAXSTR 1024

typedef struct _fwdneigh{
	unsigned short ID;
	struct _fwdneigh * next;
} fwdneigh;

typedef struct _entry{
	unsigned int addr;
	unsigned char length;
	unsigned short AS;
	char type;
	struct _entry * next;
	struct _entry * less_specs;
	struct _entry * more_specs;
} entry;

typedef struct _node{
	unsigned short n_customers;
	unsigned short * customers;
	unsigned short n_peers;
	unsigned short * peers;
	unsigned short n_providers;
	unsigned short * providers;
	unsigned char * FIB;
} node;

typedef struct _distr{
	double key;
	unsigned int qty;
	struct _distr * next;
} distr;

node network[MAXAS];
entry * PrefixTree;
unsigned int all_prefs;
unsigned int agg_prefs;

#endif
