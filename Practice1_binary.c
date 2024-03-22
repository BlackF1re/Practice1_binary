//Вариант 10
//Создать файл вещественных чисел, который содержит матрицу А размерности n* m(m - четное число).Не считывая
//матрицу в память, реорганизовать файл путем перестановки столбцов : первого со вторым, третьего с четвертым и т.д.
//Вывести на печать исходную и полученную матрицы.
#include <stdio.h>
#include <stdlib.h>

void fill(const char* filename, int rows, int columns) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Ошибка при создании файла");
        exit(1);
    }

    for (int i = 0; i < rows * columns; i++) {
        float value = (float)rand() / (float)RAND_MAX;
        value = value * 20.0 - 10.0;
        fwrite(&value, sizeof(float), 1, file);
    }
    fclose(file);
}

void printMatrix(const char* filename, int rows, int columns) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL)
        error();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            float value;
            fread(&value, sizeof(float), 1, file);
            printf("%6.2f ", value);
        }
        printf("\n");
    }
    fclose(file);
}

int error() {
    printf("Произошла ошибка.");
    exit(1);
}

void reorganizate(int n, int m, FILE* file)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m / 2; ++j) {
            float first_element, second_element;
            long int offset1 = (i * m + 2 * j) * sizeof(float);
            long int offset2 = (i * m + 2 * j + 1) * sizeof(float);

            fseek(file, offset1, SEEK_SET);
            fread(&first_element, sizeof(float), 1, file);

            fseek(file, offset2, SEEK_SET);
            fread(&second_element, sizeof(float), 1, file);

            //сохранение значения
            float temp = first_element;

            //запись первого элемента
            fseek(file, offset1, SEEK_SET);
            fwrite(&second_element, sizeof(float), 1, file);

            //запись второго элемента
            fseek(file, offset2, SEEK_SET);
            fwrite(&temp, sizeof(float), 1, file);
        }
    }
}

int main() {
    SetConsoleOutputCP(1251);
    int isDigit = 0;
    char* file_name = "matrix.bin";

    printf("Введите количество строк:\t");
    int n = 0;
    isDigit = scanf("%d", &n);
    if (isDigit != 1 || n == 0)
        error();

    printf("Введите количество столбцов:\t");
    int m = 0;
    isDigit = scanf("%d", &m);
    if (isDigit != 1 || m % 2 != 0 || m == 0)
        error();

    FILE* file = fopen(file_name, "rb+");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        exit(1);
    }

    fill(file_name, n, m);

    printf("Исходная матрица:\n");
    printMatrix(file_name, n, m);

    //реорганизация
    reorganizate(n, m, file);
    fclose(file);

    printf("Полученная матрица:\n");
    printMatrix(file_name, n, m);

    return EXIT_SUCCESS;
}
