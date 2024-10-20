/*
  //Slide 28 Laboratorio 10
Implementare un programma parallelo per l’ambiente multicore con npunità processanti che impieghi la libreria OpenMP. Il programma deve
essere organizzato come segue:

1)il core master deve generare una matrice B di dimensione NxN e due
vettori a, b di lunghezza N

2)i core devono collaborare per costruire, in parallelo, una nuova matrice A
ottenuta sommando alla diagonale principale della matrice B il vettore b

3)quindi, i core devono collaborare per calcolare in parallelo il prodotto tra
la nuova matrice A ed il vettore a, distribuendo il lavoro per colonne


4)infine, il core master stampa il risultato finale ed il tempo d’esecuzione


*/

//Solo dio saprà mettere fine a questa sofferenza


#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>


#define N 8

// Popolamento della matrice
void populateMatrix(int **matrix, int randModule){

    for (int i = 0; i < N; i++)
        for(int j=0; j < N; j++)
            matrix[i][j] = rand() % randModule + 1;
}

// Questa funzione inizializza un vettore
void populateVector(int *vector, int randModule){
    for (int i = 0; i < N; i++)
        vector[i] = (rand() % randModule) + 1;
}

// Funzione per allocare la matrice
void allocateMatrix(int ***matrix){

    *matrix = (int **) malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
       (*matrix)[i] = (int*) malloc( N * sizeof(int));

}

// Funzione per allocare il vettore
void allocateVector(int **vector){
    *vector = (int*) malloc( N * sizeof(int));
}


void printMatrix(int** matrix, char name[]) {
    
    printf("\n%s:\n", name);
    // Ciclo le righe
    for (int i = 0; i < N; i++){
        // Ciclo le colonne
        for (int j = 0; j < N; j++)
            printf("%d\t", matrix[i][j]);
        // Dopo ogni fine colonna vado a capo
        printf("\n");
    }
}

// Funzione per stampare il vettore
void printVector(int *vector, char name[]) {
    printf("\n%s:\n", name);
    for (int i = 0; i < N; i++) {
        printf("%d\t", vector[i]);
    }
    printf("\n");
}

// Funzione per liberare la memoria una volta concluso il programma
void freeMemories(int **matrix, int *diagonal) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(diagonal);
}



// Questa funzione genera una nuova matrice A s
// sommando alla diagonale di una matrice B un vettore C
int **retrieveMatrix(int **matrix, int *vector){

    // Definiamo la nuova matrice
    int **newMatrix;
    allocateMatrix(&newMatrix);
    
    // In paralleo andiamo a definire la nuova diagonale di A
    #pragma omp parallel for
    for (int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            newMatrix[i][j] = (i == j ? matrix[i][j] + vector[i] : matrix[i][j]);


    // Ritorno la nuova matrice cosi' definita
    return newMatrix;

}



// Questa funzione calcola il prodotto tra matrice e vettore 
// sfruttando la stretegia si spezzettare per colonne
int *productMatrixVector(int **matrix, int *vector){

    // Allochiamo il nuovo vettore
    int *result, i, j;
    allocateVector(&result);

    // Eseguiamo il prodotto in parallelo
    #pragma omp parallel for shared(matrix, vector, result) private(i,j)
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            #pragma omp atomic
            result[i] += matrix[i][j] + vector[j]; 
        }
    }
    
    // ritorniamo il risultato
    return result;

}




int main(){

    srand(time(NULL));
    // Definiamo la matrice B NxN,
    int **matrixB;
    // Definiamo i due vettori a e b
    int *vectorA;
    int *vectorB;

    #pragma omp master 
    {     
        // Allochiamo memorie e popoliamo gli elementi
        allocateMatrix(&matrixB);
        populateMatrix(matrixB, 20);

        allocateVector(&vectorA);
        allocateVector(&vectorB);
        populateVector(vectorA, 10);
        populateVector(vectorB, 10);
    }

    // Otteniamo la matrice A sommando alla diagonale della matrice B il vettore b;
    int **matrixA = retrieveMatrix(matrixB, vectorB);

    // Mostriamo  le matrici ed i vettori cosi' ottenuti ed utilizzati
    printMatrix(matrixB, "Matrice B");
    printVector(vectorB, "Vettore b");
    printMatrix(matrixA, "Matrice A ottenuta sommando alla Diagonale della matrice B il vettore b");

    //Calcoliamo il prodotto matrice vettore tra A ed il vettore a
    int *result = productMatrixVector(matrixA, vectorA);

    // Mostriamo il risultato
    printVector(result, "Prodotto tra A ed il vettore a");

    // Liberiamo le memorie
    freeMemories(matrixA, vectorA);
    freeMemories(matrixB, vectorB);
    free(result);


}







































































































