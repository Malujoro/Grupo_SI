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
    int tipo; // 0 - Simples, 1 - Duplos, 2 - Suíte
    int status; // 0 - Livre, 1 - Ocupado, 2 - Reservados
    float valor;
} Quarto;


// Função para limpar o Buffer (caracteres excedentes)
void limpaBuffer()
{
    while(getchar() != '\n');
}

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

// Função para exibir um quarto
// Recebe a variavel do Quarto e o "status"
// Se status for verdadeiro (!= 0), exibe o status do quarto
void exibirQuarto(Quarto quarto, int status)
{
    printf("\n----------Quarto----------");
    printf("\nQuarto %d", quarto.numero);

    if(quarto.tipo == SIMPLES)
        printf("\nTipo Simples");
    else if(quarto.tipo == DUPLO)
        printf("\nTipo Duplo");
    else if(quarto.tipo == SUITE)
        printf("\nTipo Suíte");

    if(status)
    {
        if(quarto.status == LIVRE)
            printf("\nStatus Livre");
        else if(quarto.status == OCUPADO)
            printf("\nStatus Ocupado");
        else if(quarto.status == RESERVADO)
            printf("\nStatus Reservado");
    }

    printf("\nValor %.2f\n", quarto.valor);
}

// Função para exibir o menu de opções da seção quarto
// Retorna a opção escolhida
int menuQuartos()
{
    int op;
    printf("\n-----SEÇÃO DO QUARTO-----");
    printf("\n[1] - Cadastrar quarto");
    printf("\n[2] - Consultar quarto");
    printf("\n[3] - Editar quarto");
    printf("\n[4] - Excluir quarto");
    printf("\n[5] - Exibir todos");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");
    return op;
}

// Função para exibir o menu de opções para buscar o quarto
// Retorna a opção escolhida
int menuBuscaQuarto()
{
    int op;
    printf("\n-----Buscar quarto por:-----");
    printf("\n[1] - Número");
    printf("\n[2] - Tipo");
    printf("\n[3] - Status");
    printf("\n[4] - Valor");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");

    return op;
}

int menuEditQuarto()
{
    int op;
    printf("\n-----Editar-----");
    printf("\n[1] - Tipo");
    printf("\n[2] - Valor");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");

    return op;
}

int menuTipo()
{
    int op;
    printf("\n-----Tipo do Quarto-----");
    printf("\n[0] - Simples");
    printf("\n[1] - Duplo");
    printf("\n[2] - Suíte");
    op = leiaInt("\nOpção: ");
    return op;
}

int menuStatus(int reservado)
{
    int op;
    printf("\n-----Status do Quarto-----");
    printf("\n[0] - Livre");
    printf("\n[1] - Ocupado");
    if(reservado)
        printf("\n[2] - Reservado");
    op = leiaInt("\nOpção: ");
    return op;
}

void cadastrarNumQuarto(int *num)
{
    int tam, invalido, i;
    Quarto *vetor = lerArquivoQuarto(&tam);

    do
    {
        *num = leiaInt("\nNúmero do quarto: ");
        if(*num > 0)
        {
            invalido = 0;
            for(i = 0; i < tam; i++)
            {
                if(vetor[i].numero == *num)
                {
                    invalido = 1;
                    printf("\nErro! Número já cadastrado\n");
                    break;
                }
            }
        }
        else
            printf("\nNúmero de quarto inválido!!\n");
    }while(invalido);
    free(vetor);
}

void cadastrarTipo(int *tipo)
{
    do
    {
        *tipo = menuTipo();

        if(*tipo < 0 || *tipo > 2)
            printf("\nErro! Tipo inválido\n");

    }while(*tipo < 0 || *tipo > 2);
}

void cadastrarStatus(int *status, int reservado)
{
    int limite;

    if(reservado)
        limite = 2;
    else
        limite = 1;

    do
    {
        *status = menuStatus(reservado);

        if(*status < 0 || *status > limite)
            printf("\nErro! status inválido\n");
            
    }while(*status < 0 || *status > limite);
}

