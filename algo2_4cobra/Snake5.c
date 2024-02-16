#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>

#define TAM 15
#define CARACTERE ' '
#define COMIDA '*'
#define CABECA '0'
#define CORPO 'o'
#define CERCA '\xdb'
#define ARQUIVO "pontuacao.txt"
#define BRANCO 0
#define AZUL FOREGROUND_BLUE
#define VERDE FOREGROUND_GREEN
#define CIANO 3
#define ROSA 5
#define AMARELO 6
#define VERMELHO FOREGROUND_RED
// pintarLetra(7); // Branco
#define CINZA 8
// pintarLetra(9); // Roxo

// \xC9 ┌ \xCD ─ \xBB ┐ \xBA │
// \xC8 └ \xCA ┴ \xBC ┘ \xC5 ┼

HANDLE tela;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD pinturaPadrao;

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

FILE *abrirArquivo(char *nome, char *modo)
{
    FILE *arquivo = fopen(nome, modo);
    return arquivo;
}

///////////////////////     FUNÇÕES DO JOGO EM SI     ////////////////////////////////////

void pintarLetra(int Cor)
{
    if(Cor == BRANCO)
        SetConsoleTextAttribute(tela, pinturaPadrao | FOREGROUND_INTENSITY);
    else
        SetConsoleTextAttribute(tela, Cor | FOREGROUND_INTENSITY);
}

/////  FUNÇÕES PARA GERAR UM MENU INTERATIVO  /////

void moveXY(int x, int y, char t[10])
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};
    SetConsoleCursorPosition(hStdout, position);
    printf("%s", t);
}

void beep(int frequencia, int duracao) {
    Beep(frequencia, duracao);
}

int menuInterativo()
{
    char entrada;
    int posX = 3, posY = 4, op = 1;

    do
    {		
        system("cls");

        pintarLetra(VERDE);
        moveXY(posX+2, 1, "-----SNAKE GAME-----");
        pintarLetra(AZUL);
        moveXY(posX+3, 4, "Jogar");
        moveXY(posX+3, 7, "Exibir pontuacoes");
        moveXY(posX+3, 10, "Sair");
        pintarLetra(ROSA);
        printf("\n\n\n[W] - Opcao de cima");
        printf("\n[S] - Opcao de baixo");
        printf("\n[Enter] - Escolher opcao");
        moveXY(posX, posY, "->");

        entrada = toupper(getch());
		
        switch(entrada)
        {
            case 'W': // Tecla W para cima
                if (posY > 4)
                {
                    moveXY(posX, posY, "  ");
                    posY -= 3;
                    moveXY(posX, posY, "->");
                    beep(400, 100);
                    op--;
                }
                break;

            case 'S': // Tecla S para baixo
                if (posY < 10)
                {
                    moveXY(posX, posY, "  ");
                    posY += 3;
                    moveXY(posX, posY, "->");
                    beep(400, 100);
                    op++;
                }
                break;
        }
    } while(entrada != 13);
    pintarLetra(BRANCO);
    return op;
}

////////////////////////////////////////////////////////

void instrucoes()
{
    system("cls");
    pintarLetra(VERDE);
    printf("\n     Teclas do jogo ");
    printf("\n       \xC9\xCD\xCD\xCD\xBB");
    printf("\n       \xBA W \xBA");
    printf("\n   \xC9\xCD\xCD\xCD\xC5\xCD\xCD\xCD\xC5\xCD\xCD\xCD\xBB   \xC9\xCD\xCD\xCD\xBB");
    printf("\n   \xBA A \xBA S \xBA D \xBA   \xBA P \xBA");
    printf("\n   \xC8\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xCA\xCD\xCD\xCD\xBC   \xC8\xCD\xCD\xCD\xBC");

    pintarLetra(AZUL);
    printf("\n[W] - Andar pra cima");
    printf("\n[A] - Andar pra esquerda");
    printf("\n[S] - Andar pra baixo");
    printf("\n[D] - Andar pra direita");
    printf("\n[P] - Sair da partida");
        
    pintarLetra(BRANCO);
    printf("\n\nPressione qualquer tecla para continuar!\n");
    getch();
}

void vitoria()
{
    pintarLetra(VERDE);
    printf("\nParabens! \nVoce ganhou!!\n");
    pintarLetra(BRANCO);
    return;
}

void gameOver()
{
    pintarLetra(VERMELHO);
    printf("\nGame Over!\n");
    pintarLetra(BRANCO);
    return;
}

int calculaPontuacao(int tamCobra)
{
    return (tamCobra-1)*5;
}

void preencherMatriz(char **matriz)
{
    for(int i = 0; i < TAM; i++)
    {
        for(int j = 0; j < TAM; j++)
            matriz[i][j] = CARACTERE;
    }
}

