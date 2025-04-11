#include <iostream>
#include <cmath>
#include <tuple>

struct DoubleDouble {
    double hi;
    double lo;

    DoubleDouble(double h, double l = 0.0) : hi(h), lo(l) {}

    DoubleDouble operator-() const { return DoubleDouble(-hi, -lo); }
};

const DoubleDouble pi(3.1415926535897931, 1.2246467991473532e-16);
const DoubleDouble pi_2(1.5707963267948966, 6.123233995736766e-17);
const DoubleDouble two_over_pi(0.63661977236758134, -3.9357353350364975e-17);

std::pair<double, double> two_sum(double a, double b) {
    double s = a + b;
    double v = s - a;
    double e = (a - (s - v)) + (b - v);
    return {s, e};
}

std::pair<double, double> two_mul(double a, double b) {
    double p = a * b;
    double e = std::fma(a, b, -p);
    return {p, e};
}

DoubleDouble operator+(const DoubleDouble& a, const DoubleDouble& b) {
    auto [s1, e1] = two_sum(a.hi, b.hi);
    auto [s2, e2] = two_sum(a.lo, b.lo);
    e1 += s2;
    auto [s3, e3] = two_sum(s1, e1);
    e3 += e2;
    return {s3, e3};
}

DoubleDouble operator-(const DoubleDouble& a, const DoubleDouble& b) {
    return a + (-b);
}

DoubleDouble operator*(const DoubleDouble& a, const DoubleDouble& b) {
    auto [p1, p2] = two_mul(a.hi, b.hi);
    p2 += a.hi * b.lo;
    p2 += a.lo * b.hi;
    auto [s, e] = two_sum(p1, p2);
    return {s, e};
}

DoubleDouble operator/(const DoubleDouble& a, double b) {
    double hi = a.hi / b;
    double lo = a.lo / b;
    auto [s, e] = two_sum(hi, lo);
    return {s, e};
}

std::pair<int, DoubleDouble> exact_reduce(const DoubleDouble& x) {
    DoubleDouble y = x * two_over_pi;

    double n_d = std::round(y.hi);
    int n = static_cast<int>(n_d);
    
    DoubleDouble n_times_pi_2 = DoubleDouble(n_d) * pi_2;
    DoubleDouble r = x - n_times_pi_2;
    
    return {n, r};
}

DoubleDouble sin_taylor(const DoubleDouble& x) {
    DoubleDouble sum = x;
    DoubleDouble term = x;
    DoubleDouble x_sq = x * x;

    for (int i = 1; i < 20; ++i) {
        term = (term * x_sq) / ((2 * i) * (2 * i + 1));
        sum = (i % 2 == 1) ? sum - term : sum + term;
    }
    return sum;
}

DoubleDouble sin_dd(const DoubleDouble& x) {
    auto [n, r] = exact_reduce(x);
    
    DoubleDouble sin_r = sin_taylor(r);
    DoubleDouble cos_r = sin_taylor(pi_2 - r);

    switch (((n % 4) + 4) % 4) {
        case 0: return sin_r;   
        case 1: return cos_r;     
        case 2: return -sin_r;    
        case 3: return -cos_r;   
        default: return DoubleDouble(0.0);
    }
}

int main() {
    DoubleDouble x = pi / 6.0;
    DoubleDouble result = sin_dd(x);
    std::cout.precision(25);
    std::cout << "sin(pi/6) ≈ " << result.hi + result.lo << std::endl;
    

    DoubleDouble big_x = 10.0 * pi + pi / 6.0;
    result = sin_dd(big_x);
    std::cout << "sin(10π + pi/6) ≈ " << result.hi + result.lo << std::endl;


    DoubleDouble x2 = 1;
    x2.hi = 0.0;
    x2.lo = 1e-20;
    result = sin_dd(x2);
    std::cout << "sin(1) ≈ " << result.hi + result.lo << std::endl;

    return 0;
}