// This module provides basic calculations for arbitrarily large fractions

// For all program scope functions, see high-accuracy-fraction.h for details

// The following applies to all functions:
// requires: all parameters are valid (not NULL)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "high-accuracy-fraction.h"
#include "high-accuracy-integer.h"

struct ha_frac {
  bool nega;
  struct ha_int *nume;
  struct ha_int *denom;
};

// ha_int_cmp(n, m) returns 1 is n > m, 0 if n == m, -1 if n < m
// time: O(logn + logm)
static int ha_int_cmp(const struct ha_int *n, const struct ha_int *m) {
  assert(n);
  assert(m);
  if (ha_int_gt(n, m)) {
    return 1;
  } else if (ha_int_eq(n, m)) {
    return 0;
  } else {
    return -1;
  }
}

// ha_int_sign(n) returns 1 if n is positive, -1 if n is negative, otherwise,
// returns 0
// time: O(logn)
static int ha_int_sign(const struct ha_int *n) {
  assert(n);
  struct ha_int *zero = ha_int_create("0");
  int sign = ha_int_cmp(n, zero);
  ha_int_destroy(zero);
  return sign;
}

// get_gcd(small, big) returns GCD of small and big
// requires: small > 0
//           big > 0
// effects: allocates memory(caller must call ha_int_destroy)
// time: O(n * logn * logm), where n = big, m = small
static struct ha_int *get_gcd(struct ha_int *small, struct ha_int *big) {
  assert(small);
  assert(big);
  assert(ha_int_sign(small) > 0);
  assert(ha_int_sign(big) > 0);
  struct ha_int *remain = ha_int_remainder(big, small);
  if (ha_int_sign(remain) == 0) {
    ha_int_destroy(remain);
    return ha_int_copy(small);
  } else {
    struct ha_int *result = get_gcd(remain, small);
    ha_int_destroy(remain);
    return result;
  }
}

// posi_copy(n) returns |n|
// effects: allocates memory(caller must call ha_int_destroy)
// time: O(logn)
static struct ha_int *posi_copy(const struct ha_int *n) {
  assert(n);
  struct ha_int *cpy = NULL;
  if (ha_int_sign(n) < 0) {
    struct ha_int *zero = ha_int_create("0");
    cpy = ha_int_sub(zero, n);
    ha_int_destroy(zero);
  } else {
    cpy = ha_int_copy(n);
  }
  return cpy;
}

// ha_frac_reduc(nume, denom) returns reduction of nume/denom
// requires: denom != 0
// effects: allocates memory(caller must call ha_frac_destroy)
// time: O(n * logn * logm), where n = nume, m = denom; or n = denom, m = nume
static struct ha_frac *ha_frac_reduc(const struct ha_int *nume, 
                                     const struct ha_int *denom) {
  assert(nume);
  assert(denom);
  assert(ha_int_sign(denom));
  struct ha_frac *result = malloc(sizeof(struct ha_frac));
  int n_sign = ha_int_sign(nume);
  int d_sign = ha_int_sign(denom);
  if (n_sign == 0) {
    result->nega = false;
    result->nume = ha_int_create("0");
    result->denom = ha_int_create("1");
  } else {
    if ((n_sign > 0 && d_sign > 0) || (n_sign < 0 && d_sign < 0)){
      result->nega = false;
    } else {
      result->nega = true;
    }
    struct ha_int *n_cpy = posi_copy(nume);
    struct ha_int *d_cpy = posi_copy(denom);
    int cmp = ha_int_cmp(n_cpy, d_cpy);
    if (cmp == 0) {
      result->nume = ha_int_create("1");
      result->denom = ha_int_create("1");
    } else {
      struct ha_int *gcd = NULL;
      if (cmp == 1) {
        gcd = get_gcd(d_cpy, n_cpy);
      } else {
        gcd = get_gcd(n_cpy, d_cpy);
      }
      result->nume = ha_int_quotient(n_cpy, gcd);
      result->denom = ha_int_quotient(d_cpy, gcd);
      ha_int_destroy(gcd);
    }
    ha_int_destroy(n_cpy);
    ha_int_destroy(d_cpy);
  }
  return result;
}

struct ha_frac *ha_frac_create(const char *numerator, const char *denominator) {
  struct ha_int *nume = ha_int_create(numerator);
  struct ha_int *denom = ha_int_create(denominator);
  if (nume == NULL || denom == NULL || ha_int_sign(denom) == 0) {
    printf("ERROR: %s/%s is an invalid fraction\n", numerator, denominator);
    ha_int_destroy(nume);
    ha_int_destroy(denom);
    return NULL;
  } else {
    struct ha_frac *result = ha_frac_reduc(nume, denom);
    ha_int_destroy(nume);
    ha_int_destroy(denom);
    return result;
  }
}

void ha_frac_destroy(struct ha_frac *num) {
  assert(num);
  ha_int_destroy(num->nume);
  ha_int_destroy(num->denom);
  free(num);
}

void ha_frac_print(const struct ha_frac *num, bool newline) {
  assert(num);
  if (num->nega) {
    printf("-");
  }
  ha_int_print(num->nume, false);
  struct ha_int *one = ha_int_create("1");
  if (ha_int_cmp(one, num->denom)) {
    printf("/");
    ha_int_print(num->denom, false);
  }
  ha_int_destroy(one);
  if (newline) {
    printf("\n");
  }
}