void cadastrarQuarto()
{
    Quarto quarto;

    printf("\nCADASTRO DE QUARTO\n");
    cadastrarNumQuarto(&quarto.numero);
    cadastrarStatus(&quarto.status, 0);
    cadastrarTipo(&quarto.tipo);
    quarto.valor = leiaFloat("\nValor do quarto: R$");

    if(salvarQuarto(quarto))
        printf("\nQuarto cadastrado com sucesso!\n");
}

// Função para buscar um quarto pelo seu número
// Recebe o endereço para "coletar" os dados do quarto, sua posição no vetor e o "user"
// Se o user for falso (0), irá pedir ao usuário para digitar o número doç quarto buscado
// Se o user for verdadeiro (!= 0), o número buscado será o do próprio user
int buscaNumQuarto(Quarto *quarto, int *pos, int user)
{
    int tam, i, numero;
    Quarto *vetor = lerArquivoQuarto(&tam);

    if(user == 0)
    {
        numero = leiaInt("\nNúmero do quarto: ");
        printf("\nBuscando pelo [Quarto %d]...\n", numero);
    }
    else
        numero = user;

    for(i = 0; i < tam; i++)
    {
        if(vetor[i].numero == numero)
        {
            *pos = i;
            *quarto = vetor[i];
            return 1;
        }
    }
    free(vetor);
    return 0;
}

int buscaTipo(Quarto *quarto, int *pos)
{
    int tam, i, tipo;
    char ch;
    Quarto *vetor = lerArquivoQuarto(&tam);

    // tipo = menuTipo();
    cadastrarTipo(&tipo);

    printf("\nBuscando por Quarto...\n");
    for(i = 0; i < tam; i++)
    {
        if(vetor[i].tipo == tipo)
        {
            do
            {
                exibirQuarto(vetor[i], 1);
                printf("\nEsse quarto? ");
                ch = getchar();
                limpaBuffer();
                if(ch == 'S' || ch == 's')
                {
                    *pos = i;
                    *quarto = vetor[i];
                    return 1;
                }
                else if (ch == 'N' || ch == 'n')
                    printf("\nBuscando por Quarto...\n");
                else
                    printf("\nOpção inválida!!\n");
            }while(ch != 'N' && ch != 'n');
        }
    }
    printf("\nNão foi encontrado nenhum quarto desse tipo!!\n");
    free(vetor);
    return 0;
}

int buscaStatus(Quarto *quarto, int *pos)
{
    int tam, i, status;
    char ch;
    Quarto *vetor = lerArquivoQuarto(&tam);

    cadastrarStatus(&status, 1);

    printf("\nBuscando por Quarto...\n");
    for(i = 0; i < tam; i++)
    {
        if(vetor[i].status == status)
        {
            do
            {
                exibirQuarto(vetor[i], 1);
                printf("\nEsse quarto? ");
                ch = getchar();
                limpaBuffer();
                if(ch == 'S' || ch == 's')
                {
                    *pos = i;
                    *quarto = vetor[i];
                    return 1;
                }
                else if (ch == 'N' || ch == 'n')
                    printf("\nBuscando por Quarto...\n");
                else
                    printf("\nOpção inválida!!\n");
            }while(ch != 'N' && ch != 'n');
        }
    }
    printf("\nNão foi encontrado nenhum quarto desse status!!\n");
    free(vetor);
    return 0;
}

int buscaValor(Quarto *quarto, int *pos)
{
    int tam, i;
    float valor;
    char ch;
    Quarto *vetor = lerArquivoQuarto(&tam);

    valor = leiaFloat("\nValor: R$");

    printf("\nBuscando por Quarto...\n");
    for(i = 0; i < tam; i++)
    {
        if(vetor[i].valor == valor)
        {
            do
            {
                exibirQuarto(vetor[i], 1);
                printf("\nEsse quarto? ");
                ch = getchar();
                limpaBuffer();
                if(ch == 'S' || ch == 's')
                {
                    *pos = i;
                    *quarto = vetor[i];
                    return 1;
                }
                else if (ch == 'N' || ch == 'n')
                    printf("\nBuscando por Quarto...\n");
                else
                    printf("\nOpção inválida!!\n");
            }while(ch != 'N' && ch != 'n');
        }
    }
    printf("\nNão foi encontrado nenhum quarto com esse preço!!\n");
    free(vetor);
    return 0;
}

