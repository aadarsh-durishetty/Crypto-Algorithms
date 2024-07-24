************************additive/ caesar cipher***********************1
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string encrypt(std::string plaintext, int shift) {
    for (int i = 0; i < plaintext.length(); i++) {
        plaintext[i] = char((plaintext[i] + shift - 'a') % 26 + 'a');
    }
    return plaintext;
}
string decrypt(std::string ciphertext, int shift) {
    for (int i = 0; i < ciphertext.length(); i++) {
        ciphertext[i] = char((ciphertext[i] - shift - 'a' + 26) % 26 + 'a');
    }
    return ciphertext;
}
int main() {
    string plaintext = "cryptography";
    int shift = 4;
    string ciphertext = encrypt(plaintext, shift);
    cout << "Plaintext: " << plaintext << std::endl;
    cout << "Shift: " << shift << std::endl;
    cout << "Ciphertext: " << ciphertext << std::endl;
    string decryptedtext = decrypt(ciphertext, shift);
    cout << "Decrypted text: " << decryptedtext << std::endl;
    return 0;
}

************************ affine cipher mul inverse*************************2
#include<bits/stdc++.h>
using namespace std;

const int a = 17;
const int b = 20;

string encryptMessage(string msg)
{
	string cipher = ""; 
	for (int i = 0; i < msg.length(); i++)
	{
		if(msg[i]!=' ') 
			cipher = cipher + (char) ((((a * (msg[i]-'A') ) + b) % 26) + 'A');
		else
			cipher += msg[i];	 
	}
	return cipher;
}
string decryptCipher(string cipher)
{
	string msg = "";
	int a_inv = 0;
	int flag = 0;
	for (int i = 0; i < 26; i++)
	{
		flag = (a * i) % 26;
		if (flag == 1)
		{ 
			a_inv = i;
		}
	}
	for (int i = 0; i < cipher.length(); i++)
	{
		if(cipher[i]!=' ')
			msg = msg + (char) (((a_inv * ((cipher[i]+'A' - b)) % 26)) + 'A');
		else
			msg += cipher[i]; 
	}
	return msg;
}
int main(void)
{
	string msg = "AFFINE CIPHER";
	string cipherText = encryptMessage(msg);
	cout << "Encrypted Message is : " << cipherText<<endl;
	cout << "Decrypted Message is: " << decryptCipher(cipherText);
	return 0;
}

**********************polyalphabetic autokey cipher*******************3
#include <iostream>
#include <string>
using namespace std;

string autoKeyEncrypt(string plaintext, string key) {
    string ciphertext;
    int j = 0;
    for (int i = 0; i < plaintext.length(); i++) {
        int x = (plaintext[i] + key[j]) % 26;
        x += 'A';
        ciphertext += x;
        key += plaintext[i];
        j++;
    }
    return ciphertext;
}
string autoKeyDecrypt(string ciphertext, string key) {
    string plaintext;
    int j = 0;
    for (int i = 0; i < ciphertext.length(); i++) {
        int x = ciphertext[i] - key[j] + 26;
        x %= 26;
        x += 'A';
        plaintext += x;
        key += plaintext[i];
        j++;
    }
    return plaintext;
}
int main() {
    string plaintext, key, ciphertext;
    cout << "Enter the plaintext: ";
    getline(cin, plaintext);
    cout << "Enter the key: ";
    getline(cin, key);
    ciphertext = autoKeyEncrypt(plaintext, key);
    cout << "Ciphertext: " << ciphertext << endl;
    plaintext = autoKeyDecrypt(ciphertext, key);
    cout << "Decrypted plaintext: " << plaintext << endl;
    return 0;
}

********************** playfair cipher*************************4
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

