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

const BYTE SIZE_JPEG_BLOCK = 512;
const BYTE SIZE_RECOGNIZE_JPEG = 4;

const BYTE chaos[] = {0xff, 0xd8, 0xff};

void createFile(int *numberJpeg);

void swap(BYTE array[]);

bool getByteBeforeJpeg(BYTE result[SIZE_RECOGNIZE_JPEG]);

bool checkBeginJpeg(BYTE arrayJpeg[]);

bool firstWriteVariable(BYTE jpeg[SIZE_RECOGNIZE_JPEG], BYTE arrayJpeg[SIZE_JPEG_BLOCK], size_t *countElement);

bool writeFile(BYTE arrayJpeg[SIZE_JPEG_BLOCK], int *numberJPEG);

void offsetByte(BYTE arrayJpeg[SIZE_JPEG_BLOCK]);

void errorWriteFile();



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
    size_t countElement;
    BYTE arrayJpeg[SIZE_JPEG_BLOCK];
    BYTE checkJpeg[SIZE_RECOGNIZE_JPEG];

    getByteBeforeJpeg(checkJpeg);
    createFile(&numberJpeg);
    firstWriteVariable(checkJpeg, arrayJpeg, &countElement);

    while (countElement == SIZE_JPEG_BLOCK) {
        if (writeFile(arrayJpeg, &numberJpeg)) {
            createFile(arrayJpeg);
        };
    }
    fclose(card);
    return 0;
}

/**
 * Создания объекта для сохранения фотографии в формате jpeg. И номер jpeg файла увеличивает на один
 * @param numberJpeg - номер фото
 */
void createFile(int *numberJpeg) {
    char *nameFile;
    sprintf(nameFile, "%d", *numberJpeg);
    strncat(nameFile, ".jpeg", 5);
    jpegFile = fopen(nameFile, "w");
    if (jpegFile == NULL) {
        printf("Неудалось создать файл");
        exit(EXIT_FAILURE);
    }
    *numberJpeg += 1;
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
bool getByteBeforeJpeg(BYTE result[SIZE_RECOGNIZE_JPEG]) {
    BYTE offset;
    size_t size = fread(&result, 1, SIZE_RECOGNIZE_JPEG, card);
    while (size ==  SIZE_RECOGNIZE_JPEG) {
        if (checkBeginJpeg(result)) {
            return true;
        } else {
            swap(result);
            size = 3 + fread(&result[3], sizeof(BYTE), 1, card);
        }
    }
    printf("Не найденно последовательности");
    exit(EXIT_FAILURE);
}

bool checkBeginJpeg(BYTE arrayJpeg[SIZE_JPEG_BLOCK]) {
    return (memcmp(arrayJpeg, chaos, 3) == 0 && arrayJpeg[3] > 223 && arrayJpeg[3] < 240);
}

/**
 * Первая запись в массив jpeg блока
 * @param int jpeg[4]
 */
bool firstWriteVariable(BYTE jpeg[SIZE_RECOGNIZE_JPEG], BYTE arrayJpeg[SIZE_JPEG_BLOCK], size_t *countElement) {
    *countElement = fread(arrayJpeg, 1, SIZE_JPEG_BLOCK - SIZE_RECOGNIZE_JPEG, card);
    if (*countElement != SIZE_JPEG_BLOCK - SIZE_RECOGNIZE_JPEG) {
        printf("Слишком мало значений файл");
        return false;
    }
    offsetByte(arrayJpeg);
    for (int i = 0; i < SIZE_RECOGNIZE_JPEG; i++) {
        arrayJpeg[i] = jpeg[i];
    }
    *countElement += SIZE_RECOGNIZE_JPEG;
    return true;
}

/**
 * Создает новый файл если у
 * @param arrayJpeg
 * @return
 */
bool writeFile(BYTE arrayJpeg[SIZE_JPEG_BLOCK], int *numberJPEg) {
    if (checkBeginJpeg(arrayJpeg)) {
        fclose(jpegFile);
        fwrite(arrayJpeg, SIZE_JPEG_BLOCK, 1, jpegFile);
        return true;
    }
    return false;
}

/**
 * Перемещаем значения массива на 4 байта вперед
 * @param arrayJpeg
 */
void offsetByte(BYTE arrayJpeg[SIZE_JPEG_BLOCK]) {
    for (int i = SIZE_JPEG_BLOCK - SIZE_RECOGNIZE_JPEG; i>= 0; i--) {
        arrayJpeg[i + SIZE_RECOGNIZE_JPEG] = arrayJpeg[i];
    }
}

