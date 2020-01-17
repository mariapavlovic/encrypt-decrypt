//
//  main.cpp
//  project_2
//
//  Created by Maria Pavlovic on 2019-11-07.
//  Copyright © 2019 Maria Pavlovic. All rights reserved.
//

#include <iostream>
#include <cctype>
#include <cmath>


#ifndef MARMOSET_TESTING
int main();
#endif

char *encode(char *plaintext, unsigned long key);
char *decode(char *cihpertext, unsigned long key);
std::size_t string_length(char s_str[]);

#ifndef MARMOSET_TESTING
int main() {
    char str0[]{ "A Elbereth Gilthoniel\nsilivren penna miriel\n"
        "o menel aglar elenath!\nNa-chaered palan-diriel\n" "o galadhremmin ennorath,\nFanuilos, le linnathon\n" "nef aear, si nef aearon!" };
    std::cout << "\"" << str0 << "\"" << std::endl;
    char *ciphertext{ encode( str0, 51323 ) };
    std::cout << "\"" << ciphertext << "\"" << std::endl;
    char *plaintext{ decode( ciphertext, 51323 ) };
    std::cout << "\"" << plaintext << "\"" << std::endl;
    delete[] plaintext;
    plaintext = nullptr;
    delete[] ciphertext;
    ciphertext = nullptr;

    std::cout << std::endl;

    return 0;
}
#endif

char *encode(char *plaintext, unsigned long key) { //encodes the plaintext message and converts the ciphertext to ASCII armour
    unsigned char S[256];
    int i = 0;
    int j = 0;
    int k = 0;
    unsigned char r = 0;
    unsigned char temp;
    unsigned char R;
    std::size_t size;

    if (string_length(plaintext)%4 != 0) {
        size = (int(string_length(plaintext))/4*4 + 5);
    }
    else {
        size = (string_length(plaintext)) + 1;
    }

    for (int k = 0; k < 256; ++k) {
        S[k] = k;
    }

    for (int count = 0; count < 256; ++count) { // scrambles S array
        k = i % 64;
        j = (j + S[i] + ((key >> k) & 1L)) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        i = (i + 1) % 256;
    }

    for (std::size_t count = 0; count < size; ++count) { // xor every char in plaintext
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        r = (S[i] + S[j]) % 256;
        R = S[r];
        plaintext[count] ^= R;
    }

    // ascii armour
    int dec_array[size-1];
    unsigned int sum;
    char new_ascii[5];
    int exp = 4;
    int quotient;
    unsigned int return_size = ceil((size-1)/4)*5;
    char *return_array{new char[return_size]};
    int index = 0;

    for (int t = 0; t < size-1; ++t) {
            dec_array[t] = static_cast<unsigned char>(plaintext[t]);
    }

    for (int rep = 0; rep < (size-1)/4; ++rep) {
        sum = (dec_array[0+4*rep] << 24) + (dec_array[1+4*rep] << 16) + (dec_array[2+4*rep] << 8) + dec_array[3+4*rep];
        exp = 4;
        for (int r = 0; r < 5; ++r) {
            quotient = sum/pow(85, exp);
            new_ascii[r] = quotient + 33;
            sum -= quotient*pow(85, exp);
            exp -= 1;
        }

        for (int g = 0; g < 5; ++g) {
            return_array[index] = new_ascii[g];
            index += 1;
        }

        return_array[return_size] = '\0'; // append zero

    }

    return return_array;
}


char *decode(char *cihpertext, unsigned long key) {

    std::size_t size;
    size = (string_length(cihpertext));

    // ascii armour undo
    unsigned char *dec_array{new unsigned char[string_length(cihpertext)/5*4]};
    unsigned int sum = 0;
    unsigned char new_ascii[5];
    unsigned int return_size = int(ceil(string_length(cihpertext)/5)*4) + 1;
    char *return_array{new char[return_size]};

    for (int rep = 1; rep <= size/5; ++rep) {
        sum = 0;
        for (int g = 0; g < 5; ++g) {
            new_ascii[g] = cihpertext[g+5*(rep-1)];
        }

        for (int g = 0; g < 5; ++g) {
            sum += (unsigned int)(new_ascii[g] - 33)*(unsigned int)(pow(85, 4-g));
        }

        dec_array[0+4*(rep-1)] = (unsigned char)(sum >> 24) ;

        dec_array[1+4*(rep-1)] = (unsigned char)(sum >> 16);

        dec_array[2+4*(rep-1)] = (unsigned char)(sum >> 8);

        dec_array[3+4*(rep-1)] = (unsigned char)(sum);

    }

    unsigned char S[256];
    for (int k = 0; k < 256; ++k) {
        S[k] = k;
    }

    int i = 0;
    int j = 0;
    int k = 0;
    unsigned char r = 0;
    unsigned char temp;
    unsigned char R;

    for (int count = 0; count < 256; ++count) { // scramble S array
        k = i % 64;
        j = (j + S[i] + ((key >> k) & 1UL)) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        i = (i + 1) % 256;
    }

    for (std::size_t count = 0; count < return_size - 1; ++count) { // xor every char in ciphertext
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        temp = S[i];
        S[i] = S[j];
        S[j] = temp;
        r = (S[i] + S[j]) % 256;
        R = S[r];
        return_array[count] = dec_array[count]^R;
    }

    return_array[return_size-1] = '\0';
    return return_array;
}

std::size_t string_length(char s_str[]) {
    for (std::size_t k{0}; true; ++k) {
        if (s_str[k] == '\0') {
            return k;
        }
    }
}
