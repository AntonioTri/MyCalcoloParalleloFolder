#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>



// Funzione per calcolare il prodotto matrice per scalare
void matricePerScalare(int **matrix, int scalar, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            matrix[i][j] *= scalar;
        }
    }
}

// Funzione per calcolare il prodotto vettore per scalare
void vettorePerScalare(int *vector, int scalar, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] *= scalar;
    }
}

// Funzione per calcolare il prodotto vettoriale tra due vettori
int prodottoVettoriale(int *vector1, int *vector2, int size) {
    int product = 0;
    for (int i = 0; i < size; i++) {
        product += vector1[i] * vector2[i];
    }
    return product;
}

/**
 * Funzione per calcolare il prodotto tra due matrici
 * 
 * @param N Righe della matrice A
 * @param M Colonne della matrcie A e righe della amtrice B
 * @param P Colonne della matrice B
*/


int **matrixProduct(int **matrixA, int **matrixB, int N, int M, int P) {
    
    // Allocazione della matrice risultato
    int **result;
    allocateMatrix(&result, N, P);
    // Inizializzazione della matrice
    populateMatrix(result, N, P, 1);
    
    // Se il controllo N è uguale a P passa allora viene fatto il prodotto
    for (int i = 0; i < N; i++) 
        for (int j = 0; j < P; j++) 
            for (int k = 0; k < M; k++) 
                result[i][j] += matrixA[i][k] * matrixB[k][j];
    

    return result;

}


// Questa funzione calcola il prodotto tra matrice e vettore 
// sfruttando la stretegia si spezzettare per colonne
int *productMatrixVector(int **matrix, int *vector, int row, int col){

    // Allochiamo il nuovo vettore
    int *result, i, j;
    allocateVector(&result);

    for (i = 0; i < row; i++){
        for (j = 0; j < col; j++){
            result[i] += matrix[i][j] + vector[j]; 
        }
    }
    
    // ritorniamo il risultato
    return result;

}


// Funzione per trasporre una matrice
void **transposeMatrix(int **matrix, int row, int col) {
    
    // Allochiamo la matrice trasposta con dimensioni invertite
    int **transpose;
    allocateMatrix(&transpose, col, row);
    
    // Riempimento della matrice trasposta
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }

    // Ritorniamo la matrice trasposta
    return transpose
}

// Funzione per trasporre un vettore
void **transposeVector(int *vector, int size) {
    // Allochiamo la trasposta come vettore colonna (quindi matrice size x 1)
    int **transpose;
    // Come size inviamo il size del vettore, facendole diventare righe.
    // Come numero colonne inviamo 1, perchè un vettore possiede una sola riga che diventa colonna
    allocateMatrix(&transpose, size, 1); 
    
    for (int i = 0; i < size; ++i) {
        transpose[i][0] = vector[i];
    }

    return transpose;
}