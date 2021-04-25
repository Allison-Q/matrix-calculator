// This is a structure for high-accuracy numbers

// NOTE: digits in each part are stored in a reverse order for convenience

struct ha_num {
  bool negative; // true for negative, false for positive and 0
  char *real_nume; // numerator digits of real part
  char *real_deno; // denominator digits of real part
  char *imag_nume; // numerator digits of imaginary part
  char *imag_deno; // denominator digits of imaginary part
};

