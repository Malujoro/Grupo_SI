#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int numero;
    int tipo; // 0 - Simples, 1 - Duplos, 2 - Suíte
    int status; // 0 - Livre, 1 - Ocupado, 2 - Reservados
    float valor;
} Quarto;

int main()
{
    Quarto aux;
    int op;

    do
    {
        printf("\n[1] - Cadastrar");
        printf("\n[2] - Exibir todos");
        printf("\nOpção: ");
        scanf(" %d", &op);

        switch(op)
        {
            case 1:
                printf("\nNúmero: ");
                scanf(" %d", &aux.numero);

                printf("\nTipo: ");
                printf("\n[0] - Simples");
                printf("\n[1] - Duplo");
                printf("\n[2] - Suíte");
                printf("\nOpção: ");
                scanf(" %d", &aux.tipo);            
                // aux.tipo = op2;
                
                printf("\nStatus");
                printf("\n[0] - Livre");
                printf("\n[1] - Ocupado");
                // printf("\n[3] - Reservado");
                printf("\nOpção: ");
                scanf(" %d", &aux.status);            
                // aux.status = op2;
                
                printf("\nValor: R$");
                scanf(" %f", &aux.valor);
                
                FILE *arquivo = fopen("quartos.bin", "ab");
                fwrite(&aux, sizeof(Quarto), 1, arquivo);
                fclose(arquivo);
            break;

            case 2:
                arquivo = fopen("quartos.bin", "rb");
                int tam;

                Quarto *vetor = (Quarto *) malloc(sizeof(Quarto));
                tam = 0;
                while(fread(&aux, sizeof(Quarto), 1, arquivo) == 1)
                {
                    vetor[tam] = aux;
                    (tam)++;
                    // Aumenta o tamanho do vetor dinamicamente
                    vetor = realloc(vetor, ((tam) + 1) * sizeof(Quarto));
                }

                for(int i = 0; i < tam; i++)
                {
                    printf("\n----------Quarto----------");
                    printf("\nQuarto %d", vetor[i].numero);

                    if(vetor[i].tipo == 0)
                        printf("\nTipo Simples");
                    else if(vetor[i].tipo == 1)
                        printf("\nTipo Duplo");
                    else if(vetor[i].tipo == 2)
                        printf("\nTipo Suíte");

                    if(vetor[i].status == 0)
                        printf("\nStatus Livre");
                    else if(vetor[i].status == 1)
                        printf("\nStatus Ocupado");
                    else if(vetor[i].status == 2)
                        printf("\nStatus Reservado");

                    printf("\nValor %.2f\n", vetor[i].valor);
                }

                fclose(arquivo);
        }
    }while(op != 0);
    return 0;
}