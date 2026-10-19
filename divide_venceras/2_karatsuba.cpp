#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

typedef vector<int> Big;   // digitos 0-9, little-endian (menos significativo primero)

Big normalizar(Big a) {
    while (a.size() > 1 && a.back() == 0) a.pop_back();
    return a;
}

Big suma(const Big& a, const Big& b) {
    Big r(max(a.size(), b.size()) + 1, 0);
    int carry = 0;
    for (size_t i = 0; i < r.size(); i++) {
        int x = carry;
        if (i < a.size()) x += a[i];
        if (i < b.size()) x += b[i];
        r[i] = x % 10;
        carry = x / 10;
    }
    return normalizar(r);
}

Big resta(const Big& a, const Big& b) {   // asume a >= b
    Big r(a.size(), 0);
    int borrow = 0;
    for (size_t i = 0; i < a.size(); i++) {
        int x = a[i] - borrow - (i < b.size() ? b[i] : 0);
        if (x < 0) { x += 10; borrow = 1; } else borrow = 0;
        r[i] = x;
    }
    return normalizar(r);
}

Big desplazar(const Big& a, int k) {      // a * 10^k
    if (a.size() == 1 && a[0] == 0) return a;
    Big r(a.size() + k, 0);
    for (size_t i = 0; i < a.size(); i++) r[i + k] = a[i];
    return r;
}

Big multEscolar(const Big& a, const Big& b) {
    Big r(a.size() + b.size(), 0);
    for (size_t i = 0; i < a.size(); i++) {
        long long carry = 0;
        size_t j = 0;
        for (; j < b.size() || carry; j++) {
            long long cur = r[i + j] + carry;
            if (j < b.size()) cur += (long long)a[i] * b[j];
            r[i + j] = (int)(cur % 10);
            carry = cur / 10;
        }
    }
    return normalizar(r);
}

const size_t UMBRAL = 32;   // por debajo de esto, multiplicacion escolar

Big karatsuba(const Big& a, const Big& b) {
    if (a.size() <= UMBRAL || b.size() <= UMBRAL) return multEscolar(a, b);

    size_t n = max(a.size(), b.size());
    size_t m = n / 2;

    Big a0(a.begin(), a.begin() + min(m, a.size()));
    Big a1(a.begin() + min(m, a.size()), a.end());
    Big b0(b.begin(), b.begin() + min(m, b.size()));
    Big b1(b.begin() + min(m, b.size()), b.end());
    if (a0.empty()) a0.push_back(0);
    if (a1.empty()) a1.push_back(0);
    if (b0.empty()) b0.push_back(0);
    if (b1.empty()) b1.push_back(0);
    a0 = normalizar(a0); a1 = normalizar(a1);
    b0 = normalizar(b0); b1 = normalizar(b1);

    Big z0 = karatsuba(a0, b0);
    Big z2 = karatsuba(a1, b1);
    Big sa = suma(a0, a1);
    Big sb = suma(b0, b1);
    Big z1 = karatsuba(sa, sb);
    z1 = resta(z1, z0);
    z1 = resta(z1, z2);           // z1 = a0*b1 + a1*b0

    Big resultado = suma(z0, desplazar(z1, (int)m));
    resultado = suma(resultado, desplazar(z2, (int)(2 * m)));
    return resultado;
}

Big aBig(const string& s) {
    Big r(s.size());
    for (size_t i = 0; i < s.size(); i++) r[i] = s[s.size() - 1 - i] - '0';
    return normalizar(r);
}

string aString(const Big& a) {
    string s(a.size(), '0');
    for (size_t i = 0; i < a.size(); i++) s[a.size() - 1 - i] = char('0' + a[i]);
    return s;
}

void comparar(int n) {
    mt19937 rng(42);
    string x(n, '0'), y(n, '0');
    x[0] = char('1' + rng() % 9);
    y[0] = char('1' + rng() % 9);
    for (int i = 1; i < n; i++) { x[i] = char('0' + rng() % 10); y[i] = char('0' + rng() % 10); }

    Big a = aBig(x), b = aBig(y);

    auto t0 = chrono::steady_clock::now();
    Big r1 = multEscolar(a, b);
    auto t1 = chrono::steady_clock::now();
    Big r2 = karatsuba(a, b);
    auto t2 = chrono::steady_clock::now();

    double tEscolar = chrono::duration<double>(t1 - t0).count();
    double tKaratsuba = chrono::duration<double>(t2 - t1).count();
    cerr << "n=" << n
         << "  escolar=" << tEscolar << "s"
         << "  karatsuba=" << tKaratsuba << "s"
         << "  resultados_iguales=" << (r1 == r2 ? "si" : "no") << "\n";
}

int main(int argc, char** argv) {
    if (argc > 1 && string(argv[1]) == "bench") {
        comparar(1000);
        comparar(10000);
        return 0;
    }

    string x, y;
    if (!(cin >> x >> y)) return 0;
    Big a = aBig(x), b = aBig(y);
    Big prod = karatsuba(a, b);
    cout << aString(prod) << '\n';
    return 0;
}