void exibirMatriz(char **matriz, int tamCobra)
{
    system("cls");
    
    pintarLetra(CINZA);
    for(int i = 0; i < (TAM+1)*2; i++) 
        printf("%c", CERCA);     
    printf("\n");

    for(int i = 0; i < TAM; i++)
    {
        pintarLetra(CINZA);
        printf("%c", CERCA);
        for(int j = 0; j < TAM; j++)
        {
            if(matriz[i][j] == CORPO || matriz[i][j] == CABECA)
                pintarLetra(VERDE);
            else if(matriz[i][j] == COMIDA)
                pintarLetra(VERMELHO);
            printf("%c ", matriz[i][j]);
        }
        pintarLetra(CINZA);
        printf("%c\n", CERCA);
    }

    for(int i = 0; i < ((TAM+1)*2); i++)
        printf("%c", CERCA);
    pintarLetra(BRANCO);
    printf("\nPontuacao: %d\n", calculaPontuacao(tamCobra));
}

void atualizaPixel(char **matriz, int i, int j)
{
    HANDLE video = GetStdHandle(STD_OUTPUT_HANDLE);
    int i2, j2;

    i2 = i + 1;
    j2 = (j*2) + 1;
    COORD posicao = {j2, i2};

    SetConsoleCursorPosition(video, posicao);
    if(matriz[i][j] == CORPO || matriz[i][j] == CABECA)
        pintarLetra(VERDE);
    else if(matriz[i][j] == COMIDA)
        pintarLetra(VERMELHO);

    printf("%c", matriz[i][j]);
    pintarLetra(BRANCO);
}

void atualizaPonto(int tamCobra)
{
    HANDLE video = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD posicao = {11, TAM+2};

    SetConsoleCursorPosition(video, posicao);
    printf("%d", calculaPontuacao(tamCobra));
}

void geraAlimento(char **matriz, int tamCobra, int *iComida, int *jComida)
{
    int i, j, invalido = 1;
    if(tamCobra < TAM * TAM)
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
        atualizaPixel(matriz, i, j);
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

void moveCorpo(char **matriz, int *tamCobra, Cobra *cobra)
{
    for(int i = (*tamCobra)-1; i > 0; i--)
    {
        // Move o corpo, seguindo a cabeça
        cobra[i] = cobra[i-1];
        matriz[cobra[i].i][cobra[i].j] = CORPO;
        atualizaPixel(matriz, cobra[i].i, cobra[i].j);
    }
    // "Limpa" o espaço que a cauda passou
    if((*tamCobra) < TAM*TAM && matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] != COMIDA)
    {
        matriz[cobra[*tamCobra].i][cobra[*tamCobra].j] = CARACTERE;
        atualizaPixel(matriz, cobra[*tamCobra].i, cobra[*tamCobra].j);
    }
}

void acima(char **matriz, int *tamCobra, Cobra *cobra)
{
    // Verifica se a próxima casa é comida, para então crescer o tamanho da cobra
    if(matriz[cobra[0].i-1][cobra[0].j] == COMIDA)
        (*tamCobra)++;
    // Copia os dados da cauda para a nova cauda 
    cobra[*tamCobra] = cobra[(*tamCobra)-1];

    // Move a cabeça para cima
    matriz[cobra[0].i-1][cobra[0].j] = CABECA;
    atualizaPixel(matriz, cobra[0].i-1, cobra[0].j);
    moveCorpo(matriz, tamCobra, cobra);
    // Atualiza a coordenada da cabeça
    cobra[0].i--;
}

void abaixo(char **matriz, int *tamCobra, Cobra *cobra)
{
    // Verifica se a próxima casa é comida, para então crescer o tamanho da cobra
    if(matriz[cobra[0].i+1][cobra[0].j] == COMIDA)
        (*tamCobra)++;
    // Copia os dados da cauda para a nova cauda 
    cobra[*tamCobra] = cobra[(*tamCobra)-1];

    // Move a cabeça para baixo
    matriz[cobra[0].i+1][cobra[0].j] = CABECA;
    atualizaPixel(matriz, cobra[0].i+1, cobra[0].j);
    moveCorpo(matriz, tamCobra, cobra);

    // Atualiza a coordenada da cabeça
    cobra[0].i++;
}

void direita(char **matriz, int *tamCobra, Cobra *cobra)
{
    // Verifica se a próxima casa é comida, para então crescer o tamanho da cobra
    if(matriz[cobra[0].i][cobra[0].j+1] == COMIDA)
        (*tamCobra)++;
    // Copia os dados da cauda para a nova cauda 
    cobra[*tamCobra] = cobra[(*tamCobra)-1];

    // Move a cabeça para direita
    matriz[cobra[0].i][cobra[0].j+1] = CABECA;
    atualizaPixel(matriz, cobra[0].i, cobra[0].j+1);
    moveCorpo(matriz, tamCobra, cobra);

    // Atualiza a coordenada da cabeça
    cobra[0].j++;
}

