// This module turns the input into the corresponding ha_comp

#include "high-accuracy-complex.h"


// read_input(num) turns num into the corresponding ha_comp
// note: if num is an invalid complex (real) number, then return NULL and print
//         an error message
// effects: may allocate memory (client must call ha_comp_destroy)
// time: O(n)
struct ha_comp *read_input(char *num); 
