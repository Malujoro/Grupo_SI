#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQRESERVA "reservas.bin"
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

// Função para verificar se uma hora é válida
// Recebe a String a ser verificada
int verificaHora(char *string)
{
    int x, tam = strlen(string);
    char aux[3];

    if(tam == TAM5-1 && string[2] == ':')
    {
        strncpy(aux, string, 2);
        aux[2] = '\0';
        x = atoi(aux);
        printf("\n[%d][%s][%s]\n", x, aux, string);
        if(verificaInt(aux, 2) && x >= 0 && x <= 23)
        {
            for(int i = 3; i < TAM5-1; i++)
                aux[i-3] = string[i];
            
            aux[2] = '\0';
            x = atoi(aux);
            printf("\n[%d]\n", x);
            if(verificaInt(aux, 2) && x >= 0 && x <= 59)
                return 1;
        }
    }
    return 0;
}

// Função para verificar se uma data é válida
// Recebe a String a ser verificada
/* ============================================================================ */
int verificaData(char *string)
{
    char aux[4];
    int tam = strlen(string);

    if(tam == TAM4-1 && string[2] == '/' && string[5] == '/')
    {
        strncpy(aux, string, 2);
        if(verificaInt(aux, 2))
        {
            int i;
            for(i = 3; i < 5; i++)
                aux[i-3] = string[i];

            if(verificaInt(aux, 2))
            {
                for(i = 6; i < TAM4-1; i++)
                    aux[i-6] = string[i];
                
                if(verificaInt(aux, 4))
                    return 1;
            }
        }
    }
    return 0;
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

///////////////////////////  ARQUIVO  //////////////////////////////////////////

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

///////////////////////////  RESERVAS  //////////////////////////////////////////

// Função para exibir uma reserva
// Recebe a variavel da Reserva
/* ============================================================================ */
void exibirReserva(Reserva item)
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

    printf("\n----------Reserva----------");
    printf("\nNúmero de reserva: %d", item.numReserva);
    printf("\nNúmero do quarto: %d", item.numQuarto);
    printf("\nCPF: %s", item.cpf);
    printf("\nData de entrada: %s", item.dataEntrada);
    printf("\nData de saída: %s", item.dataSaida);
    printf("\nHora de entrada: %s", item.horaEntrada);
    printf("\nHora de saída: %s", item.horaSaida);
    printf("\nTotal: R$%.2f\n", item.total);
    if(item.pagamento == 0)
        printf("\nStatus de pagamento: Pendente\n");
    else
        printf("\nStatus de pagamento: Pago\n");
}

// Função para exibir o menu de opções da seção cliente
// Retorna a opção escolhida
int menuReserva()
{
    int op;
    printf("\n-----SEÇÃO DA RESERVA-----");
    printf("\n[1] - Realizar reserva");
    printf("\n[2] - Excluir reserva");
    printf("\n[3] - Realizar Check-in");
    printf("\n[4] - Realizar pagamento");
    printf("\n[5] - Consultar reservas");
    printf("\n[6] - Valores recebidos");
    printf("\n[7] - Exibir todas");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");
    return op;
}

// Função para ler um arquivo e salvar as informações em um vetor
// Recebe uma variável para guardar o tamanho. Retorna o endereço do vetor
Reserva *lerArquivoReserva(int *tam)
{
    if(arquivoExiste(ARQRESERVA))
    {
        FILE *arquivo = abrirArquivo(ARQRESERVA, "rb");
        Reserva aux;

        Reserva *vetor = (Reserva *) malloc(sizeof(Reserva));
        *tam = 0;
        while(fread(&aux, sizeof(Reserva), 1, arquivo) == 1)
        {
            vetor[*tam] = aux;
            (*tam)++;
            // Aumenta o tamanho do vetor dinamicamente
            vetor = realloc(vetor, ((*tam) + 1) * sizeof(Reserva));
        }
        fclose(arquivo);

        return vetor;
    }
    *tam = -1;
    return NULL;
}

// Função para salvar as informações do cliente em um arquivo
// Recebe a variável da pessoa
int salvarReserva(Reserva item)
{
    FILE *arquivo = abrirArquivo(ARQRESERVA, "ab");

    if(fwrite(&item, sizeof(Reserva), 1, arquivo) < 1)
    {
        printf("\nErro! Não foi possível salvar as informações\n");
        return 0;
    }
    fclose(arquivo);

    return 1;
}

// Função para salvar a informação editada, reescrevendo o arquivo
// Recebe o endereço do vetor que guarda todas as informações e seu tamanho
void refazerArquivoReserva(Reserva *vetor, int tam)
{
    FILE *arquivo = abrirArquivo(ARQRESERVA, "wb");
    fclose(arquivo);

    for(int i = 0; i < tam; i++)
        salvarCliente(vetor[i]);
}

///////////////////////////  LEITURA  //////////////////////////////////////////

void leiaNumReserva(int *num)
{
    *num = leiaInt("Número da reserva: ");
}

void leiaNumQuarto(int *num)
{
    *num = leiaInt("Número do quarto: ");
}

