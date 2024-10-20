/*

Implementazione di un algoritmo parallelo (np core)
per il calcolo degli elementi di un vettore c, i cui valori
sono ottenuti moltiplicando l’identificativo del core per
gli elementi di un vettore a di dimensione N, in
ambiente openMP 

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


// Dimensione del vettore
#define N 8

// Funzione che alloca la memoria
void allocateMemory(int **vector){
    *vector = (int *) malloc( N * sizeof(int));

}

// Funzione per stampare il vettore
void printVector(int *vector, char name[]) {
    printf("\n%s:\n", name);
    for (int i = 0; i < N; i++) {
        printf("%d\t", vector[i]);
    }
    printf("\n");
}

// Questa funzione inizializza un vettore IN PARALLELO
void inizializeVector(int *vector, int randModule){
    #pragma omp parallel for
    for (int i = 0; i < N; i++)
        vector[i] = (rand() % randModule) + 1;
}

// Funzione parallela per definire il vettore C secondo la traccia
void defineVectorC(int *vector_c, int *vector_a){

    #pragma omp parallel for
    for (int i = 0; i < N; i++)
        vector_c[i] = vector_a[i] * omp_get_thread_num();

}

int main(){


    int *vector_a;
    int *vector_c;
    int alpha;

    // Allocazione della memoria per i vettori 'a' e 'c'
    allocateMemory(&vector_a);
    allocateMemory(&vector_c);

    // Definizione del seed
    srand(time(NULL));

    // Generiamo i valori per a in parallelo
    inizializeVector(vector_a, 10);

    // Definiamo il vettore c secondo la traccia
    defineVectorC(vector_c, vector_a);

    // Stampiamo il vettore A
    printVector(vector_a, "Vettore A:");
    
    // Stampiamo il vettore C
    printVector(vector_c, "Vettore c[i] =  a[i] * num_thread():");

    // Liberiamo la memoria
    free(vector_a);
    free(vector_c);


}














































// void printArray(int * array, int size);
// void fillArray(int * array, int size, int max_element);


// #define N 10 //size vettore

// int main() {
//     int *a, *c;
//     int np;


//     printf("Numero di core:");
//     scanf("%d", &np);

//     printf("\nCore scelti %d\n", np);


//     // Allocazione dinamica dei vettori a e c
//     a = (int *)malloc(N * sizeof(int));
//     c = (int *)malloc(N * sizeof(int));
    
//     // Inizializzazione del generatore di numeri casuali
//     srand(time(NULL));
    
//     // Riempimento del vettore a con numeri casuali
//     fillArray(a, N, 10);  // Range massimo dei numeri casuali: 0-99
    
//     // Utilizziamo OpenMP per parallelizzare il calcolo
//     #pragma omp parallel for num_threads(np)
//     for (int i = 0; i < N; i++) {
//         int core_id = omp_get_thread_num();  // Ottieni l'identificativo del core
//         c[i] = core_id * a[i];  // Calcola c[i] come identificativo del core * a[i]
//     }
    
//     // Stampiamo i vettori a e c risultanti
//     printf("Vettore a:\n");
//     printArray(a, N);
    
//     printf("\nVettore c:\n");
//     printArray(c, N);
    
//     // Liberiamo la memoria allocata
//     free(a);
//     free(c);
    
//     return 0;
// }

// void printArray(int * array, int size) {
//     for (int i = 0; i < size; ++i) {
//         printf("%d ", array[i]);
//     }
//     printf("\n");
// }


// //max element lo diamo noi in input
// void fillArray(int * array, int size, int max_element) {
//     for (int i = 0; i < size; ++i) {
//         array[i] = rand() % max_element;
//     }
// }
