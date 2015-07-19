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

#define NODE_EXISTS(n) (network[n].n_providers!=0 || \
network[n].n_customers!=0 || \
network[n].n_peers!=0)

#define CREATE_NEW_NODE(asn) \
new = (entry*) malloc( sizeof(entry) ); \
if(new==NULL){ \
	fprintf(stderr,"Memory allocation error.\n"); \
	exit(-1); \
} \
new->AS=asn; \
new->ASs=NULL; \
new->ctrl=0; \
new->parent=parent; \
new->zero=NULL; \
new->one=NULL; \
if( parent==NULL ){ \
	PrefixTree[firstbit]=new; \
}else{ \
	if(child==0) \
		parent->zero=new; \
	else \
		parent->one=new; \
} \
cursor=new

#define TEST_CEILING_OR_VALID \
all_ceiling=1; \
all_valid=1; \
for(cursor=stack->element->ASs;cursor!=NULL && (all_ceiling || all_valid);cursor=cursor->next){ \
	if( src==cursor->ID || (network[src].FIB!=NULL && network[src].FIB[cursor->ID]==1) ){ \
		all_ceiling=0; \
	}else{ \
		if( network[cursor->ID].FIB==NULL || network[cursor->ID].FIB[src]!=1 ){ \
			all_ceiling=0; \
			all_valid=0; \
		}else{ \
			all_valid=0; \
		} \
	} \
}

#define REMOVE_PROVIDERS \
if( network[cursor->ID].FIB[src]==1 ){ \
	if(cursor->next!=NULL){ \
		cursor=cursor->next; \
		stack->element->ASs=RemoveElement(stack->element->ASs,cursor->prev); \
	}else{ \
		stack->element->ASs=RemoveElement(stack->element->ASs,cursor); \
		cursor=NULL; \
	} \
}else{ \
	cursor=cursor->next; \
}

#define IS_ANNOUNCED(el) el->AS!=0 || el->ASs!=NULL
#define IS_ORIG_ANN(el) el->AS!=0
#define IS_NEW_ANN(el) el->ASs!=NULL
#define IS_ORIG_NOT_CEILING(el) (el->AS!=0 && (el->ctrl & 0x18)!=0x18)
#define UNANNOUNCED_VALID(el) (el->AS==0 && el->ASs==NULL && (el->ctrl & 0x18)==0x08)
#define NEW_W_VALID_CHILDREN(el) (el->ASs!=NULL && (el->zero->ctrl & 0x18)==0x08 && (el->one->ctrl & 0x18)==0x08)

#define PARTITIONED(el) el->ctrl=2
#define NOT_PARTITIONED(el) el->ctrl=1
#define IS_NOT_PARTITIONED(el) (el->ctrl & 0x03)==1
#define IS_PARTITIONED(el) (el->ctrl & 0x03)==2
#define NOT_EXPLORED(el) el->ctrl==0
#define VALID(el) el->ctrl |= 0x08; el->ctrl &=0xEF
#define CEILING(el) el->ctrl |= 0x10; el->ctrl &= 0xF7
#define INVALID(el) el->ctrl |= 0x18
#define ANNOUNCE(el) el->ctrl |= 0x04
#define IS_NOT_EXPLORED(el) el->ctrl < 8
#define IS_CEILING(el) (el->ctrl & 0x18)==0x10
#define IS_INVALID(el) (el->ctrl & 0x18)==0x18
#define IS_VALID(el) (el->ctrl & 0x18)==0x08
#define RESET(el) el->ctrl &= 0x07
#define CLEAN(el) el->ctrl = 0xFF
#define IS_CLEAN(el) el->ctrl==0xFF
#define NEW_CEILING_W_VALID_CHILDREN(el) ((el->ctrl & 0x18)==0x10 && (el->ctrl & 0x04)==0x04 && \
(el->zero->ctrl & 0x18)==0x08 && (el->one->ctrl & 0x18)==0x08)

#define MAXAS 65536
#define LIMNODES 100
#define MAXTIER 10
#define LIMPREFS 100

#define PROVIDER 3
#define PEER 2
#define CUSTOMER 1

typedef struct _neigh{
	unsigned short ID;
	struct _neigh * next;
	struct _neigh * prev;
} neigh;

typedef struct _node{
	unsigned short n_customers;
	unsigned short * customers;
	unsigned short n_peers;
	unsigned short * peers;
	unsigned short n_providers;
	unsigned short * providers;
	unsigned char * FIB;
} node;

/* CTRL BITS
[1:0] - Partitioned
00 (0) - Not tested
01 (1) - Not partitioned
10 (2) - Partitioned
[2] - Aggregation prefix
0 - Originally announced (if announced)
1 - Announced as an aggregation prefix
[4:3]
00 (0) - Not explored
01 (1) - Valid (AS or children ASs are indirect customers of testing AS)
10 (2) - Ceiling (AS which announces it, or a less specific prefix, is an indirect provider of testing AS)
11 (3) - Invalid (Neither of the above)
*/

typedef struct _entry{
	unsigned short AS;
	unsigned char ctrl;
	neigh * ASs;
	struct _entry * parent;
	struct _entry * zero;
	struct _entry * one;
} entry;

typedef struct _dfs{
	entry * element;
	unsigned int addr;
	unsigned char level;
	struct _dfs * next;
} dfs;

node network[MAXAS];
entry * PrefixTree[2];
unsigned int all_prefs;
unsigned int added_prefs;
char changed;

#endif
