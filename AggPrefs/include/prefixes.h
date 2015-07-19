#ifndef _PREFIXES_H_
#define _PREFIXES_H_

unsigned int BitOne(unsigned char position);
char isCompatible(unsigned int pref1, unsigned char len1, unsigned int pref2, unsigned char len2);
void InsertPrefix(unsigned int addr, unsigned char length, unsigned short AS, char type);
void RemovePrefix(entry * pref);
void ReadPrefixFile(char * filename);
void AnalyzePartitions();
neigh * RemoveProviders(neigh * ASs, unsigned short cust);
char HasCustomer(neigh * ASs, unsigned short prov);
void AnnounceAggPrefixes(unsigned short src);
void TestAllAgg();
void PrintPrefixes(char * filename);

#endif