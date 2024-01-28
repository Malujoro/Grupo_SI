#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQQUARTO "quartos.bin"

// Tipos dos quartos
#define SIMPLES 0
#define DUPLO 1
#define SUITE 2

// Status dos quartos
#define LIVRE 0
#define OCUPADO 1
#define RESERVADO 2

typedef struct
{
    int numero;
    int tipo;
    int status;
    float preco;
} Quarto;

/////////////////////////////////  OUTROS  /////////////////////////////////////////////

// Função para limpar o Buffer (caracteres excedentes)
void limpaBuffer()
{
    while(getchar() != '\n');
}
//==================================================================

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
//=================================================================={

// Função para verificar se uma string é composta apenas por números
// Recebe a String a ser verificada e o seu tamanho
int verificaInt(char *string, int tam)
{
    for(int i = 0; i < tam; i++)
    {
        if(string[i] - '0' < 0 || string[i] - '0' > 9)
            return 0;
    }
    return 1;
}
//==================================================================

// Função para verificar se uma string é composta apenas por float
// Recebe a String a ser verificada e o seu tamanho
int verificaFloat(char *string, int tam)
{
    int i, ponto = 0;

    // Conta a quantidade de pontos ou vírgulas do número
    for(i = 0; i < tam; i++)
    {
        if(string[i] == '.' || string[i] == ',')
        {
            ponto++;
            string[i] = '.';
        }

        // Caso exista mais que 1 ponto, retorna falso
        if(ponto > 1)
            return 0;
    }

    int tam1 = 0;

    // Conta a quantidade de números antes do ponto
    for(tam1 = 0; tam1 < tam && string[tam1] != '.'; tam1++);

    // Separa em duas strings os números, para conseguir verificar se são inteiros
    int tam2 = tam-tam1-1;
    char string1[tam1], string2[tam2];
    strncpy(string1, string, tam1);

    // Verifica se os números antes do ponto são válidos
    if(verificaInt(string1, tam1) == 0)
        return 0;

    if(ponto == 1 && tam1 < tam-1)
    {
		int j;
        for(i = tam1+1, j = 0; i < tam; i++, j++)
            string2[j] = string[i];

        // Verifica se os números depois do ponto são válidos
        if(verificaInt(string2, tam2) == 0)
            return 0;
    }

    return 1;   
}
//==================================================================

// Função para ler dinamicamente uma string
// Recebe a variável para receber a string. Retorna seu tamanho

int stringDinamica(char *string)
{
    int tam = 0;

    do
    {
        string[tam] = getchar();
        tam++;
        string = realloc(string, (tam+1) * sizeof(char));
    }while(string[tam-1] != '\n');
    tam--;
    string[tam] = '\0';

    return tam;
}
//===================================================================

// Função para ler apenas números com ponto flutuante
// Recebe o Texto a ser exibido. Retorna o float lido
float leiaFloat(char *texto)
{
    float num;
    
    while(1)
    {
        // Exibe a mensagem recebida
        printf("%s", texto);

        char *string = (char *) malloc(sizeof(char));
        int tam = stringDinamica(string);
        
        if(verificaFloat(string, tam) == 0)
            printf("\nEntrada inválida!! Digite um número válido!\n");
        else
        {
            num = atof(string);
            break;
        }
    }
    return num;
}
//==================================================================}

//////////////////////////////  ARQUIVO  //////////////////////////////////////////////

// Função para abrir arquivos
// Recebe o nome e modo do arquivo. Retorna o endereço do arquivo
FILE *abrirArquivo(char *nome, char *modo)
{
    FILE *arquivo = fopen(nome, modo);

    if(arquivo == NULL)
    {
        printf("\nErro ao abrir o arquivo %s\n", nome);
        exit(EXIT_FAILURE);
    }
    
    return arquivo;
}
//==================================================================

