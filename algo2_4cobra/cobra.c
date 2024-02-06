#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define TAM 15
#define CARACTERE '.'
#define COMIDA '*'
#define CABECA '0'
#define CORPO 'o'

typedef struct
{
    int i;
    int j;
} Cobra;

// Função para limpar o Buffer (caracteres excedentes)
void limpaBuffer()
{
    while(getchar() != '\n');
}

// Função para ler apenas números inteiros
// Recebe o Texto a ser exibido. Retorna o Inteiro lido
int leiaInt(char *texto)
{
    int num, retorno;

    do
    {
        // Exibe a mensagem recebida
        printf("%s", texto);
        // Scanf retorna 1 caso a leitura tenha sido um sucesso
        retorno = scanf(" %d", &num);
        limpaBuffer();

        if(retorno == 0)
            printf("\nEntrada inválida!! Digite um número inteiro!\n");
    }while(retorno == 0);
    
    return num;
}

int menu()
{
    printf("\n-----SNAKE GAME-----");
    printf("\n[1] - Jogar");
    printf("\n[0] - Sair");
    int op = leiaInt("\nOpcao: ");
    return op;
}

void preencherMatriz(char **matriz)
{
    for(int i = 0; i < TAM; i++)
    {
        for(int j = 0; j < TAM; j++)
            matriz[i][j] = CARACTERE;
    }
}

void exibirMatriz(char **matriz)
{
    system("cls");
    for(int i = 0; i < TAM; i++)
    {
        for(int j = 0; j < TAM; j++)
            printf("%c ", matriz[i][j]);
        printf("\n");
    }
}

void geraAlimento(char **matriz, int *i, int *j)
{
    int auxi, auxj, invalido = 1;
    do
    {
        auxi = rand() % TAM;
        auxj = rand() % TAM;
        if(matriz[auxi][auxj] == CARACTERE)
            invalido = 0;
    }while(invalido);
    *i = auxi;
    *j = auxj;
    matriz[auxi][auxj] = COMIDA;
}

// Falta implementar:
// - Lógica para comer e crescer
//      - Fazer verificação se a casa tem comida (desenho de comida), e aí tam++ (em tese a movimentação funcionaria tranquilo)
// - Lógica para perder (bater na parede ou em si)
//      - Fazer verificação se o movimento não vai exceder as coordenadas da matriz (coordenada < 0 ou == TAM)
//      - Fazer verificação se a casa está livre (desenho de caractere)

void acima(char **matriz, int *tamCobra, Cobra *cobra)
{
    int i;
    matriz[cobra[0].i-1][cobra[0].j] = CABECA;
    for(i = 0; i < (*tamCobra)-1; i++)
    {
        cobra[i + 1] = cobra[i];
        matriz[cobra[i+1].i][cobra[i+1].j] = matriz[cobra[i].i][cobra[i].j];
    }
    matriz[cobra[i].i][cobra[i].j] = CARACTERE;
    cobra[0].i--;
}

void abaixo(char **matriz, int *tamCobra, Cobra *cobra)
{
    int i;
    matriz[cobra[0].i+1][cobra[0].j] = CABECA;
    for(i = 0; i < (*tamCobra)-1; i++)
    {
        cobra[i + 1] = cobra[i];
        matriz[cobra[i+1].i][cobra[i+1].j] = matriz[cobra[i].i][cobra[i].j];
    }
    matriz[cobra[i].i][cobra[i].j] = CARACTERE;
    cobra[0].i++;
}

void direita(char **matriz, int *tamCobra, Cobra *cobra)
{
    int i;
    matriz[cobra[0].i][cobra[0].j+1] = CABECA;
    for(i = 0; i < (*tamCobra)-1; i++)
    {
        cobra[i + 1] = cobra[i];
        matriz[cobra[i+1].i][cobra[i+1].j] = matriz[cobra[i].i][cobra[i].j];
    }
    matriz[cobra[i].i][cobra[i].j] = CARACTERE;
    cobra[0].j++;
}

void esquerda(char **matriz, int *tamCobra, Cobra *cobra)
{
    int i;
    matriz[cobra[0].i][cobra[0].j-1] = CABECA;
    for(i = 0; i < (*tamCobra)-1; i++)
    {
        cobra[i + 1] = cobra[i];
        matriz[cobra[i+1].i][cobra[i+1].j] = matriz[cobra[i].i][cobra[i].j];
    }
    matriz[cobra[i].i][cobra[i].j] = CARACTERE;
    cobra[0].j--;
}

void jogar(char **matriz)
{
    int icomida, jcomida, tamCobra = 1;
    Cobra cobra[TAM*TAM];
    char direcao;

    preencherMatriz(matriz);

    matriz[0][0] = CABECA;
    cobra[0].i = 0;
    cobra[0].j = 0;

    geraAlimento(matriz, &icomida, &jcomida);

    do
    {
        exibirMatriz(matriz);
        direcao = getch();
        printf("\nDirecao: %c\n", direcao);
        switch(direcao)
        {
            case 'w':
                acima(matriz, &tamCobra, cobra);
                break;
            
            case 'a':
                esquerda(matriz, &tamCobra, cobra);
                break;
                
            case 's':
                abaixo(matriz, &tamCobra, cobra);
                break;
            
            case 'd':
                direita(matriz, &tamCobra, cobra);
                break;
        }
    }while(direcao != 0);
}

int main()
{
    int op;

    char **matriz = (char **) malloc(TAM * sizeof(char*));
    for(int i = 0; i < TAM; i++)
        matriz[i] = (char *) malloc(TAM * sizeof(char));

    srand(time(NULL));

    do
    {
        op = menu();
        switch(op)
        {
            case 1:
                jogar(matriz);
                break;
            case 0:
                printf("\nSaindo...\n");
                break;
            default:
                printf("\nOpcao invalida!!\n");
        }
    }while(op != 0);

    for(int i = 0; i < TAM; i++)
        free(matriz[i]);
    free(matriz);
    return 0;
}