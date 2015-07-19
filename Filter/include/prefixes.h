#ifndef _PREFIXES_H_
#define _PREFIXES_H_

char isCompatible(unsigned int prefix1, unsigned char length1, unsigned int prefix2, unsigned char length2);
void InsertPrefix(unsigned int IP, unsigned char length, unsigned short AS, char type);
void CreatePrefixTree(char * filename);
void DestroyPrefixTree();

#endif