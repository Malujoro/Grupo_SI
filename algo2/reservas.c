#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQRESERVAS "reservas.bin"
#define TAM2 15 // Tamanho do CPF e Telefone
#define TAM4 11 // Tamanho das datas 00/00/0000
#define TAM5 6 // Tamanho das horas 00:00

typedef struct
{
    int numReserva;
    int numQuarto;
    char cpf[TAM2];
    char dataEntrada[TAM4];
    char dataSaida[TAM4];
    char horaEntrada[TAM5];
    char horaSaida[TAM5];
    float total;
    int pagamento; // 0 - Pendente, 1 - Pago 
} Reserva;

///////////////////////////  OUTROS  //////////////////////////////////////////

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
    // for(i = 0; i < tam1; i++)
        // string1[i] = string[i];

    if(verificaInt(string1, tam1) == 0)
        return 0;

    if(ponto == 1 && tam1 < tam-1)
    {
        for(i = tam1+1; i < tam; i++)
            string2[i] = string[i];

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

// Função para converter uma string em maiúsculas
// Recebe a string usada como base, a variável que vai retornar o maiúsculo e o seu tamanho. Retorna o endereço da string em maiúscula
void maiusc(char *string, char *string2, int tam)
{
    for(int i = 0; i < tam; i++)
    {
        if(string[i] >= 'a' && string[i] <= 'z')
            string2[i] = string[i] - ('a' - 'A');
    }
}


// void realizarReserva();
// void excluirReserva();
// void realizarCheckin();
// void realizarPagamento();
// void consultarReservas();
// void valoresRecebidos();


int main()
{
    Reserva teste;
    printf("Reserva: ");
    scanf(" %d", &teste.numReserva);

    printf("Quarto: ");
    scanf(" %d", &teste.numQuarto);

    leiaCPF();

}