#include <iostream>
#include <string>
#include <cstring>
#include <cstdint>

// Constants for MD5 algorithm
const uint32_t S[] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

const uint32_t K[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// Function declarations
std::string md5(const std::string& message);
std::string md5_hash(const uint8_t* initial_msg, size_t initial_len);

// Main function
int main() {
    std::string message;
    std::cout << "Enter a message: ";
    std::getline(std::cin, message);

    std::string hash = md5(message);
    std::cout << "MD5 Hash: " << hash << std::endl;

    return 0;
}

// Helper functions
std::string md5(const std::string& message) {
    return md5_hash(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
}

std::string md5_hash(const uint8_t* initial_msg, size_t initial_len) {
    // Initialize variables
    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;

    // Pre-processing
    uint64_t bits_len = initial_len * 8;
    // Padding
    size_t new_len = initial_len + 1;
    while (new_len % 64 != 56)
        new_len++;
    uint8_t* msg = new uint8_t[new_len + 8];
    std::memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 0x80; // Padding with a single bit followed by zeros
    // Append length
    for (int i = 0; i < 8; ++i)
        msg[new_len + i] = (bits_len >> (i * 8)) & 0xFF;

    // Process each 512-bit block
    for (size_t offset = 0; offset < new_len; offset += 64) {
        uint32_t a = a0;
        uint32_t b = b0;
        uint32_t c = c0;
        uint32_t d = d0;

        uint32_t* chunk = reinterpret_cast<uint32_t*>(msg + offset);

        // Main loop
        for (int i = 0; i < 64; ++i) {
            uint32_t f, g;

            if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            } else {
                f = c ^ (b | (~d));
                g = (7 * i) % 16;
            }

            f = f + a + K[i] + chunk[g];
            a = d;
            d = c;
            c = b;
            b = b + ((f << S[i]) | (f >> (32 - S[i])));
        }

        // Update variables
        a0 += a;
        b0 += b;
        c0 += c;
        d0 += d;
    }

    // Combine hash parts
    uint8_t hash_bytes[16];
    memcpy(hash_bytes, &a0, 4);
    memcpy(hash_bytes + 4, &b0, 4);
    memcpy(hash_bytes + 8, &c0, 4);
    memcpy(hash_bytes + 12, &d0, 4);

    // Convert hash to hexadecimal string
    std::string result;
    for (uint8_t byte : hash_bytes) {
        char buf[3];
        sprintf(buf, "%02x", byte);
        result += buf;
    }

    delete[] msg;

    return result;
}
