#include <iostream>
#include <cmath>

// Function to calculate modular exponentiation (a^b mod m)
long long mod_exp(long long base, long long exponent, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        exponent = exponent >> 1; // Equivalent to exponent /= 2
        base = (base * base) % mod;
    }
    return result;
}

// Function to calculate multiplicative inverse
long long multiplicative_inverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
 
    if (m == 1)
        return 0;
 
    // Apply extended Euclid Algorithm
    while (a > 1) {
        // q is quotient
        q = a / m;
 
        t = m;
 
        // m is remainder now, process same as Euclid's algo
        m = a % m, a = t;
 
        t = x0;
 
        x0 = x1 - q * x0;
 
        x1 = t;
    }
 
    // Make x1 positive
    if (x1 < 0)
        x1 += m0;
 
    return x1;
}

int main() {
    // El-Gamal parameters
    long long g = 2; // generator
    long long p = 13; // large prime
    long long k = 7; // sender's secret key
    long long A = 8; // receiver's public key
    long long d = 3; // receiver's private key

    // Sender's public key
    long long B = mod_exp(g, k, p);

    // Plaintext
    long long M = 4;

    // Encryption
    long long Y1 = mod_exp(g, k, p);
    long long Y2 = (M * mod_exp(A, k, p)) % p;

    std::cout << "Ciphertext Y1: " << Y1 << std::endl;
    std::cout << "Ciphertext Y2: " << Y2 << std::endl;

    // Decryption
    long long decrypted_M = (Y2 * multiplicative_inverse(mod_exp(Y1, d, p), p)) % p;

    std::cout << "Decrypted plaintext M: " << decrypted_M << std::endl;

    return 0;
}