void esquerda(char **matriz, int *tamCobra, Cobra *cobra)
{
    // Verifica se a próxima casa é comida, para então crescer o tamanho da cobra
    if(matriz[cobra[0].i][cobra[0].j-1] == COMIDA)
        (*tamCobra)++;
    // Copia os dados da cauda para a nova cauda 
    cobra[*tamCobra] = cobra[(*tamCobra)-1];

    // Move a cabeça para esquerda
    matriz[cobra[0].i][cobra[0].j-1] = CABECA;
    atualizaPixel(matriz, cobra[0].i, cobra[0].j-1);
    moveCorpo(matriz, tamCobra, cobra);

    // Atualiza a coordenada da cabeça
    cobra[0].j--;
}

int jogar(char **matriz, int *tamCobra)
{
    // Procedimentos iniciais do jogo
    *tamCobra = 1;
    int dificuldade = 100, tamAntigo, iComida, jComida;
    Cobra cobra[(TAM*TAM)+1];
    char direcao, aux;

    preencherMatriz(matriz);
    matriz[0][0] = CABECA;
    cobra[0].i = 0;
    cobra[0].j = 0;
    cobra[1] = cobra[0];

    exibirMatriz(matriz, *tamCobra);
    geraAlimento(matriz, *tamCobra, &iComida, &jComida);

    direcao = getch();
    do
    {
        tamAntigo = *tamCobra;

        if(_kbhit())
        {
            aux = tolower(_getch());
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
                acima(matriz, tamCobra, cobra);
                break;
            
            case 'a':
                esquerda(matriz, tamCobra, cobra);
                break;
                
            case 's':
                abaixo(matriz, tamCobra, cobra);
                break;
            
            case 'd':
                direita(matriz, tamCobra, cobra);
                break;
        }

        if((tamAntigo != *tamCobra) && dificuldade > 1)
            dificuldade -= 1;

        if(matriz[iComida][jComida] != COMIDA)
        {
            geraAlimento(matriz, *tamCobra, &iComida, &jComida);
            atualizaPonto(*tamCobra);
        }

        Sleep(dificuldade);
    }while(*tamCobra < TAM * TAM);
    return 1;
}

void exibirPontuacao()
{
    FILE *arquivo = abrirArquivo(ARQUIVO, "r");
    char nome[100];
    int pontuacao;

    system("cls");
    if(arquivo == NULL)
    {
        printf("\nNao existe nenhuma pontuacao registrada\n");
        printf("\nPressione qualquer tecla para voltar ao menu!\n");
        getch();
        return;
    }
    else
    {
        printf("\nPontuacoes registradas: \n");
        while(fscanf(arquivo, "%d %[^\n]s", &pontuacao, nome) == 2)
        {
            printf("\nJogador: %s", nome);
            printf("\nPontuacao: %d\n", pontuacao);
        }
    }
    printf("\nPressione qualquer tecla para voltar ao menu!\n");
    getch();
    fclose(arquivo);
}

void salvarPontuacao(int tamCobra)
{
    FILE *arquivo = abrirArquivo(ARQUIVO, "a");
    char nome[100];

    printf("\nDigite seu nome para salvar sua pontuacao: \n");
    scanf(" %[^\n]s", nome);
    limpaBuffer();
    fprintf(arquivo, "%d %s\n", calculaPontuacao(tamCobra), nome);

    printf("\nPressione qualquer tecla para voltar ao menu!\n");
    getch();
    fclose(arquivo);
}

int main()
{
    tela = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(tela, &consoleInfo);
    pinturaPadrao = consoleInfo.wAttributes;

    int op, tamCobra;

    char **matriz = (char **) malloc(TAM * sizeof(char*));
    for(int i = 0; i < TAM; i++)
        matriz[i] = (char *) malloc(TAM * sizeof(char));

    srand(time(NULL));

    do
    {
        op = menuInterativo();
        switch(op)
        {
            case 1:
                instrucoes();
                if(jogar(matriz, &tamCobra))
                {
                    exibirMatriz(matriz, tamCobra);
                    vitoria();
                }
                else
                {
                    exibirMatriz(matriz, tamCobra);
                    gameOver();
                }

                salvarPontuacao(tamCobra);
                break;
            case 2:
                exibirPontuacao();
                break;
            case 3:
                system("cls");
                printf("\nSaindo...\n");
                break;
            default:
                printf("\nOpcao invalida!!\n");
        }
    }while(op != 3);

    for(int i = 0; i < TAM; i++)
        free(matriz[i]);
    free(matriz);
    return 0;
}