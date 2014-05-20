#ifndef GOST_H
#define GOST_H

#include <iostream>
#include <stdio.h>

using namespace std;

#define SOURCE      "in.txt"
#define CRYPT       "crypt.txt"
#define DECRYPT     "decrypt.txt"

class GOST
{
public:
    GOST(char key_in[33]);
    void Crypt();
    void Decrypt();

private:
    char secretKey[33];

    unsigned int shiftBox[8][16] =
        {
            {13,11,4,0,3,15,5,9,1,10,14,7,6,8,2,12},
            {15,1,13,0,11,3,6,8,9,14,12,2,5,10,7,4},
            {8,11,4,7,6,13,15,10,2,3,14,1,0,12,5,9},
            {10,5,11,13,2,4,8,1,14,7,12,15,6,0,9,3},
            {8,9,10,1,0,7,15,13,2,14,5,12,11,4,6,3},
            {6,3,13,10,0,8,1,15,9,2,5,11,12,4,14,7},
            {9,11,0,6,1,15,13,2,8,10,5,7,12,14,3,4},
            {7,15,5,0,3,11,10,4,9,2,13,6,14,1,8,12}
        };

    unsigned int key[8];
};

#endif // GOST_H
