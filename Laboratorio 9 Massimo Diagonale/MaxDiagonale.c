#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <limits.h> // Per INT_MIN

#define MAXVALUE 100 // Valore massimo nel fill

void alloc_matrix(int *** matrix, int row, int col);
void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
void dealloc_matrix(int ** matrix, int row);

int main() {
    int N, massimoDiag;
    int** matrice;
    int* vettore;  // Vettore per memorizzare la diagonale

    srand(time(NULL));  // Inizializza il generatore di numeri casuali

    printf("\nInserisci N: ");
    scanf("%d", &N);

    // Alloca e riempi la matrice
    alloc_matrix(&matrice, N, N);
    fill_matrix(matrice, N, N, MAXVALUE);
    print_matrix(matrice, N, N);

    // Alloca il vettore per la diagonale
    vettore = malloc(N * sizeof(int));

    massimoDiag = INT_MIN;  // Inizializza a un valore molto basso

    // Estrai la diagonale principale e trova il massimo in parallelo
    #pragma omp parallel for shared(matrice, N, vettore) reduction(max:massimoDiag)
    for (size_t i = 0; i < N; i++) {
        vettore[i] = matrice[i][i];  // Copia l'elemento della diagonale
        if (massimoDiag < vettore[i]) {
            massimoDiag = vettore[i];  // Aggiorna il massimo
        }
    }

    // Solo il master thread stampa il massimo
    #pragma omp master
    {
        printf("Il massimo della diagonale principale è: %d\n", massimoDiag);
    }

    // Dealloca la memoria
    dealloc_matrix(matrice, N);
    free(vettore);

    return 0;
}

void alloc_matrix(int *** matrix, int row, int col) {
    *matrix = malloc(row * sizeof(int *));
    #pragma omp parallel for
    for (size_t i = 0; i < row; ++i) {
        (*matrix)[i] = malloc(col * sizeof(int));
    }
}

void fill_matrix(int ** matrix, int row, int col, int max_element) {
    #pragma omp parallel for
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            matrix[i][j] = rand() % max_element;
        }
    }
}

void print_matrix(int ** matrix, int row, int col) {
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            printf("%d\t", matrix[i][j]);
        }
        putchar('\n');
    }
}

void dealloc_matrix(int ** matrix, int row) {
    #pragma omp parallel for
    for (size_t i = 0; i < row; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}
