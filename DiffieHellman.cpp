#include <iostream>
#include <cmath>

// Function to calculate power in modular arithmetic
long long int power(long long int a, long long int b, long long int P) {
    if (b == 1)
        return a;
    else
        return (((long long int)pow(a, b)) % P);
}

// Function to perform the Diffie-Hellman key exchange
long long int computeKey(long long int G, long long int a, long long int P) {
    return power(G, a, P);
}

int main() {
    // Commonly agreed prime modulus and generator
    long long int P = 23; // Prime number
    long long int G = 5;  // Generator

    long long int a = 6; 
    long long int b = 15; 

    long long int A = computeKey(G, a, P);
    long long int B = computeKey(G, b, P);

    long long int keyA = computeKey(B, a, P);
    long long int keyB = computeKey(A, b, P);
    std::cout << "The value of P: " << P << std::endl;
    std::cout << "The value of G: " << G << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "The private key a for Aadarsh: " << a << std::endl;
    std::cout << "The private key b for Saurav: " << b << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "Session key generated by Aadarsh: " << keyA << std::endl;
    std::cout << "Session key generated by Saurav: " << keyB << std::endl;

    return 0;
}