// #define N 5
// void alloc_matrix(int *** matrix, int row, int col);
// void generaVettore(int **vettore, int size);
// void riempi_matrice(int ***matrice);
// void StampaMatrice(int*** matrix, char name[]);
// void StampaVettore( int* vector, int n, char name[] );


// int main()
// {

//     int **Matrice_A, **Matrice_B;
//     int* Vettore_A, *Vettore_B;
//     int* Risultato;
//     int i, j;

//     double tempo_inizio, tempo_fine;
//     srand(time(NULL));
//     tempo_inizio = omp_get_wtime();

//     //Parte 1
//     #pragma omp master
//     {
//         alloc_matrix(&Matrice_B, N, N);
//         riempi_matrice(&Matrice_B);
//         generaVettore(&Vettore_A, N);
//         generaVettore(&Vettore_B, N);
//     }
    
//     StampaMatrice(&Matrice_B, "Matrice");
//     StampaVettore(Vettore_A, N, "vettore a");
//     StampaVettore(Vettore_B, N, "vettore B");

//     alloc_matrix(&Matrice_A, N, N);


//     //Punto 2
//     #pragma omp parallel for shared(Matrice_B, Vettore_B, Matrice_A) private(i,j)
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < N; j++) {
//         if (i == j) {
//             Matrice_A[i][j] = Matrice_B[i][j] + Vettore_B[i]; // Accedere a Vettore_B[i]
//         } else {
//             Matrice_A[i][j] = Matrice_B[i][j];
//         }
//         }   
//     }
//     StampaMatrice(&Matrice_A, "Matrice A");


//     //Punto 3
//     Risultato = (int *)malloc(N * sizeof(int));
//     #pragma omp parallel for shared(Matrice_A, Vettore_A, Risultato) private(i,j)
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < N; j++) {
//             #pragma omp atomic
//             Risultato[i] += Matrice_A[i][j] * Vettore_A[j];
//         }
//     }

//     StampaVettore(Risultato, N, "Risultato");
//     tempo_fine = omp_get_wtime();

//     //Punto 4
//     #pragma omp master
//     {
//         printf("Tempo di esecuzione: %f", tempo_fine - tempo_inizio);
//     }


//     for (int i = 0; i < N; i++) {
//         free(Matrice_B[i]);
//         free(Matrice_A[i]);
//     }
//     free(Matrice_B);
//     free(Matrice_A);
//     free(Vettore_A);
//     free(Vettore_B);
//     free(Risultato);

//     return 0;
// }


// void alloc_matrix(int ***matrix, int row, int col) {
//     *matrix = malloc(row * sizeof(int *));
//     for (int i = 0; i < row; ++i) {
//         (*matrix)[i] = malloc(col * sizeof(int));
//     }
// }




// void riempi_matrice(int ***matrix) {
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < N; j++) {
//             (*matrix)[i][j] = rand() % 2;
//         }
//     }
// }


// void StampaMatrice(int ***matrix, char name[]) {
//     printf("\n%s:\n", name);
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < N; j++) {
//             printf("%d    ", (*matrix)[i][j]);
//         }
//         printf("\n");
//     }
// }


// void generaVettore(int **vettore, int size)
// {
//     *vettore = (int *)malloc(size * sizeof(int));

//     for(size_t i = 0; i < size; i++)
//     {
//         (*vettore)[i] = rand() %10;
//     }
// }


// //Stampa vettore
// void StampaVettore( int* vector, int n, char name[] ) {
//     printf( "\n%s: ", name );
//     for ( int i = 0; i < n; i++ )
//         printf( "%d ", vector[i] );
//     printf( "\n" );
// }
