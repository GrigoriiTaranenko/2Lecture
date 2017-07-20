/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
/**
 * Проверка файла
 * @param file
 * @return
 */
FILE *card;
FILE *jpeg;

bool checkPhoto(FILE *file);

void offsetByte(BYTE startByte[4]);

void 

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Недостаточно переменных");
        return 1;
    }

    card = fopen(argv[1], "r");
    jpeg = fopen("mas.jpeg", "w");

    if (card == null) {
        return false;
    }

    BYTE arrayJpeg[512];
    BYTE checkJpeg[3];
    BYTE byt;
    int i = 0;
    int k = 1000000;
    size_t result = fread(&byt, sizeof(BYTE), 1, card);
    printf("byt = %d \n", byt);
    while (k != 0  && result == sizeof(BYTE)) {
        i++;
    //    printf ("byt = %x \n", byt);
        result = fread(&byt, sizeof(BYTE), 1, card);
        if (byt == 255) {
            k --;
        }
    }
    printf("i = %i байт \n", i);
    printf("байт равен %x \n", byt);
    fread(&byt, sizeof(BYTE), 1, card);
    printf("байт равен %x \n", byt);
    fread(&byt, sizeof(BYTE), 1, card);
    printf("байт равен %x \n", byt);
    fread(&byt, sizeof(BYTE), 1, card);
    printf("байт равен %x \n", byt);
    fclose(card);
    return 0;
}

/*
int x[10000];
fread(x, 1000, 1, card);
for (int i = 0; i<1000; i++) {
printf("x = %x \n", x[i]);
}*/