void generatePlayfairMatrix(const string &key, char playfairMatrix[5][5]) {
    string keyWithoutDuplicates;
    for (char ch : key) {
        if (isalpha(ch) && keyWithoutDuplicates.find(toupper(ch)) == string::npos) {
            keyWithoutDuplicates += toupper(ch);
        }
    }
    string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // Excluding 'J'
    size_t keyIndex = 0, alphaIndex = 0;
    for (; keyIndex < keyWithoutDuplicates.length(); ++keyIndex) {
        playfairMatrix[keyIndex / 5][keyIndex % 5] = keyWithoutDuplicates[keyIndex];
        alphabet.erase(remove(alphabet.begin(), alphabet.end(), keyWithoutDuplicates[keyIndex]), alphabet.end());
    }
    for (; alphaIndex < alphabet.length(); ++alphaIndex) {
        playfairMatrix[keyIndex / 5][keyIndex % 5] = alphabet[alphaIndex];
        ++keyIndex;
    }
}
void getCharPosition(char playfairMatrix[5][5], char ch, int &row, int &col) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (playfairMatrix[i][j] == ch) {
                row = i;
                col = j;
                return;
            }
        }
    }
}
string encryptPlayfair(const string &plaintext, char playfairMatrix[5][5]) {
    string processedText;
    for (char ch : plaintext) {
        if (isalpha(ch)) {
            processedText += toupper(ch);
        }
    }
    for (size_t i = 1; i < processedText.length(); i += 2) {
        if (processedText[i] == processedText[i - 1]) {
            processedText.insert(i, "X");
        }
    }
    if (processedText.length() % 2 != 0) {
        processedText += 'X';
    }
    string ciphertext;
    for (size_t i = 0; i < processedText.length(); i += 2) {
        int row1, col1, row2, col2;
        getCharPosition(playfairMatrix, processedText[i], row1, col1);
        getCharPosition(playfairMatrix, processedText[i + 1], row2, col2);
        if (row1 == row2) {
            ciphertext += playfairMatrix[row1][(col1 + 1) % 5];
            ciphertext += playfairMatrix[row2][(col2 + 1) % 5];
        } else if (col1 == col2) {
            ciphertext += playfairMatrix[(row1 + 1) % 5][col1];
            ciphertext += playfairMatrix[(row2 + 1) % 5][col2];
        } else {
            ciphertext += playfairMatrix[row1][col2];
            ciphertext += playfairMatrix[row2][col1];
        }
    }
    return ciphertext;
}
int main() {
    string key = "Monarchy";
    string plaintext = "instruments";
    char playfairMatrix[5][5];
    generatePlayfairMatrix(key, playfairMatrix);
    string ciphertext = encryptPlayfair(plaintext, playfairMatrix);
    cout << "Plaintext: " << plaintext << endl;
    cout << "Key: " << key << endl;
    cout << "Ciphertext: " << ciphertext << endl;
    return 0;
}

***************************hill cipher *********************5
#include <iostream>
using namespace std;

void getKeyMatrix(string key, int keyMatrix[][3])
{
	int k = 0;
	for (int i = 0; i < 3; i++) 
	{
		for (int j = 0; j < 3; j++) 
		{
			keyMatrix[i][j] = (key[k]) % 65;
			k++;
		}
	}
}
void encrypt(int cipherMatrix[][1],
			int keyMatrix[][3], 
			int messageVector[][1])
{
	int x, i, j;
	for (i = 0; i < 3; i++) 
	{
		for (j = 0; j < 1; j++)
		{
			cipherMatrix[i][j] = 0;
		
			for (x = 0; x < 3; x++)
			{
				cipherMatrix[i][j] += 
					keyMatrix[i][x] * messageVector[x][j];
			}
		
			cipherMatrix[i][j] = cipherMatrix[i][j] % 26;
		}
	}
}
void HillCipher(string message, string key)
{
	int keyMatrix[3][3];
	getKeyMatrix(key, keyMatrix);
	int messageVector[3][1];
	for (int i = 0; i < 3; i++)
		messageVector[i][0] = (message[i]) % 65;
	int cipherMatrix[3][1];
	encrypt(cipherMatrix, keyMatrix, messageVector);
	string CipherText;
	for (int i = 0; i < 3; i++)
		CipherText += cipherMatrix[i][0] + 65;
	cout << " Ciphertext:" << CipherText;
}
int main()
{
	string message = "ACT";
	string key = "GYBNQKURP";
	HillCipher(message, key);
	return 0;
}

************************** vigenere cipher*****************************6
#include<bits/stdc++.h>
using namespace std;

string generateKey(string str, string key)
{
	int x = str.size();
	for (int i = 0; ; i++)
	{
		if (x == i)
			i = 0;
		if (key.size() == str.size())
			break;
		key.push_back(key[i]);
	}
	return key;
}
string cipherText(string str, string key)
{
	string cipher_text;
	for (int i = 0; i < str.size(); i++)
	{
		char x = (str[i] + key[i]) %26;
		x += 'A';
		cipher_text.push_back(x);
	}
	return cipher_text;
}
string originalText(string cipher_text, string key)
{
	string orig_text;
	for (int i = 0 ; i < cipher_text.size(); i++)
	{
		char x = (cipher_text[i] - key[i] + 26) %26;
		x += 'A';
		orig_text.push_back(x);
	}
	return orig_text;
}
int main()
{
	string str = "GEEKSFORGEEKS";
	string keyword = "AYUSH";
	string key = generateKey(str, keyword);
	string cipher_text = cipherText(str, key);
	cout << "Ciphertext : "<< cipher_text << "\n";
	cout << "Original/Decrypted Text : "<< originalText(cipher_text, key);
	return 0;
}

*************************** vernam cipher**********************************7
#include <bits/stdc++.h>
#include <iostream>
using namespace std;

