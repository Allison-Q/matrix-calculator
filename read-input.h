// This module reads input and gives the corresponding ha_comp

#include "high-accuracy-complex.h"


// read_input(num) reads input and returns the corresponding ha_comp
// note: if num is an invalid complex (real) number, then return NULL and print
//         an error message
// effects: may allocate memory (client must call ha_comp_destroy)
// time: O(n) where n is the length of num
struct ha_comp *read_input(void); 