struct ha_frac *ha_frac_copy(const struct ha_frac *num) {
  assert(num);
  char *nume = ha_int_to_str(num->nume);
  char *denom = ha_int_to_str(num->denom);
  struct ha_frac *result = ha_frac_create(nume, denom);
  free(nume);
  free(denom);
  result->nega = num->nega;
  return result;
}

struct ha_frac *ha_frac_add(const struct ha_frac *n, const struct ha_frac *m) {
  assert(n);
  assert(m);
  struct ha_int *new_n_nume = NULL;
  struct ha_int *new_m_nume = NULL;
  struct ha_int *new_denom = NULL;
  int cmp = ha_int_cmp(n->denom, m->denom);
  if (!cmp) {
    new_n_nume = ha_int_copy(n->nume);
    new_m_nume = ha_int_copy(m->nume);
    new_denom = ha_int_copy(n->denom);
  } else {
    struct ha_int *gcd = NULL;
    if (cmp > 0) {
      gcd = get_gcd(m->denom, n->denom);
    } else {
      gcd = get_gcd(n->denom, m->denom);
    }
    struct ha_int *n_mult = ha_int_quotient(m->denom, gcd);
    struct ha_int *m_mult = ha_int_quotient(n->denom, gcd);
    ha_int_destroy(gcd);
    new_n_nume = ha_int_mult(n_mult, n->nume);
    new_m_nume = ha_int_mult(m_mult, m->nume);
    new_denom = ha_int_mult(n_mult, n->denom);
    ha_int_destroy(n_mult);
    ha_int_destroy(m_mult);
  }
  struct ha_int *new_nume = NULL;
  if ((n->nega && m->nega) || (!n->nega && !m->nega)) {
    new_nume = ha_int_add(new_n_nume, new_m_nume);
  } else if (n->nega) {
    new_nume = ha_int_sub(new_m_nume, new_n_nume);
  } else {
    new_nume = ha_int_sub(new_n_nume, new_m_nume);
  }
  ha_int_destroy(new_n_nume);
  ha_int_destroy(new_m_nume);
  char *new_nume_str = ha_int_to_str(new_nume);
  char *new_denom_str = ha_int_to_str(new_denom);
  struct ha_frac *result = ha_frac_create(new_nume_str, new_denom_str);
  free(new_nume_str);
  free(new_denom_str);
  if (n->nega && m->nega) {
    result->nega = true;
  }
  ha_int_destroy(new_nume);
  ha_int_destroy(new_denom);
  return result;
}

struct ha_frac *ha_frac_sub(const struct ha_frac *n, const struct ha_frac *m) {
  assert(n);
  assert(m);
  struct ha_frac *m_cpy = malloc(sizeof(struct ha_frac));
  m_cpy->nume = ha_int_copy(m->nume);
  m_cpy->denom = ha_int_copy(m->denom);
  if (!m->nega) {
    m_cpy->nega = true;
  } else {
    m_cpy->nega = false;
  }
  struct ha_frac *result = ha_frac_add(n, m_cpy);
  ha_frac_destroy(m_cpy);
  return result; 
}

struct ha_frac *ha_frac_mult(const struct ha_frac *n, const struct ha_frac *m) {
  assert(n);
  assert(m);
  struct ha_int *new_nume = ha_int_mult(n->nume, m->nume);
  struct ha_int *new_denom = ha_int_mult(n->denom, m->denom);
  char *new_nume_str = ha_int_to_str(new_nume);
  char *new_denom_str = ha_int_to_str(new_denom);
  struct ha_frac *result = ha_frac_create(new_nume_str, new_denom_str);
  ha_int_destroy(new_nume);
  ha_int_destroy(new_denom);
  free(new_nume_str);
  free(new_denom_str);
  if (n->nega == m->nega || ha_int_sign(n->nume) == 0 
      || ha_int_sign(m->nume) == 0) {
    result->nega = false;
  } else {
    result->nega = true;
  }
  return result;
}

struct ha_frac *ha_frac_div(const struct ha_frac *n, const struct ha_frac *m) {
  assert(n);
  assert(m);
  assert(ha_int_sign(m->nume) != 0);
  struct ha_frac *m_reci = malloc(sizeof(struct ha_frac)); 
  m_reci->nume = ha_int_copy(m->denom);
  m_reci->denom = ha_int_copy(m->nume);
  m_reci->nega = m->nega;
  struct ha_frac *result = ha_frac_mult(n, m_reci);
  ha_frac_destroy(m_reci);
  return result;
}

int ha_frac_cmp(const struct ha_frac *n, const struct ha_frac *m) {
  assert(n);
  assert(m);
  struct ha_frac *n_sub_m = ha_frac_sub(n, m);
  int sign = 1;
  if (n_sub_m->nega) {
    sign = -1;
  } else if (!ha_int_sign(n_sub_m->nume)) {
    sign = 0;
  }
  ha_frac_destroy(n_sub_m);
  return sign;
}

bool ha_frac_is_frac(const struct ha_frac *num) {
  struct ha_int *one = ha_int_create("1");
  bool is_frac = !ha_int_eq(num->denom, one);
  ha_int_destroy(one);
  return is_frac;
}