// Função para verificar se um arquivo existe
// Recebe o nome do arquivo
int arquivoExiste(char *nome)
{
    FILE *arquivo = fopen(nome, "rb");

    if(arquivo == NULL)
        return 0;
    
    fclose(arquivo);
    return 1;
}
//==================================================================

// Função para ler o arquivo do quarto e salvar as informações em um vetor
// Recebe uma variável para guardar o tamanho. Retorna o endereço do vetor
Quarto *lerArquivoQuarto(int *tam)
{
    if(arquivoExiste(ARQQUARTO))
    {
        FILE *arquivo = abrirArquivo(ARQQUARTO, "rb");
        Quarto aux;


        Quarto *vetor = (Quarto *) malloc(sizeof(Quarto));
        *tam = 0;
        while(fread(&aux, sizeof(Quarto), 1, arquivo) == 1)
        {
            vetor[*tam] = aux;
            (*tam)++;
            // Aumenta o tamanho do vetor dinamicamente
            vetor = realloc(vetor, ((*tam) + 1) * sizeof(Quarto));
        }
        fclose(arquivo);

        return vetor;
    }
    *tam = -1;
    return NULL;
}

// Função para salvar as informações do quarto em um arquivo
// Recebe a variável do quarto
int salvarQuarto(Quarto item)
{
    FILE *arquivo = abrirArquivo(ARQQUARTO, "ab");

    if(fwrite(&item, sizeof(Quarto), 1, arquivo) < 1)
    {
        printf("\nErro! Não foi possível salvar as informações\n");
        return 0;
    }
    fclose(arquivo);

    return 1;
}

// Função para salvar a informação editada, reescrevendo o arquivo
// Recebe o endereço do vetor que guarda todas as informações e seu tamanho
void refazerArquivoQuarto(Quarto *vetor, int tam)
{
    FILE *arquivo = abrirArquivo(ARQQUARTO, "wb");
    fclose(arquivo);

    for(int i = 0; i < tam; i++)
        salvarQuarto(vetor[i]);
}


/////////////////////////////  QUARTOS  ///////////////////////////////////////////////

// Função para exibir o menu de opções da seção quartos (menu principal)
// Retorna a opção escolhida
int menuQuartos()
{
    printf("\n-----SEÇÃO DOS QUARTOS-----");
    printf("\n[1] - Cadastrar quarto");
    printf("\n[2] - Consultar quarto");
    printf("\n[3] - Editar quarto");
    printf("\n[4] - Excluir quarto");
    printf("\n[5] - Exibir todos");
    printf("\n[0] - Voltar");
    int op = leiaInt("\nOpção: ");
    return op;
}
//===================================================================

//Função para exibir o menu de opções de tipos de quartos
//Retorna a opção escolhida
int menuTipo()
{
    printf("\n-----TIPO DE QUARTO-----");
    printf("\n[1] - Quarto simples");
    printf("\n[2] - Quarto duplo");
    printf("\n[3] - Quarto suíte");
    int op;
    do
    {
        op = leiaInt("\nOpção: ");
        if(op < 1 || op > 3)
        {
            printf("\nERRO! Digite uma opção válida.\n");
        }
    } while (op < 1 || op > 3);
    op--;
    return op;
}

//===================================================================

//Função para exibir o menu de opções do status do quarto
//retorna a opção escolhida
int menuStatus()
{
    printf("\n-----TIPO DE QUARTO-----");
    printf("\n[1] - Livre");
    printf("\n[2] - Ocupado");
    int op;
    do
    {
        op = leiaInt("\nOpção: ");
        if(op < 1 || op > 2)
        {
            printf("\nERRO! Digite uma opção válida.\n");
        }
    } while (op < 1 || op > 2);
    op--;
    return op;
} 
//===================================================================

//Função para exibir o menu de opções de consulta
//retorna a opção escolhida
int menuBusca()
{
    printf("\n-----CONSULTA POR-----");
    printf("\n[1] - Número");
    printf("\n[2] - Tipo");
    printf("\n[3] - Status");
    printf("\n[0] - Voltar");
    int op = leiaInt("\nOpção: ");
    return op;
}

