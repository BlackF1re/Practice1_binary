//Вариант 11
//Создать файл вещественных чисел, записать в него матрицу вещественных чисел А размерности n* n.Не считывая матрицу
//в память, реорганизовать файл путем замены элементов главной диагонали на элементы последнего столбца матрицы.
//Вывести на печать исходную и полученную матрицы.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void createFile(const char* filename, int n) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        openError();
        return;
    }
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int d = rand() % 101;

            fwrite(&d, sizeof(int), 1, f);
        }
    }
    fclose(f);
}

////попытка обработки в файле
//void reorganizeFile(const char* filename, int n) {
//    FILE* in = fopen(filename, "rb");
//    if (!in) {
//        perror("Error: could not open file");
//        return;
//    }
//    FILE* out = fopen(filename, "wb");
//    if (!out) {
//        perror("Error: could not open file");
//        return;
//    }
//    int d;
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j) {
//            fseek(in, i * n * sizeof(int) + j * sizeof(int), SEEK_SET);
//            fread(&d, sizeof(int), 1, in);
//            if (i == j) {
//                fseek(in, (n - 1) * sizeof(int), SEEK_CUR);
//                fread(&d, sizeof(int), 1, in);
//            }
//        }
//        fseek(in, i * n * sizeof(int), SEEK_SET);
//        fwrite(&d, sizeof(int) * n, 1, out);
//    }
//    fclose(in);
//    fclose(out);
//}

void reorganizeFile(const char* filename, int n) {
    FILE* in = fopen(filename, "rb");
    if (!in) {
        openError();
        return;
    }

    int* data = malloc(n * n * sizeof(int));
    if (!data) {
        allocateError();
        return;
    }

    if (fread(data, sizeof(int), n * n, in) != n * n) {
        readError();
        free(data);
        return;
    }

    fclose(in);

    for (int i = 0; i < n; ++i) {
        data[i * n + i] = data[i * n + n - 1];
    }

    FILE* out = fopen(filename, "wb");
    if (!out) {
        openError();
        free(data);
        return;
    }

    if (fwrite(data, sizeof(int), n * n, out) != n * n) {
        writeError();
    }

    fclose(out);
    free(data);
}

void printMatrix(const char* filename, int n) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        openError();
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int d;
            fseek(f, i * n * sizeof(int) + j * sizeof(int), SEEK_SET);
            fread(&d, sizeof(int), 1, f);
            printf("%d ", d);
        }
        printf("\n");
    }
    fclose(f);
}

#pragma region Errors
int openError()
{
    printf("Ошибка: невозможно выполнить открытие файла.");
    exit(1);
}

int writeError()
{
    perror("Ошибка: невозможно выполнить запись в файл.");
    exit(1);
}

int readError()
{
    perror("Ошибка: невозможно выполнить чтение файла.");
    exit(1);
}

int allocateError()
{
    perror("Ошибка: невозможно выделить память.");
    exit(1);
}

int valueError()
{
    perror("Ошибка: некорректное значение.");
    exit(1);
}
#pragma endregion

int main() {
    SetConsoleOutputCP(1251);
    const char* filename = "matrix.bin";
    int n = 5;
    createFile(filename, n);
    printf("Исходная матрица:\n");
    printMatrix(filename, n);

    reorganizeFile(filename, n);
    printf("\nОбновленная матрица:\n");
    printMatrix(filename, n);
    return 0;
}