// Função para ler o CPF digitado e conferir se está em um formato válido 123.456.789-00 ou 12345678900
// Após a leitura, irá formata-lo e deixar no formato 123.456.789-00
// Recebe a variável que vai receber o CPF
void leiaCPF(char *cpf)
{
    char aux[TAM2];
    int tam, invalido = 1;

    do
    {
        printf("\nCPF: ");
        scanf(" %[^\n]s", aux);
        limpaBuffer();

        tam = strlen(aux);

        // Caso em que foi digitado apenas o CPF numérico - 12345678900
        if((tam == TAM2-4) && verificaInt(aux, TAM2-4))
        {
            invalido = 0;
            // Formata o CPF
            cpf[3] = '.';
            cpf[7] = '.';
            cpf[11] = '-';
            for(int i = 0, j = 0; i < tam; i++, j++)
            {
                if(j == 3 || j == 7 || j == 11)
                    j++;
                cpf[j] = aux[i];
            }
        }
        // Caso em que o CPF foi digitado completo, verifica se é válido - 123.456.789-00
        else if(tam == TAM2-1)
        {
            // Verifica se os os números e símbolos estão na posição certa
            char aux2[TAM2-4];
            for(int i = 0, j = 0; i < tam; i++)
            {
                // Cria um vetor auxiliar apenas com os números
                if((i % 4) != 3)
                {
                    aux2[j] = aux[i];
                    j++;
                }
            }
            // Efetua a troca caso o CPF digitado contenha o /, para uma melhor estética
            if(aux[11] == '/')
                aux[11] = '-';

            // Executa a verificação final se está formatado corretamente
            if(verificaInt(aux2, TAM2-4) && aux[3] == '.' && aux[7] == '.' && aux[11] == '-')
            {
                invalido = 0;
                strcpy(cpf, aux);
            }
        }
        if(invalido)
            printf("\nCPF inválido!!\n");
    }while(invalido);
    cpf[TAM2-1] = '\0';
}

void leiaData(char *texto, char *data)
{
    int tam, invalido = 1;
    char aux[TAM4];

    do
    {
        printf("%s", texto);
        scanf(" %[^\n]s", aux);

        if(aux[2] == '-' || aux[2] == ' ')
            aux[2] = '/';

        if(aux[5] == '-' || aux[5] == ' ')
            aux[5] = '/';


        tam = strlen(aux);
        if(tam == TAM4-3 && verificaInt(aux, tam))
        {
            int i;
            for(i = 7; i > 3; i--)
                data[i+2] = aux[i];

            data[5] = '/';

            for(i = 3; i > 1; i--)
                data[i+1] = aux[i];

            data[2] = '/';

            strncpy(data, aux, 2);

            if(verificaData(data))
                invalido = 0;
        }

        else if(verificaData(aux))
            strcpy(data, aux);

        if(invalido)
            printf("\nData inválida!!\n");
    }while(invalido);
    data[TAM4-1] = '\0';
}

void leiaHora(char *texto, char *hora)
{
    char aux[TAM5];
    int tam, invalido = 1;

    do
    {
        printf("%s", texto);
        scanf(" %[^\n]s", aux);

        tam = strlen(aux);

        if(tam == TAM5-2 && verificaInt(aux, tam))
        {
            for(int i = 3; i > 1; i--)
                hora[i+1] = aux[i];

            hora[2] = ':';

            strncpy(hora, aux, 2);
            verificaHora(hora);
            invalido = 0;
        }
        else if(verificaHora(aux))
        {
            strcpy(hora, aux);
            invalido = 0;
        }
        if(invalido)
            printf("\nHora inválida!!\n");
    }while(invalido);
    hora[TAM5-1] = '\0';
}

///////////////////////////  BUSCA  //////////////////////////////////////////


///////////////////////////  CADASTRO  //////////////////////////////////////////


///////////////////////////  ESTRUTURA  //////////////////////////////////////////

void realizarReserva()
{
    Reserva item;

    //  cadastrarNumQuarto();
    //  cadastrarCPF();
    //  cadastrarData();
    //  cadastrarData();

    //  cadastrarHora();
    //  cadastrarHora();

    //  Total; // Calcula automático
    //  Pagamento; // 0 - Pendente, 1 - Pago - Geralmente é pendente
    //  NumReserva; // Gera automático

    // salvarReserva()
}
// void excluirReserva();
// void realizarCheckin();
// void realizarPagamento();
// void consultarReservas();
// void valoresRecebidos();
// void exibirTodasReservas();


int main()
{
    int op;

    do
    {
        op = menuReserva();
        switch (op)
        {
            case 1:
                // realizarReserva();
                break;

            case 2:
                // excluirReserva();
                break;
                
            case 3:
                // realizarCheckin();
                break;
                
            case 4:
                // realizarPagamento();
                break;
                
            case 5:
                // consultarReservas();
                break;
                
            case 6:
                // valoresRecebidos();
                break;
                
            case 7:
                // exibirTodasReservas();    
                break;
                
            case 0:
                break;
                            
            default:
                printf("\nOpção inválida!\n");
        }
    }while(op != 0);

    return 0;
}