//=======================================================================

//////////////////////////////  EXIBIR  ////////////////////////////////////////////////////

// Função para exibir um quarto
// Recebe a variavel do Quarto e o "status"
// Se status for verdadeiro (!= 0), exibe o status do quarto
void exibirQuarto(Quarto quarto, int status)
{
    printf("\n----------Quarto----------");
    printf("\nNúmero: %d", quarto.numero);

    if(quarto.tipo == SIMPLES)
        printf("\nTipo: Simples");
    else if(quarto.tipo == DUPLO)
        printf("\nTipo: Duplo");
    else if(quarto.tipo == SUITE)
        printf("\nTipo: Suíte");

    if(status)
    {
        if(quarto.status == LIVRE)
            printf("\nStatus: Livre");
        else if(quarto.status == OCUPADO)
            printf("\nStatus: Ocupado");
        else if(quarto.status == RESERVADO)
            printf("\nStatus: Reservado");
    }

    printf("\nPreço: R$%.2f\n", quarto.preco);
}

//Função para exibir todas as informações dos quartos
void exibirTodosQuartos()
{
    int tam, i;
    Quarto *vetor = lerArquivoQuarto(&tam);

    if(vetor != NULL && tam > 0)
    {
        printf("\n----- INFORMAÇÕES DOS QUARTOS -----\n");

        for(i = 0; i < tam; i++)
        {
            exibirQuarto(vetor[i], 1);

        }
    }
    else
        printf("\nNenhum quarto foi cadastrado ainda!!\n");
    free(vetor);
}

//////////////////////////////  CADASTRO  ///////////////////////////////////////////////////

//Função para verificar se não há outro quarto com o mesmo número
int numQuartoExiste(int numero)
{
    int tam, i;
    Quarto *vetor = lerArquivoQuarto(&tam);

    if(vetor != NULL && tam > 0)
    {
        for(i = 0; i < tam; i++)
        {
            if (vetor[i].numero == numero) 
            {
                free(vetor);
                return 1;
            }
        }
    }
    free(vetor);
    return 0;
}

void cadastroQuarto()
{
    Quarto novoQuarto;

    printf("\nCADASTRO DE QUARTOS:\n");
    do
    {
        novoQuarto.numero = leiaInt("\nDigite o número do quarto: ");

        if(numQuartoExiste(novoQuarto.numero)) 
        {
            printf("\nERRO! O número do quarto já existe. Tente novamente.\n");
        }
        else if(novoQuarto.numero < 0)
        {
            printf("\nERRO! O Número digitado é inválido. Tente novamente.\n");
        }
    } while (numQuartoExiste(novoQuarto.numero) || novoQuarto.numero < 0);

    novoQuarto.tipo = menuTipo();
    novoQuarto.status = menuStatus();
    novoQuarto.preco = leiaFloat("\nDigite o preço do quarto: R$");

    salvarQuarto(novoQuarto);

    printf("\nQuarto cadastrado com sucesso!\n");
}

///////////////////////////////  BUSCAR  ////////////////////////////////////////////////////

int buscaNumero(int numero) 
{
    int tam, i;
    Quarto *vetor = lerArquivoQuarto(&tam);
    char C;

    for(i = 0; i < tam; i++)
    {
        if(vetor[i].numero == numero) 
        {
            do
            {
                printf("\nQuarto encontrado:\n");   //// TALVEZ DÊ PRA ADICIONAR INFORMAÇÕES DO CLIENTE ////
                printf("\nExibir todas as informações sobre o quarto? (S/N): ");

                if(scanf(" %c", &C) == 'S' || C == 's')
                {
                    exibirQuarto(vetor[i], 1);
                    return 1;
                }
                else if(C == 'N' || C == 'n')
                {
                    return 0;
                }
                else
                {
                    printf("\nOpção inválida!!\n");
                }
            }while(C != 'N' && C != 'n' && C != 'S' && C != 's');
        }
    }
    free(vetor);
    printf("\nQuarto não encontrado.\n");
    return 0;
}
//=======================================================================

