// This module turns the input into the corresponding ha_comp

// We assume the input is in the proper form
// examples of proper form: 
//   0, 12, -123 (integers)
//   1/2, -2/3, 4/2 (fractions)
//   i, -3i, (2/3)i (only imaginary parts)
//   2+3i, -1/2-(3/4)i, -3+(4/5)i, 2/3-4i (complex numbers)

// The following applies to all functions:
// requires: all number parameters are valid (not NULL)
// time: (n) is the length of the number

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "high-accuracy-complex.h"
#include "read-input.h"


// only_real(num) determines if num has only the real part
// time: O(n)
static bool only_real(const char *num) {
  assert(num);
  const int len = strlen(num);
  for (int i = 0; i < len; ++i) {
    if (num[i] == 'i') {
      return false;	
    }
  }
  return true;
}

// only_imaginary(num) determines if num has only the imaginary part
// time: O(n)
static bool only_imaginary(const char *num) {
  assert(num);
  if (only_real(num)) {
    return false;
  }

  // num contains 'i'
  const int len = strlen(num);
  int i_idx = 0; //index of 'i'
  for (int i = 0; i < len; ++i) {
    if (num[i] == 'i') {
      i_idx = i;
      break;
    }
  }
  for (int j = i_idx - 1; j >= 0; --j) {
    if (num[j] == '+') {
      return false;
    } else if (num[j] == '-') {
      return j == 0;
    }
  }
  return true;
}

// real(num) gives the real part of num
// effects: allocates memory (caller must free)
// time: O(n)
static char *real(char *num) {
  assert(num);
  const int len = strlen(num);
  char *copy = malloc((len + 1) * sizeof(char));
  strcpy(copy, num);

  if (only_real(copy)) {
    return copy;
  } else if (only_imaginary(copy)) {
    copy = realloc(copy, sizeof(char));
    copy[0] = '\0';
    return copy;
  }

  // num has both real and imaginary part
  for (int i = 1; i < len; ++i) {
    if (copy[i] == '+' || copy[i] == '-') {
      copy[i] = '\0';
    }
  }
  return copy;
}

// imaginary(num) gives the imaginary part of num
// effects: allocates memory (caller must free)
// time: O(n)
static char *imaginary(char *num) {
  assert(num);
  const int len = strlen(num);

  if (only_imaginary(num)) {
    char *copy = malloc((len + 1) * sizeof(char));
    strcpy(copy, num);
    return copy;
  } else if (only_real(num)) {
    char *copy = malloc(sizeof(char));
    copy[0] = '\0';
    return copy;
  }

  // num has both real and imaginary part
  char *curpos = num + 1; // current position
  int imaginary_len = len - 1;
  while (*curpos != '\0') {
    ++curpos;
    --imaginary_len;
    if (*(curpos - 1) == '+' || *(curpos - 1) == '-') {
      if (*(curpos - 1) == '-') {
        --curpos;
        ++imaginary_len;
      }
      break;
    }
  }
  char *copy = malloc((imaginary_len + 1) * sizeof(char));
  strcpy(copy, curpos);
  return copy;
}

// coefficient(num) gives the coefficient of num
// notes: coefficient of real part is itself
//        coefficient of imaginary part is the fraction (exclude '()' and 'i')
// effects: allocates memory (caller must free)
// time: O(n)
static char *coefficient(const char *num) {
  assert(num);
  const int len = strlen(num);
  char *coe = NULL;

  if (len == 1 && num[0] == 'i') { // special case: num is i
    coe = malloc(2 * sizeof(char));
    coe[0] = '1';
    coe[1] = '\0';
  } else if (len == 2 && num[0] == '-' && num[1] == 'i') { // num is -i
    coe = malloc(3 * sizeof(char));
    coe[0] = '-';
    coe[1] = '1';
    coe[2] = '\0';
  } else { // other cases
    coe = malloc((len + 1) * sizeof(char));
    int coe_idx = 0;
    for (int i = 0; i < len; ++i) {
      if (num[i] == '(' || num[i] == ')' || num[i] == 'i') {
        continue;
      } else {
        coe[coe_idx] = num[i];
        ++coe_idx;
      }
    }
    coe[coe_idx] = '\0';
    int coe_len = strlen(coe);
    coe = realloc(coe, (coe_len + 1) * sizeof(char));
  }
  return coe;
}

// numerator(num) gives the numerator of num
// requires: num only has the real part or imaginary part [not asserted]
// effects: allocates memory (caller must free)
// time: O(n)
static char *numerator(const char *num) {
  assert(num);
  char *coe = coefficient(num);
  const int len = strlen(coe);
  char *nume = malloc((len + 1) * sizeof(char));
  strcpy(nume, coe);

  for (int i = 0; i < len; ++i) {
    if (nume[i] == '/') {
      nume[i] = '\0';
    }
  }
  int nume_len = strlen(nume);
  if (nume_len == 0) { // empty string
    nume = realloc(nume, 2 * sizeof(char));
    nume[0] = '0';
    nume[1] = '\0';
  } else {
    nume = realloc(nume, (nume_len + 1) * sizeof(char));
  }
  free(coe);
  return nume;
}

// denominator(num) gives the denominator of num
// requires: num only has the real part or imaginary part [not asserted]
// effects: allocates memory (caller must free)
// time: O(n)
static char *denominator(const char *num) {
  assert(num);
  char *coe = coefficient(num);
  const int len = strlen(coe);
  char *curpos = coe;
  int denom_len = len;

  while (*curpos != '\0') {
    ++curpos;
    --denom_len;
    if (*(curpos - 1) == '/') {
      break;
    }
  }
  char *denom = NULL;
  if (denom_len == 0) { // empty string
    denom = malloc(2 * sizeof(char));
    denom[0] = '1';
    denom[1] = '\0';
  } else {
    denom = malloc((denom_len + 1) * sizeof(char));
    strcpy(denom, curpos);
  }
  free(coe);
  return denom;
}

struct ha_comp *read_input(char *num) {
  assert(num);
  // change to ha_comp
  char *real_part = real(num);
  char *imaginary_part = imaginary(num);
  char *real_nume = numerator(real_part);
  char *real_denom = denominator(real_part);
  char *imaginary_nume = numerator(imaginary_part);
  char *imaginary_denom = denominator(imaginary_part);
  struct ha_comp *ha_num = 
    ha_comp_create(real_nume, real_denom, imaginary_nume, imaginary_denom);
  free(real_part);
  free(imaginary_part);
  free(real_nume);
  free(real_denom);
  free(imaginary_nume);
  free(imaginary_denom);
  return ha_num;
}
