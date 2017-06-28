//
// Created by grigo on 27.06.17.
//
#include <stdio.h>
#include <string.h>

#define NELEMS(x)  (sizeof(x) / sizeof(*x))

/**
 *
 * @param arrayNumber
 * @param sizeArray
 */
void fillArray(int *arrayNumber, int sizeArray)
{
    int index;
    for (index = 0; index < sizeArray; index++) {
        printf("Enter %i element array \n", index);
        scanf("%d", &arrayNumber[index]);
    }
}

/**
 *
 * @param arrayNumber
 * @param sizeArray
 */
void outputArray(int *arrayNumber, int sizeArray)
{
    int index;
    for (index = 0; index < sizeArray; index++) {
        printf("Element %i array is %i \n", index, arrayNumber[index]);
    }
}

/**
 * меняет местами элементы
 * @param i
 * @param j
 */
void swap(int *i, int *j)
{
    int forSwap = *i;
    *i = *j;
    *j = forSwap;
}


/**
 * Пузырьковая сортировка
 * @param arrayNumber
 * @param sizeArray
 */
void sortBudle(int *arrayNumber, int sizeArray)
{
    int i;
    int j;
    for (i = sizeArray - 1; i >= 0; i--) {
        for (j = 0; j < i; j++) {
            if (arrayNumber[j]>arrayNumber[j + 1]) {
                swap(&arrayNumber[j], &arrayNumber[j + 1]);
            }
        }
    }
}

/**
 * Сортировка выбором
 * @param int *arrayNumber Массив чисел
 * @param int sizeArray размер массива
 */
void selectSort(int *arrayNumber, int sizeArray)
{
    int i;
    int j;
    int min;
    int minIndex;
    for (i = 0; i < sizeArray; i++) {
        min = arrayNumber[i];
        minIndex = i;
        for (j=i; j < sizeArray; j++) {
            if (arrayNumber[j]<min) {
                min = arrayNumber[j];
                minIndex = j;
            }
        }
        swap(&arrayNumber[i], &arrayNumber[minIndex]);
    }
}

/**
 * Сортировка вставки
 * @param int *arrayNumber
 * @param int sizeArray
 */
void insertionSort(int *arrayNumber, int sizeArray)
{
    int i;
    int j;
    for (i = 1; i < sizeArray; i++) {
        if (arrayNumber[i-1] > arrayNumber[i]) {
            j = i;
            while ((arrayNumber[j-1] > arrayNumber[j]) && j>0) {
                swap(&arrayNumber[j], &arrayNumber[j-1]);
                j--;
            }
        }
    }
}

int main(int argc, char * argv[])
{
    int sizeArray;
    int index;
    do {
        printf("Hello enter count array number \n");
        int check = scanf("%d", &sizeArray);
        if (check == 0) {
            char checkString = getchar();
            while (checkString != '\n' && checkString != EOF) {
                checkString = getchar();
            }
        }
    } while (sizeArray<1 || sizeArray >10000);
    int arrayNumber[sizeArray];
    fillArray(arrayNumber, sizeArray);
    outputArray(arrayNumber, sizeArray);
    insertionSort(arrayNumber, sizeArray);
    printf("\n Sort buble \n");
    outputArray(arrayNumber, sizeArray);

}