int buscaTipo(int tipo)
{
    int tam, i;
    Quarto *vetor = lerArquivoQuarto(&tam);
    char C;

    for(i = 0; i < tam; i++)
    {
        if(vetor[i].tipo == tipo) 
        {
            do
            {
                printf("\nQuarto encontrado:\n");   //// TALVEZ DÊ PRA ADICIONAR INFORMAÇÕES DO CLIENTE ////
                printf("\nExibir todas as informações sobre o quarto? (S/N): ");

                if(scanf(" %c", &C) == 'S' || C == 's')
                {
                    exibirQuarto(vetor[i], 1);
                    return 1;
                }
                else if(C == 'N' || C == 'n')
                {
                    return 0;
                }
                else
                {
                    printf("\nOpção inválida!!\n");
                }
            }while(C != 'N' && C != 'n' && C != 'S' && C != 's');
        }
    }
    free(vetor);
    printf("\nQuarto não encontrado.\n");
    return 0;
}
//=======================================================================

int buscaStatus(int status)
{
    int tam, i;
    Quarto *vetor = lerArquivoQuarto(&tam);
    char C;

    for(i = 0; i < tam; i++)
    {
        if(vetor[i].status == status) 
        {
            do
            {
                printf("\nQuarto encontrado:\n");   //// TALVEZ DÊ PRA ADICIONAR INFORMAÇÕES DO CLIENTE ////
                printf("\nExibir todas as informações sobre o quarto? (S/N): ");

                if(scanf(" %c", &C) == 'S' || C == 's')
                {
                    exibirQuarto(vetor[i], 1);
                    return 1;
                }
                else if(C == 'N' || C == 'n')
                {
                    return 0;
                }
                else
                {
                    printf("\nOpção inválida!!\n");
                }
            }while(C != 'N' && C != 'n' && C != 'S' && C != 's');
        }
    }
    free(vetor);
    printf("\nQuarto não encontrado.\n");
    return 0;
}
//=======================================================================

int buscaQuarto()
{
    int op, tam;
    Quarto *vetor = lerArquivoQuarto(&tam);

    if(vetor != NULL && tam > 0)
    {
        do
        {
            op = menuBusca();
            switch(op)
            {
                case 1: 
                {
                    int numeroConsulta = leiaInt("\nDigite o número do quarto para consultar: ");
                    buscaNumero(numeroConsulta);
                }
                    break;

                case 2: 
                {
                    int tipoConsulta = menuTipo();
                    buscaNumero(tipoConsulta);
                }
                    break;

                case 3:
                {
                    int statusConsulta = menuStatus();
                    buscaNumero(statusConsulta);
                }
                    break;

                case 0:
                {
                    printf("\nVoltando...\n");
                    break;
                }

                default:
                    printf("\nOpção inválida!\n");
            }
        } while (op < 0 || op > 3);
    }
    else
        printf("\nNenhum quarto foi cadastrado ainda!!\n");
    return op;
}
//=======================================================================

//////////////////////////////  EDITAR  ////////////////////////////////////////////////////

//=======================================================================

//////////////////////////////  EXCLUIR  ///////////////////////////////////////////////////

//=======================================================================

//////////////////////////////  MAIN  //////////////////////////////////////////////////////
int main()
{
    int op;

    do
    {
        op = menuQuartos();
        switch(op)
        {
            case 1:
                cadastroQuarto();
                break;
            
            case 2:
                buscaQuarto();
                break;

            case 3:
                
                break;
            
            case 4:
                
                break;

            case 5:
                exibirTodosQuartos();
                break;
            
            case 0:
            {
                printf("\nVoltando...\n");
                break;
            }

            default:
                printf("\nOpção inválida!\n");
        }
    } while(op != 0);
    return 0;
}
