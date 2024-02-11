#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define TAM 15
#define CARACTERE ' '
#define COMIDA '*'
#define CABECA '0'
#define CORPO 'o'
#define CERCA '\xdb'

// printf("\xC9 \xCD \xBB\n"); // ┌ ─ ┐
// printf("\xCA \xC5 \xBC\n"); // ┴ ┼ ┘
// printf("\xC8 \xBA"); // └ │

HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD pinturaPadrao;

void salvaPintura()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    pinturaPadrao = consoleInfo.wAttributes;
}

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
            printf("\nEntrada invalida!! Digite um numero inteiro!\n");
    }while(retorno == 0);
    
    return num;
}
///////////////////////     FUNÇÕES DO JOGO EM SI     ////////////////////////////////////

int menu()
{
    printf("\n-----SNAKE GAME-----");
    printf("\n[1] - Jogar");
    printf("\n[0] - Sair");
    int op = leiaInt("\nOpcao: ");
    return op;
}

void instrucoes()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD pinturaPadrao;

    // Salva as configurações de cor atuais
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    pinturaPadrao = consoleInfo.wAttributes;

    // Define a cor do texto
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    printf("\nTeclas do jogo: ");
    printf("\n    \xC9\xCD\xCD\xCD\xBB");
    printf("\n    \xBA W \xBA");
    printf("\n\xC9\xCD\xCD\xCD\xC5\xCD\xCD\xCD\xC5\xCD\xCD\xCD\xBB   \xC9\xCD\xCD\xCD\xBB");
    printf("\n\xBA A \xBA S \xBA D \xBA   \xBA P \xBA");
    printf("\n\xC8\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xBC   \xC8\xCD\xCD\xCD\xBC");

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    printf("\n[W] - Andar pra cima");
    printf("\n[A] - Andar pra esquerda");
    printf("\n[S] - Andar pra baixo");
    printf("\n[D] - Andar pra direita");
    printf("\n[P] - Sair da partida");
        
    SetConsoleTextAttribute(hConsole, pinturaPadrao);
        
    printf("\n\nPressione qualquer tecla para continuar!\n");
    getch();
}

void vitoria()
{
    printf("\nParabens! \nVoce ganhou!!\n");
    return;
}

void gameOver()
{
    printf("\nGame Over!\n");
    return;
}

void preencherMatriz(char **matriz)
{
    for(int i = 0; i < TAM; i++)
    {
        for(int j = 0; j < TAM; j++)
            matriz[i][j] = CARACTERE;
    }
}

void exibirMatriz(char **matriz, int *tamCobra)
{
    system("cls");
    
    for(int i = 0; i < (TAM+1)*2; i++) 
        printf("%c", CERCA);     
    printf("\n");

    for(int i = 0; i < TAM; i++)
    {
        printf("%c", CERCA);
        for(int j = 0; j < TAM; j++)
            printf("%c ", matriz[i][j]); 
        printf("%c\n", CERCA);
    }

    for(int i = 0; i < ((TAM+1)*2); i++)
        printf("%c", CERCA);
    printf("\nPontuacao: %d\n", ((*tamCobra)-1)*5);
}

void geraAlimento(char **matriz, int *tamCobra, int *iComida, int *jComida)
{
    int i, j, invalido = 1;
    if((*tamCobra) < TAM * TAM)
    {
        do
        {
            i = rand() % TAM;
            j = rand() % TAM;
            if(matriz[i][j] == CARACTERE)
                invalido = 0;
        }while(invalido);
        matriz[i][j] = COMIDA;
        *iComida = i;
        *jComida = j;
    }
}

int morte(char **matriz, Cobra *cobra, char direcao)
{
    if((direcao == 'w' && cobra[0].i-1 < 0) || (direcao == 's' && cobra[0].i+1 >= TAM) ||
       (direcao == 'a' && cobra[0].j-1 < 0) || (direcao == 'd' && cobra[0].j+1 >= TAM))
       return 1;
    
    if((direcao == 'w' && matriz[cobra[0].i-1][cobra[0].j] == CORPO) ||
       (direcao == 's' && matriz[cobra[0].i+1][cobra[0].j] == CORPO) ||
       (direcao == 'a' && matriz[cobra[0].i][cobra[0].j-1] == CORPO) ||
       (direcao == 'd' && matriz[cobra[0].i][cobra[0].j+1] == CORPO))
        return 1;
    
    return 0;
}

void acima(char **matriz, int *tamCobra, Cobra *cobra)
{
    int i;

    // Verifica se a próxima casa é comida, para então crescer o tamanho da cobra
    if(matriz[cobra[0].i-1][cobra[0].j] == COMIDA)
        (*tamCobra)++;
    // Copia os dados da cauda para a nova cauda 
    cobra[*tamCobra] = cobra[(*tamCobra)-1];

    // Move a cabeça para cima
    matriz[cobra[0].i-1][cobra[0].j] = CABECA;
    for(i = (*tamCobra)-1; i > 0; i--)
    {
        // Move o corpo, seguindo a cabeça
        cobra[i] = cobra[i-1];
        matriz[cobra[i].i][cobra[i].j] = CORPO;
    }
    // "Limpa" o espaço que a cauda passou
    if((*tamCobra) < TAM*TAM && matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] != COMIDA)
        matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] = CARACTERE;
    // Atualiza a coordenada da cabeça
    cobra[0].i--;
}

