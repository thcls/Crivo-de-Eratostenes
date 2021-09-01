#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "omp.h"

#define MAX 1000000000
//#define MAX 750000000
//#define MAX 500000000

int crivo_mp(){
    int *primos = (int*) malloc(MAX * sizeof(int));
    long int sqrtmax = (long int) sqrt(MAX);
    int contador = 0;
    primos[0] = primos[1] = 0;
    #pragma omp parellel num_threads(2)
    {
        #pragma omp parellel for
        for(long int i = 2; i < MAX; i++){
                primos[i] = 1;
        }
        #pragma omp parellel for schedule(dynamic)
        for(long int i = 2; i <= sqrtmax; i++){
            if(primos[i]==1){
                for(long int j = i*i;j < MAX;j += i){
                    primos[j] = 0;
                }
            }
        }
        #pragma omp parallel for reduction(+:contador)
        for(long int i = 0; i < MAX; i++){
            if(primos[i]==1){
                contador++;
            }
        }
    }
    free(primos);
    return contador;
}
int main()
{
    int repeticoes = 1;

    double mediatemp_p = 0;
    int contador = 0;

    for(int i = 0;i<repeticoes;i++){
        clock_t inicio = clock();
        contador = crivo_mp();
        clock_t fim = clock();
        mediatemp_p += fim-inicio;
        printf("Paralelo executou %d vezes.\n",i+1);
    }
    mediatemp_p /= CLOCKS_PER_SEC;
    mediatemp_p /= repeticoes;

    printf("\nNumero de primos: %d\n", contador);
    printf("Media do tempo paralelo: %lf\n", mediatemp_p);
}
