// This module provides basic calculations for arbitrarily large integers

// For all program scope functions, see high-accuracy-integer.h for details

// The following applies to all functions:
// requires: all number parameters are valid (not NULL)
// time: (n) is the number (so number of digits is logn)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "high-accuracy-integer.h"


struct ha_int {
  bool sign; // true for positive and 0, false for negative
  char *digits; // stored in a reverse order for convenience
};


// print_invalid_integer(s) prints an error message in the form 
//   "Error: s is an invalid integer"
// requires: s is a valid string (not NULL)
// effects: produces output
// time: O(logn)
static void print_invalid_integer(const char *s) {
  assert(s);
  printf("Error: %s is an invalid integer\n", s);
}

struct ha_int *ha_int_create(const char *s) {
  assert(s);
  const int s_len = strlen(s);
  struct ha_int *integer = malloc(sizeof(struct ha_int));
  integer->sign = true;

  // situations where s is an invalid integer
  if (s_len == 0) { // s is an empty string
    print_invalid_integer(s);
    free(integer);
    return NULL;
  }
  int s_idx = 0; // current index of s
  int digit_num = s_len; // number of digits in s
  if (s[0] == '-') { // s is negative
    if (s_len == 1 || s[1] < '1' || s[1] > '9') {
      print_invalid_integer(s);
      free(integer);
      return NULL;
    }
    integer->sign = false;
    ++s_idx;
    --digit_num;
  }
  if (s[0] == '0' && s_len > 1) { // successive 0s ahead
    print_invalid_integer(s);
    free(integer);
    return NULL;
  }
  for (int i = s_idx; i < s_len; ++i) {
    if (s[i] < '0' || s[i] > '9') { // s has other invalid chars
      print_invalid_integer(s);
      free(integer);
      return NULL;
    }
  }

  // s is a valid integer
  integer->digits = malloc((digit_num + 1) * sizeof(char));
  for (int j = digit_num - 1; j >= 0; --j) {
    integer->digits[j] = s[s_idx];
    ++s_idx;
  }
  integer->digits[digit_num] = '\0';
  return integer;
}

void ha_int_destroy(struct ha_int *integer) {
  assert(integer);
  free(integer->digits);
  free(integer);
}

void ha_int_print(const struct ha_int *integer, bool newline) {
  assert(integer);
  if (!integer->sign) { // negative
    printf("-");
  }

  const int len = strlen(integer->digits);
  for (int i = len - 1; i >= 0 ; --i) {
    printf("%c", integer->digits[i]);
  }

  if (newline) {
    printf("\n");
  }
}

struct ha_int *ha_int_copy(const struct ha_int *n) {
  assert(n);
  struct ha_int *new_int = malloc(sizeof(struct ha_int));
  new_int->sign = n->sign;
  const int n_len = strlen(n->digits);
  new_int->digits = malloc((n_len + 1) * sizeof(char));
  strcpy(new_int->digits, n->digits);
  return new_int;
}

bool ha_int_eq(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);

  // different signs
  if (n->sign != m->sign) {
    return false;
  }

  // different length
  const int n_len = strlen(n->digits);
  const int m_len = strlen(m->digits);
  if (n_len != m_len) {
    return false;
  }

  // same sign and length
  for (int i = n_len - 1; i >= 0; --i) {
    if (n->digits[i] != m->digits[i]) { // different digits
      return false;
    }
  }

  return true;
}

// abs_gt(n, m) determines if |n| > |m|
// time: O(logn + logm)
static bool abs_gt(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);
  const int n_len = strlen(n->digits);
  const int m_len = strlen(m->digits);

  if (n_len > m_len) { // n has more digits
    return true;
  } else if (n_len < m_len) { // n has less digits
    return false;
  } else { // same number of digits
    for (int i = n_len - 1; i >= 0; --i) {
      if (n->digits[i] > m->digits[i]) {
        return true;
      } else if (n->digits[i] < m->digits[i]) {
        return false;
      }
    }
  }
  // n == m
  return false;
}

