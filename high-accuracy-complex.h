#include <stdbool.h>
#include "high-accuracy-fraction.h"

// This module provides basic calculations for arbitrarily large complex numbers

// The following applies to all functions:
// requires: all number parameters are valid (not NULL)
// time: n1 = max(real_nume, ima_nume), n2 = max(real_denom, ima_denom) if not 
// specified (so numbers of digits are log(n1), log(n2)), if there are 
// 2 numbers, n1, n2 are max(real_nume, ima_nume), max(real_denom, ima_denom)
// of the first number paremeter, m1, m2 are of the second


struct ha_comp;

// ha_comp_create(real_nume, real_denom, ima_nume, ima_denom) returns a struct
// ha_comp with the parameters given, or returns NULL if at least one of them
// is invalid
// notes: valid real_nume, real_denom, ima_nume, ima_denom satisfy:
//          1. all of them are valid integers
//          2. both denoms are non-zero
//        if either one is invalid, error message(s) will be 
//        printed
// examples: 0 1 0 1=> 0
//           1 2 0 2=> 1/2
//           12 34 1 2=> 6/17+(1/2)i
//           -1 2 -3 3=> -1/2-i
//           1 0 1 1is invalid (returns NULL)
// requires: real_nume, real_denom, ima_nume, ima_denom are not NULL
// effects: may allocate memory (client must call ha_comp_destroy)
//          may produce output (error message)
// time: O(n * logn * logm) or O(m * logn *logm) 
// where n = max(real_nume, ima_nume), m = max(real_denom, ima_denom) 
struct ha_comp *ha_comp_create(const char *real_nume, const char *real_denom,
                               const char *ima_nume, const char *ima_denom);

// ha_comp_destroy(num) destroys num
// effects: num is no longer valid
// time: O(1)
void ha_comp_destroy(struct ha_comp *num);

// ha_frac_print() prints num followed by an optional \n (if newline is true)
// notes: if num is real, then print the real part only
//        if num is not real, assume real = a and ima = b, then print a+bi if b
// is an int; otherwise print a+(b)i
// effects: prints output
// time: O((n2) * log(n2) + log(n1))
void ha_comp_print(const struct ha_comp *num, bool newline);

// ha_comp_is_zero(num) returns true is num == 0; false otherwise
// time: O((n2) * log(n2))
bool ha_comp_is_zero(const struct ha_comp *num);

// ha_comp_is_zero(num) returns true is num == 1; false otherwise
// time: O((n2) * log(n2))
bool ha_comp_is_one(const struct ha_comp *num);

// ha_comp_add(n, m) gives n + m
// effects: allocates memory (caller must call ha_comp_destroy)
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
struct ha_comp *ha_comp_add(const struct ha_comp *n, const struct ha_comp *m);

// ha_comp_add(n, m) gives n - m
// effects: allocates memory (caller must call ha_comp_destroy)
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
struct ha_comp *ha_comp_sub(const struct ha_comp *n, const struct ha_comp *m);

// ha_comp_add(n, m) gives n * m
// effects: allocates memory (caller must call ha_comp_destroy)
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
struct ha_comp *ha_comp_mult(const struct ha_comp *n, const struct ha_comp *m);

// ha_comp_add(n, m) gives n / m
// requires: m != 0
// effects: allocates memory (caller must call ha_comp_destroy)
// time: O((n2) * log(n2) * log(m2)) or O((m2) * log(m2) * log(n2))
struct ha_comp *ha_comp_div(const struct ha_comp *n, const struct ha_comp *m);

// ha_comp_to_str(num) returns the cooresponding string of num
// effects: allocates memory(caller must free)
// time: O(log(n1) + log(n2))
char *ha_comp_to_str(const struct ha_comp *num);
