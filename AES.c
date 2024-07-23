#include <stdint.h>
#include <stdio.h>
#define AES_256 14

static const uint8_t SboxEntries[256] = {
 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 
 0xfe, 0xd7, 0xab, 0x76,
 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 
 0x9c, 0xa4, 0x72, 0xc0,
 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 
 0x71, 0xd8, 0x31, 0x15,
 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 
 0xeb, 0x27, 0xb2, 0x75,
 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 
 0x29, 0xe3, 0x2f, 0x84,
 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 
 0x4a, 0x4c, 0x58, 0xcf,
 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 
 0x50, 0x3c, 0x9f, 0xa8,
 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 
 0x10, 0xff, 0xf3, 0xd2,
 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 
 0x64, 0x5d, 0x19, 0x73,
 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 
 0xde, 0x5e, 0x0b, 0xdb,
 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 
 0x91, 0x95, 0xe4, 0x79,
 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 
 0x65, 0x7a, 0xae, 0x08,
 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 
 0x4b, 0xbd, 0x8b, 0x8a,
 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 
 0x86, 0xc1, 0x1d, 0x9e,
 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 
 0xce, 0x55, 0x28, 0xdf,
 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 
 0xb0, 0x54, 0xbb, 0x16
};

static const uint8_t RconEntries[10] = {
  0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36
};

// Galois Field multiplication function
uint8_t Multiply(uint8_t a, uint8_t b) {
  uint8_t result = 0;
  uint8_t high_bit_set;
  for (int i = 0; i < 8; ++i) {
    if ((b & 1) == 1) {
      result ^= a;
    }
    high_bit_set = a & 0x80;
    a <<= 1;
    if (high_bit_set == 0x80) {
      a ^= 0x1b; // AES irreducible polynomial: x^8 + x^4 + x^3 + x + 1
    }
    b >>= 1;
  }
  return result;
}

void KeyExpansion(const uint8_t * key, uint8_t * roundKeys) {
  const int keySize = 32; // Key size in bytes for AES-256
  const int roundKeySize = 240; // Round key size in bytes for AES-256
  // Copy the original key to the first 32 bytes of roundKeys
  for (int i = 0; i < keySize; ++i) {
    roundKeys[i] = key[i];
  }
  // Variables
  int bytesGenerated = keySize; // Bytes generated so far
  int rconIteration = 1; // RconEntries iteration starts from 1
  while (bytesGenerated < roundKeySize) {
    // Copy the previous 4 bytes to the temporary array
    uint8_t temp[4];
    for (int i = 0; i < 4; ++i) {
      temp[i] = roundKeys[bytesGenerated - 4 + i];
    }
    // Perform key schedule core
    if (bytesGenerated % keySize == 0) {
      // RotWord
      uint8_t tempByte = temp[0];
      temp[0] = temp[1];
      temp[1] = temp[2];
      temp[2] = temp[3];
      temp[3] = tempByte;
      // SubWord
      for (int i = 0; i < 4; ++i) {
        temp[i] = SboxEntries[temp[i]];
      }
      // XOR with RconEntries
      temp[0] ^= RconEntries[rconIteration++];
    }
    // XOR temp with the 4-byte block [bytesGenerated-keySize] and store in roundKeys
    for (int i = 0; i < 4; ++i) {
      roundKeys[bytesGenerated] = roundKeys[bytesGenerated - keySize] ^
        temp[i];
      bytesGenerated++;
    }
  }
}

void ShiftRows(uint8_t* state) {
// Shift row 1 by 1 position to the left
uint8_t temp = state[1];
state[1] = state[5];
state[5] = state[9];
state[9] = state[13];
state[13] = temp;
// Shift row 2 by 2 positions to the left
temp = state[2];
state[2] = state[10];
state[10] = temp;
temp = state[6];
state[6] = state[14];
state[14] = temp;
// Shift row 3 by 3 positions to the left
temp = state[3];
state[3] = state[15];
state[15] = state[11];
state[11] = state[7];
state[7] = temp;
}