bool ha_int_gt(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);

  // n and m have different signs
  if (n->sign && !m->sign) { // n is +ve/0, m is -ve
    return true;
  } else if (!n->sign && m->sign) { // n is -ve, m is +ve/0
    return false;
  }

  // n and m have same signs
  if (n->sign) { // positive or 0
    return abs_gt(n, m);
  } else { // negative
    return abs_gt(m, n);
  }
}

// max(a, b) finds the bigger value between a and b
static int max(int a, int b) {
  if (a >= b) {
    return a;
  } else {
    return b;
  }
}

// get_digit(s, len, pos) gets the real digit s[pos] represents, returning 0
//   if pos >= len
// requires: s is a valid integer string (not NULL)
//           len > 0
//           pos >= 0
// time: O(1)
static int get_digit(const char *s, int len, int pos) {
  assert(s);
  assert(len > 0);
  assert(pos >= 0);
  if (pos >= len) {
    return 0;
  }
  else {
    return s[pos] - '0';
  }
}

// remove_leading_zeros(num) deletes invalid leading zeros in num
// note: modified num should be a valid integer
// effects: may modify num
// time: O(logn)
static char *remove_leading_zeros(char *num) {
  assert(num);
  int len = strlen(num);
  while (num[len - 1] == '0' && len > 1) {
    --len;
  }
  num = realloc(num, (len + 1) * sizeof(char));
  num[len] = '\0';
  return num;
}

// init_new_int(n, len) initializes n so that all the char are '0'
// requires: len >= 0
// effects: modifies num
// time: O(logn)
static void init_new_int(struct ha_int *n, int len) {
  assert(n);
  assert(len >= 0);
  for (int i = 0; i < len; ++i) {
    n->digits[i] = '0';
  }
  n->digits[len] = '\0';
}

// abs_add(n, m) gives |n| + |m|
// effects: allocates memory (caller must free)
// time: O(logn + logm)
static struct ha_int *abs_add(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);
  const int n_len = strlen(n->digits);
  const int m_len = strlen(m->digits);
  struct ha_int *new_int = malloc(sizeof(struct ha_int));
  new_int->sign = true;
  int new_int_len = max(n_len, m_len) + 1; // maximum length of new_int
  new_int->digits = malloc((new_int_len + 1) * sizeof(char));
  init_new_int(new_int, new_int_len);

  // add each digit
  int carry = 0;
  for (int j = 0; j < new_int_len; ++j) {
    const int sum = carry + get_digit(m->digits, m_len, j) +
      get_digit(n->digits, n_len, j);
    new_int->digits[j] = sum % 10 + '0';
    carry = sum / 10;
  }

  new_int->digits = remove_leading_zeros(new_int->digits);
  return new_int;
}

// abs_big_sub_small(n, m) gives max(|m|, |n|) - min(|m|, |n|)
// effects: allocates memory (caller must free)
// time: O(logn + logm)
static struct ha_int *abs_big_sub_small(const struct ha_int *n, 
                                        const struct ha_int *m) {
  assert(n);
  assert(m);
  const int n_len = strlen(n->digits);
  const int m_len = strlen(m->digits);
  struct ha_int *new_int = malloc(sizeof(struct ha_int));
  new_int->sign = true;
  int new_int_len = max(n_len, m_len); // maximum length of new_int
  new_int->digits = malloc((new_int_len + 1) * sizeof(char));
  init_new_int(new_int, new_int_len);

  // find max(|m|, |n|) and min(|m|, |n|)
  char *big = malloc((new_int_len + 1) * sizeof(char));
  int big_len = 0;
  char *small = malloc((new_int_len + 1) * sizeof(char));
  int small_len = 0;
  if (abs_gt(n, m)) { // |n| > |m|
    strcpy(big, n->digits);
    big_len = n_len;
    strcpy(small, m->digits);
    small_len = m_len;
  } else { // |n| <= |m|
    strcpy(big, m->digits);
    big_len = m_len;
    strcpy(small, n->digits);
    small_len = n_len;
  }

  // big - small for each digit
  int carry = 0;
  for (int i = 0; i < new_int_len; ++i) {
    int sub = carry + get_digit(big, big_len, i) - 
      get_digit(small, small_len, i);
    if (sub < 0) {
      sub += 10;
      carry = -1;
    } else {
      carry = 0;
    }
    new_int->digits[i] = sub % 10 + '0';
  }

  new_int->digits = remove_leading_zeros(new_int->digits);
  free(big);
  free(small);
  return new_int;
}

