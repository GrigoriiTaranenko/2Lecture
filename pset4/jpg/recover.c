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
#include <string.h>
#include <stdbool.h>

#include "bmp.h"
/**
 * Проверка файла
 * @param file
 * @return
 */
FILE *card;
FILE *jpeg;

const BYTE chaos[] = {0xff, 0xd8, 0xff};

bool createFile(int *numberJpeg);

void swap(BYTE array[]);

bool getByteBeforeJpeg(BYTE result[4]);

bool checkBeginJpeg(BYTE arrayJpeg[512]);

void firstWriteJPG(BYTE jpeg[4], BYTE arrayJpeg[512]);

bool writeFile(BYTE arrayJpeg[512]);

void offsetByte(BYTE arrayJpeg[512]);


int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Недостаточно переменных");
        return 1;
    }

    card = fopen(argv[1], "r");

    if (card == NULL) {
        printf("Неудалось открыть файл");
        return false;
    }
    int numberJpeg = 1;
    BYTE arrayJpeg[512];
    BYTE checkJpeg[4];

    if (!getByteBeforeJpeg(checkJpeg)) {
        printf("Не найденно последовательности");
        return 1;
    }
    if (!createFile(&numberJpeg)) {
        return 1;
    }
    firstWriteJPG(jpeg, arrayJpeg);

    fclose(card);
    return 0;
}

/**
 * Создания объекта для сохранения фотографии в формате jpeg
 * @param numberJpeg - номер фото
 * @return возвращает удачность создания файла
 */
bool createFile(int *numberJpeg) {
    char *nameFile;
    sprintf(nameFile, "%d", *numberJpeg);
    strncat(nameFile, ".jpeg", 5);
    jpeg = fopen(nameFile, "w");
    if (jpeg == NULL) {
        printf("Неудалось создать файл");
        return false;
    }
    *numberJpeg++;
    return true;
}


/**
 * меняет местами значения элементов в массиве
 * @param array
 */
void swap(BYTE array[]) {
    for (int i = 0; i<2; i++) {
        BYTE c = array[i];
        array[i+1] = array[i];
        array[i] = c;
    }
}

/**
 * нахождения первой последовательности до чтения jpeg
 * @return bool При нахождении возвращает True при ненахождении возвращения false
 * @var BYTE result[]
 */
bool getByteBeforeJpeg(BYTE result[4]) {
    BYTE offset;
    fread(&result, 4 * sizeof(BYTE), 1, card);
    while (result == sizeof(BYTE) + 3) {
        if (checkBeginJpeg(jpeg)) {
            return true;
        } else {
            swap(result);
            fread(&result[3], sizeof(BYTE), 1, jpeg);
        }
    }
    return false;
}

bool checkBeginJpeg(BYTE arrayJpeg[512]) {
    return (memcmp(arrayJpeg, chaos, 3) == 0 && arrayJpeg[3] > 223 && arrayJpeg[3] < 240);
}

/**
 *
 * @param int jpeg[4]
 */
void firstWriteJPG(BYTE jpeg[4], BYTE arrayJpeg[512]) {
    fread(arrayJpeg, 508, 1, card);
    offsetByte(arrayJpeg);
    for (int i = 0; i < 4; i++) {
        arrayJpeg[i] = jpeg[i];
    }
}

bool writeFile(BYTE arrayJpeg[512]) {
    if (checkBeginJpeg(arrayJpeg)) {
        fclose(jpeg);
    }
}

void offsetByte(BYTE arrayJpeg[512]) {
    for (int i = 508; i>= 0; i--) {
        arrayJpeg[i + 4] = arrayJpeg[i];
    }
}