void MixColumns(uint8_t* state) {
for (int i = 0; i < 4; ++i) {
uint8_t a = state[i];
uint8_t b = state[4 + i];
uint8_t c = state[8 + i];
uint8_t d = state[12 + i];
state[i] = Multiply(a, 0x02) ^ Multiply(b, 0x03) ^ c ^ d;
state[4 + i] = a ^ Multiply(b, 0x02) ^ Multiply(c, 0x03) ^ d;
state[8 + i] = a ^ b ^ Multiply(c, 0x02) ^ Multiply(d, 0x03);
state[12 + i] = Multiply(a, 0x03) ^ b ^ c ^ Multiply(d, 0x02);
}
}

void AddRoundKey(uint8_t * state,
  const uint8_t * roundKey) {
  for (int i = 0; i < 16; ++i) {
    state[i] ^= roundKey[i];
  }
}

void SubBytes(uint8_t * state) {
  for (int i = 0; i < 16; ++i) {
    state[i] = SboxEntries[state[i]];
  }
}

void AES256Encryption(const uint8_t * plaintext,
  const uint8_t * key, uint8_t *
    ciphertext) {
  uint8_t roundKeys[240]; // Round keys for AES-256
  // Key expansion
  KeyExpansion(key, roundKeys);
  // Copy the plaintext to the initial state
  for (int i = 0; i < 16; ++i) {
    ciphertext[i] = plaintext[i];
  }
  // Initial round key addition
  AddRoundKey(ciphertext, roundKeys);
  // Main rounds
  for (int round = 1; round < AES_256; ++round) {
    SubBytes(ciphertext);
    ShiftRows(ciphertext);
    MixColumns(ciphertext);
    AddRoundKey(ciphertext, roundKeys + round * 16);
  }
  // Final round (no MixColumns)
  SubBytes(ciphertext);
  ShiftRows(ciphertext);
  AddRoundKey(ciphertext, roundKeys + AES_256 * 16);
}

void AES256Decryption(const uint8_t * ciphertext, const uint8_t * key, uint8_t * decryptedText) {
  uint8_t roundKeys[240];
  KeyExpansion(key, roundKeys);
  for (int i = 0; i < 16; ++i) {
    decryptedText[i] = ciphertext[i];
  }
  AddRoundKey(decryptedText, roundKeys + AES_256 * 16);
  for (int round = AES_256 - 1; round > 0; --round) {
    ShiftRows(decryptedText);
    SubBytes(decryptedText);
    AddRoundKey(decryptedText, roundKeys + round * 16);
    MixColumns(decryptedText);
  }
  ShiftRows(decryptedText);
  SubBytes(decryptedText);
  AddRoundKey(decryptedText, roundKeys);
}

int main() {
  // Example usage
  uint8_t plaintext[16] = {
    0x00,0x11,0x22,0x33,0x44,
    0x55,0x66,0x77,0x88,0x99,
    0xaa,0xbb,0xcc,0xdd,0xee,0xff
  };
  uint8_t key[32] = {
    0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
    0xab,0xf7,0x97,0x75,0x46,0x20,0x63,0xed,
    0xc8,0x37,0x3c,0x35,0x40,0x2d,0x22,0x75,
    0x2b,0x06,0x62,0xe2,0xea,0x0d,0x33,0x3c
  };
  uint8_t ciphertext[16];
  uint8_t decryptedText[16];
  // Encrypt
  AES256Encryption(plaintext, key, ciphertext);
  // Print the result
  printf("Plaintext: ");
  for (int i = 0; i < 16; ++i) {
    printf("%02x ", plaintext[i]);
  }
  printf("\n");
  printf("Key: ");
  for (int i = 0; i < 32; ++i) {
    printf("%02x ", key[i]);
  }
  printf("\n");
  printf("Ciphertext: ");
  for (int i = 0; i < 16; ++i) {
    printf("%02x ", ciphertext[i]);
  }
  printf("\n");

  // Decrypt
  AES256Decryption(ciphertext, key, decryptedText);
  // Print the decrypted result
  printf("Decrypted Text: ");
  for (int i = 0; i < 16; ++i) {
    printf("%02x ", plaintext[i]);
  }
  printf("\n");
  return 0;
}