struct ha_int *ha_int_add(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);
  if (n->sign == m->sign) { // n and m have same signs
    struct ha_int *new_int = abs_add(n, m);
    new_int->sign = n->sign;
    return new_int;
  } else { // n and m have different signs
    struct ha_int *new_int = abs_big_sub_small(n, m);
    if (abs_gt(n, m)) { // |n| > |m|
      // when n >= 0 and m < 0, sign is true
      // when n < 0 and m >= 0, sign is false
      if (!n->sign) {
        new_int->sign = false;
      }
    } else if (abs_gt(m, n)) { // |n| < |m|
      // when n >= 0 and m < 0, sign is false
      if (n->sign) {
        new_int->sign = false;
      }
      // when n < 0 and m >= 0, sign is true
    }
    return new_int;
  }
}

struct ha_int *ha_int_sub(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);
  if (n->sign != m->sign) { // n and m have different signs
    struct ha_int *new_int = abs_add(n, m);
    new_int->sign = n->sign;
    return new_int;
  } else { // n and m have same signs
    struct ha_int *new_int = abs_big_sub_small(n, m);
    if (abs_gt(n, m)) { // |n| > |m|
      // when n,m >= 0 sign is true
      // when n,m < 0 sign is false
      if (!n->sign) {
        new_int->sign = false;
      }
    } else if (abs_gt(m, n)) { // |n| < |m|
      // when n,m >= 0 sign is false
      if (n->sign) {
        new_int->sign = false;
      }
      // when n,m < 0, sign is true
    }
    return new_int;
  }
}

// is_zero(n) determines if n is zero
// requires: n is a valid integer [not asserted]
// time: O(logn)
static bool is_zero(const struct ha_int *n) {
  assert(n);
  const int len = strlen(n->digits);
  return len == 1 && n->digits[0] == '0';
}

// mult_div_sign(n, m) determines the sign for n * m and n / m
// note: returning true means positive or 0, false means negative
// time: O(logn + logm)
static bool mult_div_sign(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);
  if (is_zero(n) || is_zero(m)) {
    return true;
  } else {
    if (n->sign == m->sign) { // same sign
      return true;
    } else { // different signs
      return false;
    }
  }
}

struct ha_int *ha_int_mult(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);
  const int n_len = strlen(n->digits);
  const int m_len = strlen(m->digits);
  struct ha_int *new_int = malloc(sizeof(struct ha_int));
  new_int->sign = mult_div_sign(n, m);
  int new_int_len = n_len + m_len; // maximum length of new_int
  new_int->digits = malloc((new_int_len + 1) * sizeof(char));
  init_new_int(new_int, new_int_len);

  // multiply each digit and add
  int carry = 0;
  for (int i = 0; i < n_len; ++i) {
    for (int j = 0; j < m_len; ++j) {
      const int mult = 
        get_digit(n->digits, n_len, i) * get_digit(m->digits, m_len, j) + 
        get_digit(new_int->digits, new_int_len, i + j) + carry;
      new_int->digits[i + j] = mult % 10 + '0';
      carry = mult / 10;
    }
    new_int->digits[m_len + i] = carry + '0';
    carry = 0;
  }
  new_int->digits = remove_leading_zeros(new_int->digits);
  return new_int;
}

