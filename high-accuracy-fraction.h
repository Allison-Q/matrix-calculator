#include <stdbool.h>
#include "high-accuracy-integer.h"

// This module provides basic calculations for arbitrarily large fractions

// The following applies to all functions:
// requires: all number parameters are valid (not NULL)
// time: if parameter type is ha_int, (n) (m) is the number (so number of 
// digits is logn); otherwise, 
// (n1), (n2) are numerator and denominator of number a coorespondingly, if not 
// specified (so numbers of digits are logn, logm), if there are 2 numbers,
// n1, n2 are nume and denom of the first number paremeter, m1, m2 are nume and
// denom of the second


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
// effects: may allocate memory (client must call ha_frac_destroy)
//          may produce output (error message)
// time: O(n * logn * logm), where n is numerator, m is denominator; 
//       or n is denominator, m is numerator
struct ha_frac *ha_frac_create(const char *numerator, const char *denominator);

// ha_frac_destroy(num) destroys num
// effects: num is no longer valid
// time: O(1)
void ha_frac_destroy(struct ha_frac *num);

// ha_frac_print() prints the ha_frac followed by an optional \n (if newline is
//   true)
// notes: if num is an integer, then print "numerator" only
//        if num is a fraction, then print "numerator/denominator"
// effects: prints output
// time: O(log(n1) + log(n2))
void ha_frac_print(const struct ha_frac *num, bool newline);

// ha_frac_copy(num) returns a copy of num
// effects: allocates memory(caller must call ha_frac_destroy)
// time:O(n1 * logn1 * logn2)
struct ha_frac *ha_frac_copy(const struct ha_frac *num);

// ha_frac_add(n, m) gives n + m
// effects: allocates memory (caller must free)
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
struct ha_frac *ha_frac_add(const struct ha_frac *n, const struct ha_frac *m);

// ha_frac_sub(n, m) gives n - m
// effects: allocates memory (caller must free)
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
struct ha_frac *ha_frac_sub(const struct ha_frac *n, const struct ha_frac *m);

// ha_frac_mult(n, m) gives n * m
// effects: allocates memory (caller must free)
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
struct ha_frac *ha_frac_mult(const struct ha_frac *n, const struct ha_frac *m);

// ha_frac_div(n, m) gives n / m
// requires: m is not zero
// effects: allocates memory (caller must free)
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
struct ha_frac *ha_frac_div(const struct ha_frac *n, const struct ha_frac *m);

// ha_frac_cmp(n, m) returns 1 if n > m, 0 if n == m, -1 if n < m
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
int ha_frac_cmp(const struct ha_frac *n, const struct ha_frac *m);

// ha_frac_is_frac(num) returns false if num is an integer, true otherwise
// time: O(log(n2))
bool ha_frac_is_frac(const struct ha_frac *num);

// ha_frac_to_str(num) returns the cooresponding string of num
// effects: allocates memory(caller must free)
// time: O(log(n1) + log(n2))
char *ha_frac_to_str(const struct ha_frac *num);
