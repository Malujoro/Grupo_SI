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
                int op2;

                printf("\nNúmero: ");
                scanf(" %d", &aux.numero);

                printf("\nTipo: ");
                printf("\n[0] - Simples");
                printf("\n[1] - Duplo");
                printf("\n[2] - Suíte");
                printf("\nOpção: ");
                // scanf(" %d", &aux.tipo);            
                // aux.tipo = op2;
                
                // Ideia - Caso escolha o quarto reservado, puxar a função de realizar reserva? ou então obter a data da reserva do cara
                // Ou melhor, deixa só podendo cadastrar status livre ou ocupado (ele se contradiz no próprio pdf)
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
                    printf("\nQuarto %d", vetor[i].numero);
                    printf("\nTipo %d", vetor[i].tipo);
                    printf("\nStatus %d", vetor[i].status);
                    printf("\nValor %.2f\n", vetor[i].valor);
                }

                fclose(arquivo);
        }
    }while(op != 0);
    return 0;
}