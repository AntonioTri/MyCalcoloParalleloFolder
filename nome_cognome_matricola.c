

/*
    Traccia

*/



#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

// Definizione di dimensioni standard per il programma
#define N 12
#define M 12

// Definizione delle funzioni usate
void populateMatrix(int **matrix, int row, int col, int randModule);
void populateVector(int *vector, int size, int randModule);
void allocateMatrix(int ***matrix, int rows, int cols);
void allocateVector(int **vector, int size);
void printMatrix(int** matrix, int rows, int cols, char name[]);
void printVector(int *vector, int size, char name[]);
void freeMatrix(int **matrix, int rows);


int main(){

    // Definizione di vettori e matrici
    int **matriceA;
    int **matriceB;
    int **matriceC;

    int *vectorA;
    int *vectorB;
    int *vectorC;

    // Definizione di variabili di supporto
    int np = omp_get_max_threads();
    // ...

    // Il master inizializza e popola i vettori e le matrici
    #pragma omp master
    {

        // Matrici
        allocateMatrix(&matriceA, N, M);
        populateMatrix(matriceA, N, M, 10);

        // Vettori
        allocateVector(&vectorA, N);
        populateVector(vectorA, N, 10);

        // Stampa delle matrici
        printMatrix(matriceA, N, M, "Matrice A");
        
        // Stampa dei vettori
        printVector(vectorA, N, "Vettore A");

    }

    //Variabile che conserva il tempo di inizio
    double startingTimer = omp_get_wtime();

    // Sezione parallela
    #pragma omp parallel 
    {
        
        // ... definizione e risoluzione del problema sulla traccia

    }

    // Variabile che conserva il tempo di fine
    double endingtimer = omp_get_wtime();

    // Facciamo stampare al master il tempo di esecuzione
    #pragma omp master 
    {
        double totalTime = startingTimer - endingtimer;
        printf("\n********************\n\nTempo finale: %.7f", totalTime);
    }




}


// Popolamento della matrice.
void populateMatrix(int **matrix, int row, int col, int randModule){
    for (int i = 0; i < row; i++)
        for(int j=0; j < col; j++)
            matrix[i][j] = rand() % randModule;
}

// Popolamento di un vettore
void populateVector(int *vector, int size, int randModule){
    for (int i = 0; i < size; i++)
        vector[i] = (rand() % randModule);
}

// Funzione per allocare la matrice
void allocateMatrix(int ***matrix, int rows, int cols) {
    *matrix = (int **) malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        (*matrix)[i] = (int*) malloc(cols * sizeof(int));
}

// Funzione per allocare il vettore
void allocateVector(int **vector, int size){
    *vector = (int*) malloc( size * sizeof(int));
}


// Per questa funzione basta inviare il nome della matrice.
// Non serve l'indirizzo '&'.
void printMatrix(int** matrix, int rows, int cols, char name[]) {
    printf("\n%s:\n", name);
    // Ciclo le righe
    for (int i = 0; i < rows; i++){
        // Ciclo le colonne
        for (int j = 0; j < cols; j++)
            printf("%d\t", matrix[i][j]);
        // Dopo ogni fine colonna vado a capo
        printf("\n");
    }
}

// Funzione per stampare il vettore
void printVector(int *vector, int size, char name[]) {
    printf("\n%s:\n", name);
    for (int i = 0; i < size; i++) {
        printf("%d\t", vector[i]);
    }
    printf("\n");
}

// Funzione per liberare la memoria di una matrice
void freeMatrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) 
        free(matrix[i]);

    free(matrix);
}