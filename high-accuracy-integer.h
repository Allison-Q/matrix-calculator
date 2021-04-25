// This module provides basic calculations for arbitrarily large integers

// The following applies to all functions:
// requires: all number parameters are valid (not NULL)
// time: (n) is the number (so number of digits is logn)


struct ha_int;


// ha_int_create(s) creates an ha_int with the integer represented by s, or
//   returns NULL if s is an invalid integer
// notes: a valid integer satisfies:
//          1. an optional negative sign (-) followed by digits
//          2. there is at least one digit
//          3. 0 has no negative sign ahead
//          4. only negative sign and numbers appear
//          5. neither empty, nor having successive 0s ahead
//        if s is an invalid integer, an error message is printed
// examples: "12" => 12
//           "-12" => -12
//           "0" => 0
//           "012" is invalid (returns NULL)
//           "00" is invalid (returns NULL)
//           "-0" is invalid (returns NULL)
//           "-" is invalid (returns NULL)
//           "1a" is invalid (returns NULL)
//           "" is invalid (returns NULL)
// requires: s is a valid string (not NULL)
// effects: may allocate memory (client must call ha_int_destroy)
//          may produce output (error message)
// time: O(logn)
struct ha_int *ha_int_create(const char *s);

// ha_int_destroy(num) destroys num
// effects: num is no longer valid
// time: O(1)
void ha_int_destroy(struct ha_int *num);

// ha_int_print() prints the ha_int followed by an optional \n (if newline is 
//   true)
// effects: prints output
// time: O(logn)
void ha_int_print(const struct ha_int *num, bool newline);

// ha_int_add(n, m) gives n + m
// effects: allocates memory (caller must free)
// time: O(logn + logm)
struct ha_int *ha_int_add(const struct ha_int *n, const struct ha_int *m);

// ha_int_sub(n, m) gives n - m
// effects: allocates memory (caller must free)
// time: O(logn + logm)
struct ha_int *ha_int_sub(const struct ha_int *n, const struct ha_int *m);

// ha_int_mult(n, m) gives n * m
// effects: allocates memory (caller must free)
// time: O(logn * logm)
struct ha_int *ha_int_mult(const struct ha_int *n, const struct ha_int *m);

// ha_int_quotient(n, m) gives quotient when n / m
// requires: m is not zero
// effects: allocates memory (caller must free)
// time: O(logn * logm)
struct ha_int *ha_int_quotient(const struct ha_int *n, const struct ha_int *m);

// ha_int_remainder(n, m) gives remainder when n / m
// note: n = m * quotient + remainder
// requires: m is not zero
// effects: allocates memory (caller must free)
// time: O(logn * logm)
struct ha_int *ha_int_remainder(const struct ha_int *n, const struct ha_int *m);