int consultarQuarto(Quarto *quarto, int *pos)
{
    int op, tam;
    Quarto *vetor = lerArquivoQuarto(&tam);
    if(vetor != NULL && tam > 0)
    {
       do
        {
            op = menuBuscaQuarto();

            switch(op)
            {
                case 1:
                    if(buscaNumQuarto(quarto, pos, 0))
                        exibirQuarto(*quarto, 1);
                    else
                        op = 0;
                    break;

                case 2:
                    if(buscaTipo(quarto, pos) == 0)
                        op = 0;
                    break;
                case 3:
                    if(buscaStatus(quarto, pos) == 0)
                        op = 0;
                    break;
                case 4:
                    if(buscaValor(quarto, pos) == 0)
                        op = 0;
                    break;
                case 0:
                    printf("\nVoltando...\n");
                    break;

                default:
                    printf("\nOpção inválida!\n");
            }
        }while(op < 0 || op > 2);
        return op; 
    }
    free(vetor);
    printf("\nNenhum quarto foi cadastrado ainda!!\n");
    return 0;
}

void editarQuarto()
{
    int op, pos;
    Quarto quarto, quarto2;

    if(consultarQuarto(&quarto, &pos))
    {
        if(quarto.status == LIVRE)
        {
            do
            {
                op = menuEditQuarto();
                switch(op)
                {
                    case 1:
                        cadastrarTipo(&quarto2.tipo);
                        quarto.tipo = quarto2.tipo;
                        break;
                    case 2:
                        quarto2.valor = leiaFloat("\nValor do quarto: R$");
                        quarto.valor = quarto2.valor;
                        break;

                    case 0:
                        printf("\nVoltando...\n");
                        break;

                    default:
                        printf("\nOpção inválida!\n");
                }
            }while(op < 0 || op > 2);

            if(op > 0)
            {
                printf("\nDados alterados com sucesso\n");
                exibirQuarto(quarto, 1);
                // Lê todos os dados do arquivo
                int tam;
                Quarto *vetor = lerArquivoQuarto(&tam);
                vetor[pos] = quarto;

                // Salva a informação editada, reescrevendo o arquivo
                refazerArquivoQuarto(vetor, tam);
                free(vetor);
            }
        }
        else
            printf("\nNão é possível editar quartos reservados/ocupados\n");
    }
}

void excluirQuarto()
{
    int pos, tam, i;
    char ch;
    Quarto quarto;
    Quarto *vetor = lerArquivoQuarto(&tam);

    if(consultarQuarto(&quarto, &pos))
    {
        if(quarto.status == LIVRE)
        {
            do
            {
                printf("Tem certeza que deseja excluir? Os dados desse cliente serão perdidos para sempre\n");
                ch = getchar();
                limpaBuffer();
                if(ch == 'S' || ch == 's')
                {
                    tam--;
                    for(i = pos; i < tam; i++)
                        vetor[i] = vetor[i+1];
                    
                    refazerArquivoQuarto(vetor, tam);
                    printf("\nQuarto excluído com sucesso\n");
                }
                else if (ch == 'N' || ch == 'n')
                    printf("\nCancelando...\n");
                else
                    printf("\nOpção inválida!!\n");
            }while(ch != 'N' && ch != 'n' && ch != 's' && ch != 'S');
        }
        else
            printf("\nNão é possível excluir quartos reservados/ocupados\n");
    }
    free(vetor);
}

void exibirTodosQuartos()
{
    int tam;
    Quarto *vetor = lerArquivoQuarto(&tam);

    if(vetor != NULL && tam > 0)
    {
        for(int i = 0; i < tam; i++)
            exibirQuarto(vetor[i], 1);
    }
    else
        printf("\nNenhum quarto foi cadastrado ainda!!\n");
    free(vetor);
}

int main()
{
    int op, pos;
    Quarto aux;

    do
    {
        op = menuQuartos();
        switch(op)
        {
            case 1:
                cadastrarQuarto();
                break;
            
            case 2:
                consultarQuarto(&aux, &pos);
                break;

            case 3:
                editarQuarto();
                break;
            
            case 4:
                excluirQuarto();
                break;

            case 5:
                exibirTodosQuartos();
                break;

            case 0:
                printf("\nVoltando...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }
    }while(op != 0);
    return 0;
}