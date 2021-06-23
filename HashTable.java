/****************************************************************************

                                                Javisth Chabria
                                                CSE 12, Spring 2017
                                                10th May 2017
                                                cs12xaq
                              Assignment 6

File Name:      HashTable.java
Description:    This program implements a hash table
****************************************************************************/
public class HashTable extends Base {

	private static int counter = 0;	// number of HashTables so far
        private static boolean debug;        // allocation of debug state

        // set in locate, last location checked in hash table 
        private int index = 0; 

        // set in insert/lookup, count of location in probe sequence
        private int count = 0;
        
        private int probeCount[];   // where we are in the probe sequence 
        private long occupancy;     // how many elements are in the Hash Table
        private int size;           // size of Hash Table
        private Base table[];       // the Hash Table itself ==> array of Base
        private int tableCount;     // which hash table it is

        // messages
        private static final String AND = " and ";
        private static final String DEBUG_ALLOCATE = " - Allocated]\n";
        private static final String DEBUG_LOCATE = " - Locate]\n";
        private static final String DEBUG_LOOKUP = " - Lookup]\n";
        private static final String BUMP = "[Bumping To Next Location...]\n";
        private static final String COMPARE = " - Comparing ";
        private static final String FOUND_SPOT = " - Found Empty Spot]\n";
        private static final String HASH = "[Hash Table ";
	private static final String HASH_VAL = "[Hash Value Is ";
        private static final String INSERT = " - Inserting ";
        private static final String PROCESSING = "[Processing ";
        private static final String TRYING = "[Trying Index ";

        
        /**
         * This method sets the debug on
         *
         * @param  none
         * @return nothing
         */

        public static void debugOn () {
                // setting debug to true
		debug = true;
        }

        /**
         * This method sets the debug off
         *
         * @param  sz   The number of elements for the table...MUST BE PRIME!!!
         */
        public static void debugOff () {
                // setting debug to false
		debug = false;
        }

        /**
         * This method allocates and initializes the memory
         * associated with a hash table.
         *
         * @param  sz   The number of elements for the table...MUST BE PRIME!!!
         */
        public HashTable (int sz) {
                
		int init = 0;            // loop variable
		size = sz;               // initializing size of table
		table = new Base[size];  // initializing hash table
		probeCount = new int[size]; // initializing probe table
		for ( init =0; init < size; init++ ){
			// setting default values of hash table to null
			table[init] = null;
			probeCount[init] = 0;

		}
		// incrementing number of hash tables created
		tableCount ++;
		counter ++;
		// debug messages
		if ( debug == true ){
			System.err.print(HASH+tableCount+DEBUG_ALLOCATE);
		}
        }


        /**
         * This method will insert the element in the hash table.
         * If the element cannot be inserted, false will be returned.
         * If the element can be inserted, the element is inserted
         * and true is returned.  Duplicate insertions will
         * cause the existing element to be deleted, and the duplicate
         * element to take its place.
         *
         * @param   element       The element to insert.
         * @param   initialCount  Where to start in probe seq (recursive calls) 
         * @return  true or false indicating success or failure of insertion
         */
        public boolean insert (Base element, int initialCount) {
		count = initialCount; // storing initialCount value to count
                int initial_location;  // variable to store location
		int increment;         // variable to store increment value
		int numeric_attribute = element.hashCode();// storing hashvalue
		initial_location = numeric_attribute % size; // storing location
		increment = (numeric_attribute %(size-1))+1; // incrementing
		index = initial_location;
		// checking if it is not the first element inserted
		if (count!=1){
			index = (index+increment)%size;
		}
		if (count == 1){
			index = initial_location;
		}
                // debug messages
		if ( debug == true ){
			System.err.println(HASH+tableCount+INSERT+
			element.getName()+"]");
		}
		// checking if element is found at location
                if ( locate(element) ){
		    table[index] = element;
                    return true;
		}
		else{ 
			// checking if array is full
			if ( size == occupancy ){
				return false;
			}
			// checking if array index is null
			else if( table[index]==null ){
			        table[index] = element;
			        probeCount[index] = count;
				// debug statement
				if ( debug == true ){
					System.err.print(HASH+tableCount
					+FOUND_SPOT);
				}
				occupancy++;
			        return true;
			}
			// checking how far element has travelled
		        else if ( count > probeCount[index] ){
		            Base copy = table[index];
			    initialCount ++;
			    // debug statement
			    if ( debug == true ){
				System.err.print(BUMP);
			    }
			    return insert(copy, initialCount);
			}
		}
	        return false;
        }


        /**
         * This method will locate the location in the
         * table for the insert or lookup.
         *
         * @param   element  The element needing a location.
         * @return  true if item found, or false if not found
         */
        private boolean locate (Base element) {
                int initial_location;  // variable to store location
		int increment;         // variable to store increment value
		int numeric_attribute = element.hashCode();// storing hashvalue
		initial_location = numeric_attribute % size; // storing location
		increment = (numeric_attribute %(size-1))+1; // incrementing
		// debug messages
		if ( debug == true ){
			System.err.print(HASH+tableCount+DEBUG_LOCATE);
			System.err.println(PROCESSING+element.getName()+"]");
			System.err.println(HASH_VAL+numeric_attribute+"]");
		}
		for( ; count < size; count++){	
                  // debug messages
		  if ( debug == true ){
			System.err.println(TRYING+index+"]");
		  }
		  // checking if array index is null
		  if ( table[index] == null ){
			return false;
		  }
		  
		  // if the element is in the index
		  if ( table[index].getName().equals(element.getName()) ){
		        // debug messages
			if ( debug == true ){
			     System.err.println(HASH+tableCount+
		      COMPARE+element.getName()+AND+table[index].getName()+"]");
			}
			return true;
		  }
                  // checking how far an element has travelled
                  if( table[index]!=null && count > probeCount[index] ){
			increment = (numeric_attribute % (size - 1))+1;
			index = (index + increment) % size;
			return false;
		  }
         
		}
		return false;
        }


        /**
         * This method will lookup the element in the hash table.  If
         * found a pointer to the element is returned.  If the element
         * is not found, NULL will be returned to the user.
         *
         * @param   element  The element to look up.
         * @return  A pointer to the element if found, else NULL
         */
        public Base lookup (Base element) {
		// debug messages
		if ( debug == true ){
			System.err.println(HASH+tableCount+DEBUG_LOOKUP);
		}
		if ( locate(element) == true ){
			return table[index];
		}
		else return null;
        }


        /**
         * Creates a string representation of the hash table. The method 
         * traverses the entire table, adding elements one by one ordered
         * according to their index in the table. 
         *
         * @return  String representation of hash table
         */
        public String toString () {
                String string = "Hash Table " + tableCount + ":\n";
                string += "size is " + size + " elements, "; 
                string += "occupancy is " + occupancy + " elements.\n";
   
                /* go through all table elements */
                for (int index = 0; index < size; index++) {
			if (table[index] != null) {
				string += "at index " + index + ": ";
                                string += "" + table[index];
                                string += " with probeCount: "; 
                                string += probeCount[index] + "\n";
                        }
                }

                return string;
        }
}
