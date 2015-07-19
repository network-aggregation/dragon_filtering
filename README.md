# DRAGON's filtering code

This README is aimed at explaining the use of programs Filter and Aggprefs.
Filter is used to measure the filtering efficiency of ASs in a network-wide implementation of DRAGON.
Aggprefs is used for determining the set of aggregation prefixes that the ASs would generate under a network-wide implementation of DRAGON.
Their optimal use is in a UNIX-based operating system such as Linux.


# Compiling the program

In order to execute either of these programs, it might be necessary to initially re-compile them.
To do so, simply open a terminal in the program's main directory and type:

'''
make clean
make
'''

# Executing the program
To execute the programs from a terminal in their main directory, simply type:

-For Filter: ./bin/filter <NETWORK_FILE> <PREFIX_FILE>
-For Aggprefs: ./bin/aggpregs <NETWORK_FILE> <PREFIX_FILE>

The arguments <NETWORK_FILE> and <PREFIX_FILE> are described in further detail in the INPUTS text file.
Both arguments are mandatory.

# Output files

The program Filter outputs a file named "filtering_efficiency.txt" with some statistics regarding the filtering efficiency when considering a network-wide deployment of DRAGON. The output file contains the following sections:

1. Filtering Efficiency per AS

Each of the first lines in the file has the format AS<X> [(STUB)]: <ORIGINAL_#_FIB_ENTRIES> -> <NEW_#_FIB_ENTRIES>, <FILTERING_EFFICIENCY>
- AS<X> [(STUB)] is the AS to which the data refers. In the event that this AS is a stub, then the indication (STUB) is included.
- <ORIGINAL_#_FIB_ENTRIES> is the amount of entries in this AS's FIB after all routes were propagated, without the use of DRAGON
- <NEW_#_FIB_ENTRIES> is the amount of entries in this AS's FIB after all routes were propagated, once DRAGON is implemented
- <FILTERING_EFFICIENCY> is the measure of DRAGON's filtering efficiency in this AS, calculated as <NEW_#_FIB_ENTRIES> / <ORIGINAL_#_FIB_ENTRIES>

The final two lines of this section contain the maximum and the minimum filtering efficiencies obtained, respectively.

2. Complementary Cumulative Distribution of Filtering Efficiency

The following section contains pairs of values which are used to graph the complementary cumulative distribution function of filtering efficiency.
In each pair <X> <Y>, <X> is the number of ASs which have a filtering efficiency that is either equal to, or greather than <Y>.
This section is divided in two sub-sections: the first one pertains to all ASs, while the second one pertains exclusively to non-stub ASs.

The program Aggprefs outputs a prefix file "prefixes_with_agg.txt" with the same format as the input <PREFIX_FILE> which contains all of the IPv4 prefixes received in the input file, as well as the newly generated aggregation prefixes.
