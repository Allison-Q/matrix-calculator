// This module provides basic calculations for arbitrarily large complex numbers

// For all program scope functions, see high-accuracy-complex.h for details

// The following applies to all functions:
// requires: all number parameters are valid (not NULL)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "high-accuracy-complex.h"
#include "high-accuracy-fraction.h"

struct ha_comp {
  struct ha_frac *real;
  struct ha_frac *ima;
};

// ha_comp(ha_comp_create_with_ha_frac) returns a pointer p at a struct ha_comp
// with p->real = real, p->ima = ima
// effects: allocates memory(caller must call ha_comp_destroy)
// time: O(1)
static struct ha_comp *ha_comp_create_with_ha_frac(struct ha_frac *real, 
                                                   struct ha_frac *ima) {
  assert(real);
  assert(ima);
  struct ha_comp *result = malloc(sizeof(struct ha_comp));
  result->real = real;
  result->ima = ima;
  return result;
}

struct ha_comp *ha_comp_create(const char *real_nume, const char *real_denom,
                               const char *ima_nume, const char *ima_denom) {
  assert(real_nume);
  assert(real_denom);
  assert(ima_nume);
  assert(ima_denom);
  struct ha_frac *real = ha_frac_create(real_nume, real_denom);
  struct ha_frac *ima = ha_frac_create(ima_nume, ima_denom);
  if (real && ima) {
    return ha_comp_create_with_ha_frac(real, ima);
  } else {
    if (real) {
      ha_frac_destroy(real);
    }
    if (ima) {
      ha_frac_destroy(ima);
    }
    return NULL;
  }
}

void ha_comp_destroy(struct ha_comp *num) {
  assert(num);
  ha_frac_destroy(num->real);
  ha_frac_destroy(num->ima);
  free(num);
}

// ha_frac_cmp_with_n(num, n) returns 1 if num > n, 0 if num == n, -1 if num < n
// time: O((n2) * log(n2))
static int ha_frac_cmp_with_n(const struct ha_frac *num, const char *n) {
  assert(num);
  assert(n);
  struct ha_frac *temp = ha_frac_create(n, "1");
  int sign = ha_frac_cmp(num, temp);
  ha_frac_destroy(temp);
  return sign;
}

// ha_frac_sign(num) returns 1 if num is positive, 0 if num == n, -1 if num is 
// negative
// time: O((n2) * log(n2))
static int ha_frac_sign(const struct ha_frac *num) {
  assert(num);
  return ha_frac_cmp_with_n(num, "0");
}

void ha_comp_print(const struct ha_comp *num, bool newline) {
  assert(num);
  int real_sign = ha_frac_sign(num->real);
  if (real_sign) {
    ha_frac_print(num->real, false);
  }
  int ima_sign = ha_frac_sign(num->ima);
  if (ima_sign) {
    if (ima_sign == 1 && real_sign) {
      printf("+");
    } else if (ima_sign == -1) {
      printf("-");
    }
    bool is_frac = ha_frac_is_frac(num->ima);
    if (is_frac) {
      printf("(");
    }
    if (ha_frac_cmp_with_n(num->ima, "1") && 
        ha_frac_cmp_with_n(num->ima, "-1")) {
      if (ima_sign == 1) {
        ha_frac_print(num->ima, false);
      } else {
        struct ha_frac *zero = ha_frac_create("0", "1");
        struct ha_frac *posi_ima = ha_frac_sub(zero, num->ima);
        ha_frac_print(posi_ima, false);
        ha_frac_destroy(zero);
        ha_frac_destroy(posi_ima);
      }
    }
    if (is_frac) {
      printf(")");
    }
    printf("i");
  }
  if (!ima_sign && !real_sign) {
    printf("0");
  }
  if (newline) {
    printf("\n");
  }
}

bool ha_comp_is_zero(const struct ha_comp *num) {
  assert(num);
  if (!ha_frac_sign(num->real) && !ha_frac_sign(num->ima)) {
    return true;
  } else {
    return false;
  }
}

bool ha_comp_is_one(const struct ha_comp *num) {
  assert(num);
  if (ha_frac_sign(num->ima)) {
    return false;
  } else {
    if (!ha_frac_cmp_with_n(num->real, "1")) {
      return true;
    } else {
      return false;
    }
  }
}

struct ha_comp *ha_comp_add(const struct ha_comp *n, const struct ha_comp *m) {
  assert(n);
  assert(m);
  struct ha_frac *new_real = ha_frac_add(n->real, m->real);
  struct ha_frac *new_ima = ha_frac_add(n->ima, m->ima);
  return ha_comp_create_with_ha_frac(new_real, new_ima);
}

struct ha_comp *ha_comp_sub(const struct ha_comp *n, const struct ha_comp *m) {
  assert(n);
  assert(m);
  struct ha_frac *new_real = ha_frac_sub(n->real, m->real);
  struct ha_frac *new_ima = ha_frac_sub(n->ima, m->ima);
  return ha_comp_create_with_ha_frac(new_real, new_ima);
}

struct ha_comp *ha_comp_mult(const struct ha_comp *n, const struct ha_comp *m) {
  assert(n);
  assert(m);
  struct ha_frac *real_real = ha_frac_mult(n->real, m->real);
  struct ha_frac *ima_ima = ha_frac_mult(n->ima, m->ima);
  struct ha_frac *new_real = ha_frac_sub(real_real, ima_ima);
  ha_frac_destroy(real_real);
  ha_frac_destroy(ima_ima);
  struct ha_frac *real_ima = ha_frac_mult(n->real, m->ima);
  struct ha_frac *ima_real = ha_frac_mult(n->ima, m->real);
  struct ha_frac *new_ima = ha_frac_add(real_ima, ima_real);
  ha_frac_destroy(real_ima);
  ha_frac_destroy(ima_real);
  return ha_comp_create_with_ha_frac(new_real, new_ima);
}

struct ha_comp *ha_comp_div(const struct ha_comp *n, const struct ha_comp *m) {
  assert(n);
  assert(m);
  assert(!ha_comp_is_zero(m));
  struct ha_frac *zero = ha_frac_create("0", "1");
  struct ha_frac *nega_m_ima = ha_frac_sub(zero, m->ima);
  ha_frac_destroy(zero);
  struct ha_comp *m_conju = 
    ha_comp_create_with_ha_frac(ha_frac_copy(m->real), nega_m_ima);
  struct ha_comp *m_mult_m_conju = ha_comp_mult(m, m_conju);
  struct ha_frac *div = m_mult_m_conju->real;
  struct ha_comp *n_mult_m_conju = ha_comp_mult(n, m_conju);
  ha_comp_destroy(m_conju);
  struct ha_frac *new_real = ha_frac_div(n_mult_m_conju->real, div);
  struct ha_frac *new_ima = ha_frac_div(n_mult_m_conju->ima, div);
  ha_comp_destroy(m_mult_m_conju);
  ha_comp_destroy(n_mult_m_conju);
  return ha_comp_create_with_ha_frac(new_real, new_ima);
}