void abaixo(char **matriz, int *tamCobra, Cobra *cobra)
{
    int i;

    // Verifica se a próxima casa é comida, para então crescer o tamanho da cobra
    if(matriz[cobra[0].i+1][cobra[0].j] == COMIDA)
        (*tamCobra)++;
    // Copia os dados da cauda para a nova cauda 
    cobra[*tamCobra] = cobra[(*tamCobra)-1];

    // Move a cabeça para baixo
    matriz[cobra[0].i+1][cobra[0].j] = CABECA;
    for(i = (*tamCobra)-1; i > 0; i--)
    {
        // Move o corpo, seguindo a cabeça
        cobra[i] = cobra[i-1];
        matriz[cobra[i].i][cobra[i].j] = CORPO;
    }
    // "Limpa" o espaço que a cauda passou
    if((*tamCobra) < TAM*TAM && matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] != COMIDA)
        matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] = CARACTERE;
    // Atualiza a coordenada da cabeça
    cobra[0].i++;
}

void direita(char **matriz, int *tamCobra, Cobra *cobra)
{
    int i;

    // Verifica se a próxima casa é comida, para então crescer o tamanho da cobra
    if(matriz[cobra[0].i][cobra[0].j+1] == COMIDA)
        (*tamCobra)++;
    // Copia os dados da cauda para a nova cauda 
    cobra[*tamCobra] = cobra[(*tamCobra)-1];

    // Move a cabeça para direita
    matriz[cobra[0].i][cobra[0].j+1] = CABECA;
    for(i = (*tamCobra)-1; i > 0; i--)
    {
        // Move o corpo, seguindo a cabeça
        cobra[i] = cobra[i-1];
        matriz[cobra[i].i][cobra[i].j] = CORPO;
    }
    // "Limpa" o espaço que a cauda passou
    if((*tamCobra) < TAM*TAM && matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] != COMIDA)
        matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] = CARACTERE;
    // Atualiza a coordenada da cabeça
    cobra[0].j++;
}

void esquerda(char **matriz, int *tamCobra, Cobra *cobra)
{
    int i;

    // Verifica se a próxima casa é comida, para então crescer o tamanho da cobra
    if(matriz[cobra[0].i][cobra[0].j-1] == COMIDA)
        (*tamCobra)++;
    // Copia os dados da cauda para a nova cauda 
    cobra[*tamCobra] = cobra[(*tamCobra)-1];

    // Move a cabeça para esquerda
    matriz[cobra[0].i][cobra[0].j-1] = CABECA;
    for(i = (*tamCobra)-1; i > 0; i--)
    {
        // Move o corpo, seguindo a cabeça
        cobra[i] = cobra[i-1];
        matriz[cobra[i].i][cobra[i].j] = CORPO;
    }
    // "Limpa" o espaço que a cauda passou
    if((*tamCobra) < TAM*TAM && matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] != COMIDA)
        matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] = CARACTERE;
    // Atualiza a coordenada da cabeça
    cobra[0].j--;
}

int jogar(char **matriz)
{
    // Procedimentos iniciais do jogo
    int tamCobra = 1, dificuldade = 100, tamAntigo, iComida, jComida;
    Cobra cobra[(TAM*TAM)+1];
    char direcao, aux;

    preencherMatriz(matriz);
    matriz[0][0] = CABECA;
    cobra[0].i = 0;
    cobra[0].j = 0;
    cobra[1] = cobra[0];

    geraAlimento(matriz, &tamCobra, &iComida, &jComida);
    exibirMatriz(matriz, &tamCobra);

    direcao = getch();
    do
    {
        tamAntigo = tamCobra;
        exibirMatriz(matriz, &tamCobra);
        
        if(_kbhit())
        {
            aux = _getch();
            switch(aux)
            {
                case 'w':
                    direcao = (direcao == 's') ? 's' : 'w';
                    break;
                
                case 'a':
                    direcao = (direcao == 'd') ? 'd' : 'a';
                    break;
                    
                case 's':
                    direcao = (direcao == 'w') ? 'w' : 's';
                    break;
                
                case 'd':
                    direcao = (direcao == 'a') ? 'a' : 'd';
                    break;

                case 'p':
                    return 0;
            }
        }
        if(morte(matriz, cobra, direcao))
            return 0;

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

        if((tamAntigo != tamCobra) && dificuldade > 30)
            dificuldade -= 10;
        else if((tamAntigo != tamCobra) && dificuldade > 1)
            dificuldade -= 1;
        if(matriz[iComida][jComida] != COMIDA)
            geraAlimento(matriz, &tamCobra, &iComida, &jComida);
        Sleep(dificuldade);
    }while(tamCobra < TAM * TAM);
    exibirMatriz(matriz, &tamCobra);
    return 1;
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
                instrucoes();
                if(jogar(matriz))
                    vitoria();
                else
                    gameOver();

                printf("\nPressione qualquer tecla para voltar ao menu!\n");
                getch();
                system("cls");
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
