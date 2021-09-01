
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define TAM 500000000
//#define TAM 750000000
//#define TAM 1000000000

int main(int argc, char **argv) {

    MPI_Init(NULL, NULL);
    FILE *arquivo = fopen("Desempenho.txt", "a");

    //Inicialização
    double tempoInicial, tempoFinal;
    int idProc;
    int qtdProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &qtdProc);

    if(idProc == 0)
    	tempoInicial = MPI_Wtime();

    int qtdElementosProc = ceil(TAM/qtdProc);
    int raizTAM = floor(sqrt(TAM));

    int *blocoPrimos = malloc(sizeof(int)*(qtdElementosProc));
    int *primos = (int*)malloc(sizeof(int)*(TAM+1));

    //Inicializando vetor
    if(idProc == 0) {

        for(int i = 0; i <= TAM; i++)
            primos[i] = 1;

        //0 e 1 Não são primos
        primos[0] = 0;
        primos[1] = 0;

        //Marcando os primos de 2 até raiz(TAM)
        for(int p = 2; p*p <= raizTAM; p++) {

            if(primos[p] == 1) {

                for(int i = p*p; i <= TAM; i+=p)
                    primos[i] = 0;

            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&primos[0], raizTAM+1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(primos, qtdElementosProc, MPI_INT, &blocoPrimos[0], qtdElementosProc, MPI_INT, 0, MPI_COMM_WORLD);

    //Onde cada processo começa sua marcação
    int primeiro = idProc*qtdElementosProc + 1;

    for(int p = 2; p <= raizTAM; p++) {

        if(primos[p] == 1) {

            int n = primeiro;
            n = n + p/2;
            n = n - (n%p);

            if(n < primeiro)
                n = n + p;
            if(n == p)
                n = n + p;

            for(int i = n-primeiro+1; i < qtdElementosProc; i+=p)
                blocoPrimos[i] = 0;
        }
    }

    int subPrimos = 0;
    for(int i = 2; i < qtdElementosProc; i++) {

        if (blocoPrimos[i] == 1)
            subPrimos++;
    }
    printf("\nRANK %d: %d primos encontrados entre %d e %d\n\n", idProc, subPrimos, primeiro, primeiro+qtdElementosProc-1);

    int *totalPrimos = NULL;
    if(idProc == 0)
        totalPrimos = malloc(sizeof(int) * qtdProc);

    MPI_Gather(&subPrimos, 1, MPI_INT, totalPrimos, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(idProc == 0) {

        int total = 0;
        for(int i = 0; i < qtdProc; i++)
            total += totalPrimos[i];

        printf("Total de primos: %d\n", total);
        tempoFinal = MPI_Wtime();
    	printf("Tempo de Execucao: %lf\n", tempoFinal-tempoInicial);
        fprintf(arquivo, "%lf\n", tempoFinal-tempoInicial);
    }

    fclose(arquivo);
    MPI_Finalize();
    free(primos);
    return 0;
}