struct ha_int *ha_int_quotient(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);

  // if m == 0, print error message
  if (is_zero(m)) {
    printf("Error: divisor cannot be zero");
    return NULL;
  }

  const int n_len = strlen(n->digits);
  const int m_len = strlen(m->digits);
  struct ha_int *new_int = malloc(sizeof(struct ha_int));
  int new_int_len = n_len; // maximum length of new_int

  // n / m == 0 if |n| < |m|
  if (abs_gt(m, n)) {
    new_int->sign = true;
    new_int->digits = malloc(2 * sizeof(char));
    new_int->digits[0] = '0';
    new_int->digits[1] = '\0';
  } else { // |n| >= |m|
    new_int->sign = mult_div_sign(n, m);
    new_int->digits = malloc((new_int_len + 1) * sizeof(char));
    init_new_int(new_int, new_int_len);
  }

  // initialize the dividend that can be modified
  // set dividend positive for convenience
  struct ha_int *dividend = ha_int_copy(n);
  dividend->sign = true;
  int dividend_len = strlen(dividend->digits);

  // divide and minus
  int idx = 0;
  for (int i = m_len; i <= n_len; ++i) {
    const int len_differ = dividend_len - m_len;

    // give divisor enough zeroes
    // example: for 30 / 2, we adjust 2 to 20, so that the first digit got is
    //          30 / 20 = 1
    // 10^? that divisor multiplied with
    char *ten_to_power = malloc((len_differ + 2) * sizeof(char));
    ten_to_power[0] = '1';
    for (int j = 1; j <= len_differ; ++j) {
      ten_to_power[j] = '0';
    }
    ten_to_power[len_differ + 1] = '\0';
    struct ha_int *multiplier = ha_int_create(ten_to_power);
    free(ten_to_power);

    // set modified_divisor positive for convenience
    struct ha_int *modified_divisor = ha_int_mult(m, multiplier);
    modified_divisor->sign = true;
    ha_int_destroy(multiplier);

    // dividend minus modified_divisor for as many times as possible
    int quotient = 0;
    bool divisor_is_bigger = abs_gt(modified_divisor, dividend);
    struct ha_int *new_dividend = NULL;
    while (!divisor_is_bigger) {
      new_dividend = ha_int_sub(dividend, modified_divisor);
      ha_int_destroy(dividend);
      dividend = new_dividend;
      ++quotient;
      divisor_is_bigger = abs_gt(modified_divisor, dividend); // for efficiency
    }
    new_int->digits[idx] = quotient + '0';
    ++idx;
    --dividend_len;
    ha_int_destroy(modified_divisor);
  }
  ha_int_destroy(dividend);

  // modify the new_int->digits (before, it is reversed)
  for (int k = 0; k < idx / 2; ++k) {
    const char temp = new_int->digits[k];
    new_int->digits[k] = new_int->digits[idx - 1 - k];
    new_int->digits[idx - 1 - k] = temp;
  }
  new_int->digits = remove_leading_zeros(new_int->digits);
  return new_int;
}

struct ha_int *ha_int_remainder(const struct ha_int *n, 
                                const struct ha_int *m) {
  assert(n);
  assert(m);

  // if m == 0, print error message
  if (is_zero(m)) {
    printf("Error: divisor cannot be zero");
    return NULL;
  }
  
  struct ha_int *quotient = ha_int_quotient(n, m);
  struct ha_int *mult = ha_int_mult(m, quotient); // m * quotient
  struct ha_int *new_int = ha_int_sub(n, mult);
  ha_int_destroy(quotient);
  ha_int_destroy(mult);
  return new_int;
}

char *ha_int_to_str(const struct ha_int *n) {
  assert(n);
  int len = strlen(n->digits);
  if (!n->sign) { // negative
    ++len;
  }
  char *num = malloc((len + 1) * sizeof(char));
  
  int front_idx = 0;
  if (!n->sign) {
    num[0] = '-';
    ++front_idx;
  }
  
  int digit = strlen(n->digits) - 1; // current index of n->digits
  for (int i = front_idx; i < len; ++i) {
    num[i] = n->digits[digit];
    --digit;
  }
  num[len] = '\0';
  return num;
}
