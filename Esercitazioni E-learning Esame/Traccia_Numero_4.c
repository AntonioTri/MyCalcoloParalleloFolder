/*

Implementare un programma parallelo per l’ambiente multicore con np unità processanti impieghi la libreria OpenMP. 
Il programma deve essere organizzato come segue:

1)il core master deve leggere una matrice di
dimensione N×N

2) quindi i core devo collaborare per ricopiare in parallelo gli elementi della diagonale principale in un vettore di lunghezza N. 

3)Infine, i core devono effettuare la somma degli elementi di tale vettore in parallelo.



*/



#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>


#define N 10


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

// Popolamento della matrice in parallelo
void populateMatrix(int **matrix, int randModule){

    #pragma omp parallel for
    for (int i = 0; i < N; i++)
        for(int j=0; j < N; j++)
            matrix[i][j] = rand() % randModule + 1;
    
}

// Funzione che estrae la diagonale della matrice
void extractDiagonalFromMatrix(int **matrix, int *diagonal){

    #pragma omp parallel for
    for (int i = 0; i < N; i++)
        diagonal[i] = matrix[i][i];
    
}


// Funzione che calcla la somma del vettore in parallelo usando la seconda strategia
int calculateSumParallel(int *vector){

    int SUM = 0;

    // La reduction e' il metodo di calcolo somma usato nella seconda strategia
    #pragma omp parallel for reduction(+:SUM)
    for (int i = 0; i < N; i++)
        SUM += vector[i];
    

    return SUM;
}

// Metodo che stampa la matrice
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
void freeStuffs(int **matrix, int *diagonal) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(diagonal);
}


int main(){

    srand(time(NULL));

    // Variabili
    int **matrix;
    int *diagonal;
    int diagonalTotalSum;

    // Definizione della matrice N x N
    allocateMatrix(&matrix);
    populateMatrix(matrix, 10);
    
    // Definizione del vettore che deve contenere la diagonale della matrice
    allocateVector(&diagonal);

    // Fase parallela di estrazione dat dalla diagonale principale
    extractDiagonalFromMatrix(matrix, diagonal);

    // Fase parallela per la solla degli elementi del vettore in parallelo
    int sum = calculateSumParallel(diagonal);

    // Mostriamo gli elementi ed ol risultato della operazione parallela 
    printMatrix(matrix, "Matrice A:");
    printVector(diagonal, "Diagonale della matrice A:");
    printf("\nLa somma degli elementi della diagonale della matrice e' : %d\n", sum);

    // Liberiamo la memoria
    freeStuffs(matrix, diagonal);
    
    return 0;
}













































































// void allocationMatrix(int***, int, int);
// void fillMatrix(int***, int, int);
// void printMatrix(int**, int, int, char[]);
// void deallocationMatrix(int***, int);
// void printVector(int* vector, int N, char name[]);


// int main()
// {
//     int **Matrice;
//     int *Vettore;
//     int N;
//     int np;

//     srand(time(NULL));

//     printf("Inserisci N:");
//     scanf("%d", &N);
    
//     printf("inserisci numero core:");
//     scanf("%d", &np);
   

//    //Punto 1
//    #pragma omp master
//    {
//     allocationMatrix(&Matrice, N, N);
//     fillMatrix(&Matrice, N, N);
//     printMatrix(Matrice, N, N, "Matrice");
//    }


//     // Inizializzazione del vettore per la diagonale principale
//     Vettore = (int*) malloc(N * sizeof(int));

//     //Punto 2
//     // Copia degli elementi della diagonale principale in parallelo
//     #pragma omp parallel for shared(Matrice, Vettore) num_threads(np)
//     for (int i = 0; i < N; i++) {
//         Vettore[i] = Matrice[i][i];
//     }

//     printVector(Vettore, N, "Vettore Diagonale");


//     //punto 3
//     int sum = 0;    
//     #pragma omp parallel for reduction(+:sum) num_threads(np)
//     for (int i = 0; i < N; i++) {
//         sum += Vettore[i];
//     }

//     // Stampa della somma
//     printf("Somma degli elementi della diagonale principale: %d\n", sum);

//     // Deallocazione della memoria
//     deallocationMatrix(&Matrice, N);
//     free(Vettore);

//     return 0;


// }




// void printVector(int* vector, int N, char name[]) {
//     printf("\n%s:\n", name);
//     for (int i = 0; i < N; i++) {
//         printf("%d\t", vector[i]);
//     }
//     printf("\n");
// }

// void allocationMatrix(int*** matrix, int N, int M) {
//     *matrix = (int**) calloc(N, sizeof(int*));
//     for (int i = 0; i < N; i++)
//         (*matrix)[i] = (int*) calloc(M, sizeof(int));
// }

// void fillMatrix(int*** matrix, int N, int M) {
//     for (int i = 0; i < N; i++)
//         for (int j = 0; j < M; j++)
//             (*matrix)[i][j] = rand() % 6;
// }

// void printMatrix(int** matrix, int N, int M, char name[]) {
//     printf("\n%s:\n", name);
//     for (int i = 0; i < N; i++) {
//         for (int j = 0; j < M; j++)
//             printf("%d    ", matrix[i][j]);
//         printf("\n");
//     }
// }

// void deallocationMatrix(int*** matrix, int N) {
//     for (int i = 0; i < N; i++)
//         free((*matrix)[i]);
//     free(*matrix);
// }