string stringEncryption(string text, string key)
{
	string cipherText = "";
	int cipher[key.length()];
	for (int i = 0; i < key.length(); i++) {
		cipher[i] = text.at(i) - 'A' + key.at(i) - 'A';
	}
	for (int i = 0; i < key.length(); i++) {
		if (cipher[i] > 25) {
			cipher[i] = cipher[i] - 26;
		}
	}
	for (int i = 0; i < key.length(); i++) {
		int x = cipher[i] + 'A';
		cipherText += (char)x;
	}
	return cipherText;
}
static string stringDecryption(string s, string key)
{
	string plainText = "";
	int plain[key.length()];
	for (int i = 0; i < key.length(); i++) {
		plain[i] = s.at(i) - 'A' - (key.at(i) - 'A');
	}
	for (int i = 0; i < key.length(); i++) {
		if (plain[i] < 0) {
			plain[i] = plain[i] + 26;
		}
	}
	for (int i = 0; i < key.length(); i++) {
		int x = plain[i] + 'A';
		plainText += (char)x;
	}
	return plainText;
}
int main()
{
	string plainText = "Hello";
	string key = "MONEY";
	for (int i = 0; i < plainText.length(); i++) {
		plainText[i] = toupper(plainText[i]);
	}
	for (int i = 0; i < key.length(); i++) {
		key[i] = toupper(key[i]);
	}
	string encryptedText = stringEncryption(plainText, key);
	cout << "Cipher Text - " << encryptedText << endl;
	cout << "Message - "<< stringDecryption(encryptedText, key);
	return 0;
}

*************************PC1 64 from 54 bits*****************************8
#include <iostream>
#include <bitset>
using namespace std;

// Permutation table PC1
const int PC1[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};
// Function to permute the key using PC1 table
bitset<56> permuteKey64to56(const bitset<64>& key) {
    bitset<56> permutedKey;
    for (int i = 0; i < 56; ++i) {
        permutedKey[i] = key[PC1[i] - 1]; 
    }
    return permutedKey;
}
int main() {
    // Example 64-bit key
    bitset<64> key64("0000000100100011010001010110011110001001101010111100110111101111");
    // Generate 56-bit key using PC1 table
    bitset<56> key56 = permuteKey64to56(key64);
    // Output the generated 56-bit key
    cout << "Generated 56-bit key: " << key56 << endl;
    return 0;
}

****************************PC2 64 from 54 bits**************************9
#include <iostream>
#include <bitset>
using namespace std;

// PC2 table for key permutation
int PC2_table[48] = {14, 17, 11, 24, 1, 5,
                      3, 28, 15, 6, 21, 10,
                      23, 19, 12, 4, 26, 8,
                      16, 7, 27, 20, 13, 2,
                      41, 52, 31, 37, 47, 55,
                      30, 40, 51, 45, 33, 48,
                      44, 49, 39, 56, 34, 53,
                      46, 42, 50, 36, 29, 32};
// Function to convert permutation key (C1D1) to key (K1) using PC2 table
bitset<48> permutedKey(bitset<56> inputKey) {
    bitset<48> outputKey;
    for (int i = 0; i < 48; ++i) {
        outputKey[i] = inputKey[PC2_table[i] - 1];
    }
    return outputKey;
}
int main() {
    // Example input permutation key (C1D1) of size 56 bits
    string inputKey = "11001011000000000000000000000000000000000000000000000000";
    // Convert input string to bitset
    bitset<56> permKey(inputKey);
    // Convert permutation key (C1D1) to key (K1) using PC2 table
    bitset<48> keyK1 = permutedKey(permKey);
    // Print the resulting key (K1)
    cout << "Key (K1) after PC2 table: " << keyK1 << endl;

    return 0;
}

**************************** RSA-1*****************************************10
#include <iostream>
#include <cmath>
using namespace std;

// Function to calculate modular exponentiation (base^exp mod mod)
long long modExp(long long base, long long exp, long long mod) {
    if (mod == 1) return 0;
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}
// Function to calculate modular multiplicative inverse using extended Euclidean algorithm
long long modInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}
int main() {
    long long p, q, n, phi, e = 13, d, M;

    // Input values
    cout << "Enter prime number p: ";
    cin >> p;
    cout << "Enter prime number q: ";
    cin >> q;
    cout << "Enter message M: ";
    cin >> M;
    // Calculate n and phi
    n = p * q;
    phi = (p - 1) * (q - 1);
    // Calculate d (private key)
    d = modInverse(e, phi);
    // Encryption
    long long C = modExp(M, e, n);
    cout << "Encrypted message: " << C << endl;
    // Decryption
    M = modExp(C, d, n);
    cout << "Decrypted message: " << M << endl;
    return 0;
}

*************************RSA-2****************************************************
#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int h)
{
	int temp;
	while (1) {
		temp = a % h;
		if (temp == 0)
			return h;
		a = h;
		h = temp;
	}
}

int main()
{
	double p; 
	double q;
	cout<<"Enter prime number1 ";
	cin>>p;
	cout<<"Enter prime number2 ";
	cin>>q;
	double n = p * q;
	double e = 2;
	double phi = (p - 1) * (q - 1);
	while (e < phi) {
		if (gcd(e, phi) == 1)
			break;
		else
			e++;
	}
	int k = 2; 
	double d = (1 + (k * phi)) / e;
	double msg = 12;
	printf("Message data = %lf", msg);
	double c = pow(msg, e);
	c = fmod(c, n);
	printf("\nEncrypted data = %lf", c);
	double m = pow(c, d);
	m = fmod(m, n);
	printf("\nOriginal Message Sent = %lf", m);
	return 0;
}
