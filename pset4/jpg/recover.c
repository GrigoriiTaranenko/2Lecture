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
FILE *jpegFile;

const int SIZE_JPEG_BLOCK = 512;
const BYTE SIZE_RECOGNIZE_JPEG = 4;

const BYTE chaos[] = {0xff, 0xd8, 0xff};

void createFile(int *numberJpeg);

void swap(BYTE array[]);

void getByteBeforeJpeg(BYTE checkJpeg[SIZE_RECOGNIZE_JPEG]);

bool checkBeginJpeg(BYTE arrayJpeg[]);

void firstWriteVariable(BYTE jpeg[SIZE_RECOGNIZE_JPEG], BYTE arrayJpeg[SIZE_JPEG_BLOCK], size_t *countElement);

void writeFile(BYTE arrayJpeg[SIZE_JPEG_BLOCK], int *numberJpeg);

void offsetByte(BYTE arrayJpeg[SIZE_JPEG_BLOCK]);

void errorWriteFile();



int main(int argc, char* argv[])
{

    int numberJpeg = 1;

    size_t countElement;
    BYTE arrayJpeg[SIZE_JPEG_BLOCK];
    BYTE checkJpeg[SIZE_RECOGNIZE_JPEG];

    if (argc != 2) {
        printf("Недостаточно переменных \n");
        return 1;
    }

    card = fopen("card.raw", "r");
    if (card == NULL) {
        printf("Неудалось открыть файл \n");
        return 1;
    }

    getByteBeforeJpeg(checkJpeg);
    firstWriteVariable(checkJpeg, arrayJpeg, &countElement);
    printf("[0] = %d [1] =%d [2] = %d [3] = %d \n", arrayJpeg[0], arrayJpeg[1], arrayJpeg[2], arrayJpeg[3]);

    createFile(&numberJpeg);
    printf("[0] = %d [1] =%d [2] = %d [3] = %d \n", arrayJpeg[0], arrayJpeg[1], arrayJpeg[2], arrayJpeg[3]);
    while (countElement == SIZE_JPEG_BLOCK) {
        writeFile(arrayJpeg, &numberJpeg);
        countElement = fread(arrayJpeg, 1, SIZE_JPEG_BLOCK, card);
    }

    fclose(card);
    return 0;
}

/**
 * Создания объекта для сохранения фотографии в формате jpeg. И номер jpeg файла увеличивает на один
 * @param numberJpeg - номер фото
 */
void createFile(int *numberJpeg) {
    char nameFile[20];
    sprintf(nameFile, "%d", *numberJpeg);
    strncat(nameFile, ".jpeg", 5);
    jpegFile = fopen(nameFile, "w");
    if (jpegFile == NULL) {
        printf("Неудалось создать файл \n");
        exit(EXIT_FAILURE);
    }
    *numberJpeg += 1;
}


/**
 * меняет местами значения элементов в массиве
 * @param array
 */
void swap(BYTE array[]) {
    for (int i = 0; i<3; i++) {
        BYTE c = array[i];
        array[i] = array[i+1];
        array[i+1] = c;
    }
}

/**
 * нахождения первой последовательности до чтения jpeg
 * @return bool При нахождении возвращает True при ненахождении возвращения false
 * @var BYTE result[]
 */
void getByteBeforeJpeg(BYTE checkJpeg[SIZE_RECOGNIZE_JPEG]) {
    BYTE offset;
    size_t size = fread(checkJpeg, 1, SIZE_RECOGNIZE_JPEG, card);
    int i= 0;
    while (size ==  SIZE_RECOGNIZE_JPEG) {
        if (checkBeginJpeg(checkJpeg)) {
            printf("[0] = %d [1] =%d [2] = %d [3] = %d \n", checkJpeg[0], checkJpeg[1], checkJpeg[2], checkJpeg[3]);
            return;
        } else {
            swap(checkJpeg);
            size = 3 + fread(&checkJpeg[3], 1, 1, card);
        }
    }
    printf("Не найденно последовательности \n");
    fclose(jpegFile);
    exit(EXIT_FAILURE);
}

bool checkBeginJpeg(BYTE arrayJpeg[SIZE_JPEG_BLOCK]) {
    return (memcmp(arrayJpeg, chaos, 3) == 0 && arrayJpeg[3] > 223 && arrayJpeg[3] < 240);
}

/**
 * Первая запись в массив jpeg блока
 * @param int jpeg[4]
 */
void firstWriteVariable(BYTE jpeg[SIZE_RECOGNIZE_JPEG], BYTE arrayJpeg[SIZE_JPEG_BLOCK], size_t *countElement) {
    *countElement = fread(arrayJpeg, 1, SIZE_JPEG_BLOCK - SIZE_RECOGNIZE_JPEG, card);
    if (*countElement != SIZE_JPEG_BLOCK - SIZE_RECOGNIZE_JPEG) {
        printf("Слишком мало значений файл \n");
        exit(EXIT_FAILURE);
    }
    offsetByte(arrayJpeg);
    for (int i = 0; i < SIZE_RECOGNIZE_JPEG; i++) {
        arrayJpeg[i] = jpeg[i];
    }
    printf("[0] = %d [1] =%d [2] = %d [3] = %d \n", arrayJpeg[0], arrayJpeg[1], arrayJpeg[2], arrayJpeg[3]);
    *countElement = 512;
}

/**
 * Создает новый файл если у
 * @param Byte arrayJpeg[]
 * @param int *numberJpeg
 */
void writeFile(BYTE arrayJpeg[SIZE_JPEG_BLOCK], int *numberJpeg) {
    if (checkBeginJpeg(arrayJpeg)) {
        printf("[0] = %d [1] =%d [2] = %d [3] = %d \n", arrayJpeg[0], arrayJpeg[1], arrayJpeg[2], arrayJpeg[3]);
        fclose(jpegFile);
        createFile(numberJpeg);
        fwrite(arrayJpeg, 1, SIZE_JPEG_BLOCK, jpegFile);
    } else {
        fwrite(arrayJpeg, 1, SIZE_JPEG_BLOCK, jpegFile);
    }

}

/**
 * Перемещаем значения массива на 4 байта вперед
 * @param arrayJpeg
 */
void offsetByte(BYTE arrayJpeg[SIZE_JPEG_BLOCK]) {
    for (int i = SIZE_JPEG_BLOCK - SIZE_RECOGNIZE_JPEG - 1; i>= 0; i--) {
        arrayJpeg[i + SIZE_RECOGNIZE_JPEG] = arrayJpeg[i];
    }
}

