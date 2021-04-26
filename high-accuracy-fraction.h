// This module provides basic calculations for arbitrarily large fractions

// The following applies to all functions:
// requires: all number parameters are valid (not NULL)
// time: (n), (m) are the numbers (so numbers of digits are logn, logm)


struct ha_frac;


// ha_frac_create(numerator, denominator) creates an ha_frac with the numerator
//   and the denominator given, or returns NULL if at least one of them is 
//   invalid
// notes: valid numerators and denominators satisfy:
//          1. both of them are valid integers
//          2. denominator is non-zero
//        if either numerator or denominator is invalid, an error message 
//          will be printed
// examples: 0 1 => 0
//           1 2 => 1/2
//           12 34 => 6/17
//           -1 2 => -1/2
//           1 0 is invalid (returns NULL)
// requires: numerator and denominator are valid ha_ints (not NULL)
// effects: may allocate memory (client must call ha_frac_destroy)
//          may produce output (error message)
// time: O(logn)
struct ha_frac *ha_frac_create(const struct ha_int *numerator, 
                               const struct ha_int *denominator);

// ha_frac_destroy(num) destroys num
// effects: num is no longer valid
// time: O(1)
void ha_frac_destroy(struct ha_frac *num);

// ha_frac_print() prints the ha_frac followed by an optional \n (if newline is
//   true)
// notes: if num is an integer, then print "numerator" only
//        if num is an fraction, then print "nunmerator/denominator"
// effects: prints output
// time: O(logn)
void ha_frac_print(const struct ha_frac *num, bool newline);

// ha_frac_add(n, m) gives n + m
// effects: allocates memory (caller must free)
// time: 
struct ha_frac *ha_frac_add(const struct ha_frac *n, const struct ha_frac *m);

// ha_frac_sub(n, m) gives n - m
// effects: allocates memory (caller must free)
// time: 
struct ha_frac *ha_frac_sub(const struct ha_frac *n, const struct ha_frac *m);

// ha_frac_mult(n, m) gives n * m
// effects: allocates memory (caller must free)
// time: 
struct ha_frac *ha_frac_mult(const struct ha_frac *n, const struct ha_frac *m);

// ha_frac_div(n, m) gives n / m
// requires: m is not zero
// effects: allocates memory (caller must free)
// time:
struct ha_frac *ha_frac_div(const struct ha_frac *n, const struct ha_frac *m);

