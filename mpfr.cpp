#include <iostream>
#include <mpfr.h>

void calculate_sin(const char* desc, mpfr_t angle) {
    mpfr_t result;
    mpfr_init2(result, 133);

    mpfr_sin(result, angle, MPFR_RNDN);

    mpfr_printf("%-10s = %.40Rf\n", desc, result);
    mpfr_clear(result);
}

int main() {
    mpfr_set_default_prec(133);

    mpfr_t pi, angle;
    mpfr_inits(pi, angle, nullptr);
    mpfr_const_pi(pi, MPFR_RNDN);

    // 1. sin(0)
    mpfr_set_d(angle, 0.0, MPFR_RNDN);
    calculate_sin("sin(0)", angle);

    // 2. sin(1)
    mpfr_set_d(angle, 1.0, MPFR_RNDN);
    calculate_sin("sin(1)", angle);

    // 3. sin(π/2)
    mpfr_div_ui(angle, pi, 2, MPFR_RNDN);
    calculate_sin("sin(π/2)", angle);

    // 4. sin(π)
    mpfr_set(angle, pi, MPFR_RNDN);
    calculate_sin("sin(π)", angle);

    // 5. sin(π/6)
    mpfr_div_ui(angle, pi, 6, MPFR_RNDN);
    calculate_sin("sin(π/6)", angle);

    // 6. sin(3π/2)
    mpfr_mul_ui(angle, pi, 3, MPFR_RNDN);
    mpfr_div_ui(angle, angle, 2, MPFR_RNDN);
    calculate_sin("sin(3π/2)", angle);

    mpfr_clears(pi, angle, nullptr);
    mpfr_free_cache();

    return 0;
}