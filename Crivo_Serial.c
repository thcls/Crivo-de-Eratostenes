#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX 1000000000
//#define MAX 750000000
//#define MAX 500000000

int crivo_serial(){
    int *primos = (int*) malloc(MAX * sizeof(int));
    long int sqrtmax = (long int) sqrt(MAX);
    int contador = 0;
        for(long int i = 2; i < MAX; i++){
                primos[i] = 1;
        }
        primos[0] = primos[1] = 0;
        for(long int i = 2; i <= sqrtmax; i++){
            if(primos[i]==1){
                for(long int j = i*i;j < MAX;j += i){
                    primos[j] = 0;
                }
            }
        }
        for(long int i = 0; i < MAX; i++){
            if(primos[i]==1){
                contador++;
            }
        }
    free(primos);
    return contador;
}

int main()
{
    int repeticoes = 100;

    double mediatemp_s = 0;
    int contador = 0;

    for(int i = 0;i<repeticoes;i++){
        clock_t inicio = clock();
        contador = crivo_serial();
        clock_t fim = clock();
        mediatemp_s += fim-inicio;
        printf("Serial executou %d vezes.\n",i+1);

    }
    mediatemp_s /= CLOCKS_PER_SEC;
    mediatemp_s /= repeticoes;

    printf("\nNumero de primos: %d\n", contador);
    printf("Media do tempo serial: %lf\n", mediatemp_s);
}
