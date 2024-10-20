/**
* ESERCITAZIONE: 08/06/2023
* 
* Implementare un programma parallelo per l'ambiente multicore con np
* unità processanti che impieghi la libreria OpenMP. Il programma deve
* essere organizzato come segue:
*
* 1. il core master deve leggere una matrice A di dimensione NxM
*
* 2. i core devono organizzarsi per estrarre ognuno una sottomatrice,
*    decomponendo la matrice A per blocchi riga, e costruendo la trasposta
*    di tale sottomatrice, in una matrice Bloc
*
* 3. infine i core devono collaborare per sommare le sotto matrici estratte Bloc 
*    in un'unica matrice finale C
*
* 4. il core master stampa la matrice risutato e il tempo d'esecuzione
*/


#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>


#define N 24
#define M 8


void allocMatrix(int ***matrix, int row, int col);
void populateMatrix(int ***matrix, int randModule, int row, int col);
void printMatrix(int **matrix, char name[], int row, int col);


int main(){

    srand(time(NULL));
    // Definiamo la matrice iniziale A
    int **matrixA;
    // Definiamo La matrice Bloc
    int **Bloc;
    // Definiamo la matrice finale C
    int **Ctot;
    // Otteniamo il numero di thread disponibili
    int numberOfThreads = omp_get_max_threads();
    // Variabile comune nel caso il numero di ricghe non sia divisibile per il numero di threads
    
    if (N % omp_get_max_threads() >0 ) 
    {   
        printf("Il numero di righe non e' divisivbile per il numero dei processori.\nPertanto non puo' essere eseguita la somma delle porzioni delle sottomatrici Bloc.\n");
        return 0;
    } else {
        printf("Numero di threads = %d.\nModulo N mosulo numero di threads = %d.\n",numberOfThreads, N % numberOfThreads);
    }


    #pragma omp master
    {

        //Allochiamo e popoliamo la matrice
        allocMatrix(&matrixA, N, M);
        populateMatrix(&matrixA, 10, N, M);
        //Mostriamo la matrice
        printMatrix(matrixA, "Matrice A", N, M);

        //Allochiamo Ctot
        allocMatrix(&Ctot, M, N /numberOfThreads);

    }



    #pragma omp parallel for
    for (int i = 0; i < N / numberOfThreads; i++)
        for(int j = 0; j < M; j++)
            Ctot[j][i] = 0;
    

    double startTimer = omp_get_wtime();

    #pragma omp parallel num_threads(numberOfThreads) shared(matrixA, Ctot) private(Bloc)
    {
        // Otteniamo il numero del thread
        int threadNumber = omp_get_thread_num();
        // Definiamo il numero di righe proprietarie di ogni thread
        int localRows = N / numberOfThreads;
        int resto = N % numberOfThreads;
        int startingRow = localRows * threadNumber;

        // Controllo per allocare

        // Conto delle righe da assegnare in più se c'è il resto
        if (threadNumber < resto){
            localRows += 1;
        } else {
            startingRow += resto;
        }
        
        // Allocazione della matrice Bloc TRASPOSTA
        allocMatrix(&Bloc, M, localRows);

        // Estraiamo i dati dalla matrice A e li inseriamo in Bloc
        for (int i = 0; i < localRows; i++)
            for(int j = 0; j < M; j++)
                Bloc[j][i] = matrixA[startingRow + i][j];



        // Mostriamo la matrice di ogni thread
        #pragma omp critical
        {
            for (int i = 0; i < localRows; i++)
                for(int j = 0; j < M; j++)
                    Ctot[j][i] += Bloc[j][i] ;
            // printMatrix(Bloc, "Matrice Bloc", M, localRows);
        }

    }

    double endTimer = omp_get_wtime();
    double totalTime = endTimer - startTimer;

    #pragma omp master 
    {
        printMatrix(Ctot, "Matrice Ctot", M, N / numberOfThreads);
        printf("\n********************\n\nTempo finale: %.7f", totalTime);
    }

}



// Funzione per allocare una matrice
void allocMatrix(int ***matrix, int row, int col) {
    *matrix = (int **)malloc(row * sizeof(int *));
    for (int i = 0; i < row; ++i) {
        (*matrix)[i] = (int *)malloc(col * sizeof(int));
    }
}

// Funzione per riempire la matrice con valori casuali
void populateMatrix(int ***matrix, int randModule, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            (*matrix)[i][j] = rand() % randModule;
        }
    }
}



void printMatrix(int **matrix, char name[], int row, int col) {
    
    printf("\n%s:\n", name);
    // Ciclo le righe
    for (int i = 0; i < row; i++){
        // Ciclo le colonne
        for (int j = 0; j < col; j++)
            printf("%d\t", matrix[i][j]);

        // Dopo ogni fine colonna vado a capo
        printf("\n");
    }
}