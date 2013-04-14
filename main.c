#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int stringSize = 30;

void checkKeys(int argc, char *argv[]);
void editInputFiles();
int inputNumericalData();
void showResult();
void showHelp();
FILE *fopenWithCheck(char *path, char *mode);
int *fInputArray(FILE *file);
void combineToFile(int *firstArray, int *secondArray, FILE *resultFile);

int main(int argc, char *argv[])
{
    checkKeys(argc, argv);
    int *firstArray, *secondArray;
    FILE *firstFile, *secondFile, *resultFile;

    firstFile = fopenWithCheck("first", "rb");
    secondFile = fopenWithCheck("second", "rb");
    resultFile = fopen("result", "wb");

    firstArray = fInputArray(firstFile);
    secondArray = fInputArray(secondFile);
    combineToFile(firstArray, secondArray, resultFile);

    free(firstArray);
    free(secondArray);
    fclose(firstFile);
    fclose(secondFile);
    fclose(resultFile);
    return 0;
}

FILE *fopenWithCheck(char *path, char *mode)
{
    FILE *file;
    if (!(file = fopen(path, mode))) {
        printf("No input file\n");
        exit(1);
    }
    return file;
}

int *fInputArray(FILE *file)
{
    int size, *array;
    fread(&size, sizeof(int), 1, file);
    array = (int *) malloc(size * sizeof(int));
    array++;
    fread(array, sizeof(int), size - 1, file);
    *(--array) = size;
    return array;
}

void combineToFile(int *firstArray, int *secondArray, FILE *resultFile)
{
    int size, i, j;
    size = firstArray[0] + secondArray[0] - 1;
    fwrite(&size, sizeof(int), 1, resultFile);
    i = firstArray[0] - 1;
    j = secondArray[0] - 1;
    while (i >= 1 && j >=1) {
        if (firstArray[i] > secondArray[j]) {
            fwrite(&firstArray[i], sizeof(int), 1, resultFile);
            i--;
        } else {
            fwrite(&secondArray[j], sizeof(int), 1, resultFile);
            j--;
        }
    }
    if (i == 1) {
        while (j >= 1) {
            fwrite(&secondArray[j], sizeof(int), 1, resultFile);
            j--;
        }
    } else {
        while (i >= 1) {
            fwrite(&firstArray[i], sizeof(int), 1, resultFile);
            i--;
        }
    }
}

void checkKeys(int argc, char *argv[])
{
    if (argc == 1) {
        return;
    }
    if (!strcmp("-h", argv[1])) {
        showHelp();
    }
    if (!strcmp("-s", argv[1])) {
        showResult();
    }
    if (!strcmp("-e", argv[1])) {
        editInputFiles();
    }
}

void editInputFiles()
{
    int *array, size, i;
    FILE *firstFile, *secondFile;
    firstFile = fopen("first", "wb");
    secondFile = fopen("second", "wb");

    printf("Enter size of first array: ");
    size = inputNumericalData();
    array = (int *) malloc(++size * sizeof(int));
    printf("Enter the numbers for first array: ");
    array[0] = size;
    for (i = 1; i < size; i++) {
        array[i] = inputNumericalData();
    }
    fwrite(array, sizeof(int), size, firstFile);
    fclose(firstFile);
    free(array);

    printf("Enter size of second array: ");
    size = inputNumericalData();
    array = (int *) malloc(++size * sizeof(int));
    printf("Enter the numbers for second array: ");
    array[0] = size;
    for (i = 1; i < size; i++) {
        array[i] = inputNumericalData();
    }
    fwrite(array, sizeof(int), size, secondFile);
    fclose(secondFile);
    free(array);
    exit(0);
}

int inputNumericalData()
{
    char buffer[stringSize];
    int num;
    do {
        fgets(buffer, stringSize, stdin);
        num = atoi(buffer);
        if (num <= 0) {
            printf("Incorrect value. Try again: ");
            continue;
        }
        break;
    } while (1);
    return num;
}

void showResult()
{
    int *array, i;
    FILE *resultFile;
    resultFile = fopenWithCheck("result", "rb");
    array = fInputArray(resultFile);
    printf("Result: ");
    for (i = 1; i < array[0]; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    exit(0);
}

void showHelp()
{
    printf("=============================================================\n");
    printf("                           MANUAL\n");
    printf("Options: -e, -s\n");
    printf("    -e\n");
    printf("        DESCRIPTION: Opens \"first\" and \"second\" files for editing.\n");
    printf("        USAGE:       Enter size and values for first file and then for second.\n");
    printf("    -s\n");
    printf("        DESCRIPTION: Shows result from file \"result\".\n");
    printf("=============================================================\n");
    exit(0);
}
