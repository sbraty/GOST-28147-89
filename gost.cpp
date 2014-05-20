#include "gost.h"

GOST::GOST(char key_in[33]) {
    for(int i = 0; i < 33; i++) {
        secretKey[i] = key_in[i];
    }
}


void GOST::Crypt() {
    for(int i = 0; i<8; i++) {
        key[i] = (secretKey[4*i]<<24) + (secretKey[1+4*i]<<16) + (secretKey[2+4*i]<<8) + secretKey[3+4*i];  // razbivaem kluck na 8 chastei
    }

    FILE* in;
    FILE* out;
    int fileSize;

    in = fopen(SOURCE, "r");

    //vichasyaem razmer fila
    fseek (in, 0, SEEK_END);
    fileSize = ftell(in);
    fseek (in, 0, SEEK_SET);

    out = fopen(CRYPT, "wr");

    do{
        unsigned int subBlockA = 0; // starshii subblock
        unsigned int subBlockB = 0; // mladshii subblock

        //zapolnyaem starshi subblock
        if(fileSize>=4)
        {
            fread(&subBlockA, 4, 1, in);
            fileSize -= 4;
        }
        else
        {
            fread(&subBlockA, fileSize, 1, in);
            for(int i = 0; i<(4-fileSize); i++)
            {
                subBlockA += (32<<(24-(i*8)));
                fileSize = 0;
            }
        }

        //zapolnyaem mladshii subblock
        if(fileSize>=4)
        {
            fread(&subBlockB, 4, 1, in);
            fileSize -= 4;
        }
        else
        {
            fread(&subBlockB, fileSize, 1, in);
            for(int i = 0; i<(4-fileSize); i++)
            {
                subBlockB += (32<<(24-(i*8)));
                fileSize = 0;
            }
        }


        unsigned int subBlockBuf;

        // 16 ili 32 raunda
        for(int i = 0; i<32; i++)
        {
            subBlockBuf = 0;

            if(i<24) {
                subBlockBuf = (subBlockA+key[i%8]) % 0x100000000;
            }
            else {
                subBlockBuf = (subBlockA+key[7-(i%8)]) % 0x100000000;
            }

            // preobrazuu v hex
            unsigned int smallBlock[8] =
            {
                (subBlockBuf & 0xF0000000)>>28,
                (subBlockBuf & 0xF000000)>>24,
                (subBlockBuf & 0xF00000)>>20,
                (subBlockBuf & 0xF0000)>>16,
                (subBlockBuf & 0xF000)>>12,
                (subBlockBuf & 0xF00)>> 8,
                (subBlockBuf & 0xF0)>> 4,
                (subBlockBuf & 0xF)
            };

            for(int j = 0; j<8; j++)
            {
                smallBlock[j] = shiftBox[j][smallBlock[j]];
            }

            subBlockBuf = (smallBlock[0]<<28) +
                    (smallBlock[1]<<24) +
                    (smallBlock[2]<<20) +
                    (smallBlock[3]<<16) +
                    (smallBlock[4]<<12) +
                    (smallBlock[5]<< 8) +
                    (smallBlock[6]<< 4) +
                    smallBlock[7];

            subBlockBuf = (subBlockBuf<<11);
            subBlockBuf ^= subBlockB;

            if(i != 31)
            {
                subBlockB = subBlockA;
                subBlockA = subBlockBuf;
            }
            else
            {
                subBlockB = subBlockBuf;
            }
        }

        fwrite(&subBlockA, 4, 1, out);
        fwrite(&subBlockB, 4, 1, out);
    }while(fileSize != 0);

    fclose(in);
    fclose(out);

}


void GOST::Decrypt() {
    for(int i = 0; i<8; i++) {
        key[i] = (secretKey[4*i]<<24) + (secretKey[1+4*i]<<16) + (secretKey[2+4*i]<<8) + secretKey[3+4*i];  // razbivaem kluck na 8 chastei
    }

    FILE* in;
    FILE* out;
    int fileSize;


    in = fopen(CRYPT, "r");

    //vichasyaem razmer fila
    fseek (in, 0, SEEK_END);
    fileSize = ftell(in);
    fseek (in, 0, SEEK_SET);

    out = fopen(DECRYPT, "w");


    do{
        unsigned int subBlockA = 0;
        unsigned int subBlockB = 0;

        //zapolnyaem starshi subblock
        if(fileSize>=4)
        {
            fread(&subBlockA, 4, 1, in);
            fileSize -= 4;
        }
        else
        {
            fread(&subBlockA, fileSize, 1, in);
            for(int i = 0; i<(4-fileSize); i++)
            {
                subBlockA += (32<<(24-(i*8)));
                fileSize = 0;
            }
        }

        //zapolnyaem mladshii subblock
        if(fileSize>=4)
        {
            fread(&subBlockB, 4, 1, in);
            fileSize -= 4;
        }
        else
        {
            fread(&subBlockB, fileSize, 1, in);
            for(int i = 0; i<(4-fileSize); i++)
            {
                subBlockB += (32<<(24-(i*8)));
                fileSize = 0;
            }
        }


        unsigned int subBlockBuf;


        for(int i = 0; i<32; i++)
        {
            subBlockBuf = 0;

            if(i<8) {
                subBlockBuf = (subBlockA+key[i%8])  % 0x100000000;
            }
            else {
                subBlockBuf = (subBlockA+key[7-(i%8)]) % 0x100000000;
            }


            unsigned int smallBlock[8] =
            {
                (subBlockBuf & 0xF0000000)>>28,
                (subBlockBuf & 0xF000000)>>24,
                (subBlockBuf & 0xF00000)>>20,
                (subBlockBuf & 0xF0000)>>16,
                (subBlockBuf & 0xF000)>>12,
                (subBlockBuf & 0xF00)>> 8,
                (subBlockBuf & 0xF0)>> 4,
                (subBlockBuf & 0xF)
            };

            for(int j = 0; j<8; j++)
            {
                smallBlock[j] = shiftBox[j][smallBlock[j]];
            }

            subBlockBuf = (smallBlock[0]<<28) +
                    (smallBlock[1]<<24) +
                    (smallBlock[2]<<20) +
                    (smallBlock[3]<<16) +
                    (smallBlock[4]<<12) +
                    (smallBlock[5]<< 8) +
                    (smallBlock[6]<< 4) +
                    smallBlock[7];

            subBlockBuf = (subBlockBuf<<11);
            subBlockBuf ^= subBlockB;

            if(i != 31)
            {
                subBlockB = subBlockA;
                subBlockA = subBlockBuf;
            }
            else
            {
                subBlockB = subBlockBuf;
            }
        }

        fwrite(&subBlockA, 4, 1, out);
        fwrite(&subBlockB, 4, 1, out);
    }while(fileSize != 0);

    fclose(in);
    fclose(out);
}
