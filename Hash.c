/****************************************************************************

                                                Javisth Chabria
                                                CSE 12, Spring 2017
                                                10th May 2017
                                                cs12xaq
                              Assignment 6

File Name:      Hash.c
Description:    This program implements a hash table
****************************************************************************/
#include <cstdlib>
#include <string>
#include "Hash.h"

using namespace std;

static const char AND[] = " and ";
static const char DEBUG_ALLOCATE[] = " - Allocated]\n";
static const char DEBUG_DEALLOCATE[] = " has been deallocated]\n";
static const char DEBUG_LOCATE[] = " - Locate]\n";
static const char DEBUG_LOOKUP[] = " - Lookup]\n";
static const char BUMP[] = "[Bumping To Next Location...]\n";
static const char COMPARE[] = " - Comparing ";
static const char FOUND_SPOT[] = " - Found Empty Spot]\n";
static const char HASH[] = "[Hash Table ";
static const char HASH_VAL[] = "[Hash value is: ";
static const char INSERT[] = " - Inserting ";
static const char PROCESSING[] = "[Processing ";
static const char TRYING[] = "[Trying index ";

bool HashTable :: debug = 0;
int HashTable :: counter = 0;
int count = 0;

void HashTable :: Set_Debug (int option)

/***************************************************************************
% Routine Name : HashTable :: Set_Debug (public)
% File :         hash.c
% 
% Description :  This function sets debug mode on or off
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/

{
        debug = option;
}


HashTable :: HashTable (int sz) : size (sz),
	table_count(++counter), occupancy (0), table (new Base *[sz]), 
	probeCount (new int[sz])
/***************************************************************************
% Routine Name : HashTable :: HashTable (public)
% File :         hash.c
% 
% Description :  This function allocates and initializes the memory
%                associated with a hash table.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% size               The number of elements for the table...MUST BE PRIME!!!
***************************************************************************/

{ 
        /* initialize all table elements */
	index = 0;
	for ( int index = 0; index < size; index++ ){
		table[index] = 0;
		probeCount[index] = 0;
	}
	if( debug ){
		cerr<<HASH<<counter<<DEBUG_ALLOCATE;
	}

}	/* end: HashTable */


HashTable :: ~HashTable (void)
/***************************************************************************
% Routine Name : HashTable :: ~HashTable  (public)
% File :         hash.c
% 
% Description :  deallocates memory associated with the Hash Table.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
        int index = 0;   /* initializing index */
	while ( index < size ){
		if ( table[index] != 0)
	/* call function to delete individual elements */
	             delete table[index];
	             index ++;
	}
	/* delete table itself */
	delete[] table;
	delete[] probeCount;
}	/* end: ~HashTable */


bool HashTable :: Insert (Base * element, int initialCount) /*false*/
/***************************************************************************
% Routine Name : HashTable :: Insert (public)
% File :         hash.c
% 
% Description : This function will insert the element in the hash table.
%		If the element cannot be inserted, false will be returned.
%		If the element can be inserted, the element is inserted
%		and true is returned.  Duplicate insertions will
%		cause the existing element to be deleted, and the duplicate
%		element to take its place.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert.
% recursiveCall      Whether or not we are entering from recursion
% <return>           true or false indicating success or failure of insertion
*************************************************************************/
{
	count = initialCount; /* storing initialCount in count */
	int ASCIIsum;    /* ASCII sum of element */
	int increment;   /* variable to increment index */

	/* initializing values */
	ASCIIsum = (long)*element;
	index = ASCIIsum % size;
	increment = (ASCIIsum %(size-1))+1;

	/* checking if not first iteration */
	if(count!=1){
		index = (index+increment)%size;
	}
        if(count = 1){
		index = ASCIIsum %size;
	}
        /* debug messages */
	if ( debug ){
		cerr<<HASH<<table_count<<INSERT<<(char*)*element<<"]\n";
		cerr<<TRYING<<index<<"]\n";
	}
	/* checking if element is in location */
	if ( Locate(element) ){
		table[index] = element;
		return true;
	}
	else{
		/* checking if array is full */
		if ( size == occupancy ){
			return false;
		}
		/* checking if index is null */
		else if ( table[index] == NULL ){
			table[index] = element;
			probeCount[index]=count;
			/* debug messages */
			if ( debug ){
				cerr<<HASH<<table_count<<FOUND_SPOT;
			}
			occupancy ++;
			return true;
		}
		/* checking how long the element has travelled */
	        else if ( count > probeCount[index] ){
			Base* copy = table[index];
			initialCount = initialCount+1;
			/* debug messages */
			if ( debug ){
				cerr<<BUMP;
			}
			return Insert(copy,initialCount);
		}
	}
	return false;

}


bool HashTable :: Locate (Base * element)
/**********************************************************************
% Routine Name : HashTable :: Locate (private)
% File :         HashTable.c
% 
% Description : This function will locate the location in the
%               table for the insert or lookup.
%
% Parameters descriptions :
%  
% name               description
% ------------------ -------------------------------------------------
% element            The element needing a location.
% <return>           true if item found, or false if not found
**********************************************************************/

{
        int ASCIIsum;    /* ASCII sum of element */
	int increment;   /* variable to increment index */

	/* initializing values */
	ASCIIsum = (long)*element;
	index = ASCIIsum % size;
	increment = (ASCIIsum %(size-1))+1;

	/* debug messages */
	if ( debug ){
		cerr<<HASH<<table_count<<DEBUG_LOCATE;
		cerr<<PROCESSING<<*element+"]\n";
		cerr<<HASH_VAL<<ASCIIsum<<"]\n";
	}
	while ( count < size ){
		/* debug messages */
		if ( debug ){
			cerr<<TRYING<<index<<"]\n";
		}
		/* checking if element at index is null */
		if ( table[index] == 0){
			/* debug messages */
			if ( debug ){
				cerr<<HASH<<table_count<<FOUND_SPOT;
			}
			return false;
		}
		/* checking if element is in table */
		if ( element == table[index]) {
			if ( debug ){
				cerr<<HASH<<table_count<<COMPARE
				<<element<<AND<<table[index]<<"]\n";
			}
			return true;
		}
		/* checking how long the element has travlled in array */
		if ( table[index]!=NULL && count > probeCount[index] ){
		        increment = (ASCIIsum % (size-1))+1;
			index = (index + increment)%size;
			return false;
	        }
		count++;
	}
	
	return false;
}

Base * HashTable :: Lookup (Base * element)
/***************************************************************************
% Routine Name : HashTable :: Lookup (public)
% File :         hash.c
% 
% Description : This function will lookup the element in the hash table.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert or to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{
	/* debug messages */
	if ( debug ){
		cerr<<HASH<<table_count<<DEBUG_LOOKUP;
	}

	if ( Locate(element) ){
		return table[index];
	}
	else return NULL;
}


ostream & HashTable :: Write (ostream & stream)
/***************************************************************************
% Routine Name : HashTable :: Write (public)
% File :         hash.c
% 
% Description : This funtion will output the contents of the hash table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
	stream << "Hash Table " << table_count << ":\n"
	       << "size is " << size << " elements, "
	       << "occupancy is " << occupancy << " elements.\n";

	/* go through all table elements */
	for (int index = 0; index < size; index++)
		if (table[index])
			table[index]->Write(stream << "at index "
			<< index << ": ") << " with probeCount: "
			<< probeCount[index] << "\n";
	return stream;
}	/* end: Write */
