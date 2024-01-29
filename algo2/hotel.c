#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Nomes dos arquivos
#define ARQQUARTO "quartos.bin"
#define ARQCLIENTE "clientes.bin"
#define ARQRESERVA "reservas.bin"

// Tipos dos quartos
#define SIMPLES 0
#define DUPLO 1
#define SUITE 2

// Status dos quartos
#define LIVRE 0
#define OCUPADO 1
#define RESERVADO 2

// Pagamento da Reserva
#define PENDENTE 0
#define PAGO 1

// Tamanhos "universais"
#define TAM1 100 // Tamanho do Nome, Email e Endereço
#define TAM2 15 // Tamanho do CPF e Telefone
#define TAM3 13 // Tamanho do RG
#define TAM4 11 // Tamanho das datas 00/00/0000
#define TAM5 6 // Tamanho das horas 00:00

// Structs utilizadas
typedef struct
{
    int numero;
    int tipo; // 0 - Simples, 1 - Duplos, 2 - Suíte
    int status; // 0 - Livre, 1 - Ocupado, 2 - Reservados
    float preco;
} Quarto;

typedef struct
{
    char nome[TAM1];
    char cpf[TAM2]; // 123.456.789-00
    char rg[TAM3];  // 12.345.678-9
    char telefone[TAM2]; // (00)12345-6789
    char endereco[TAM1];
    char email[TAM1];
} Cliente;

typedef struct
{
    int numReserva;
    int numQuarto;
    char nome[TAM1];
    char cpf[TAM2];
    char dataEntrada[TAM4];
    char dataSaida[TAM4];
    char horaEntrada[TAM5];
    char horaSaida[TAM5];
    float total;
    int pagamento; // 0 - Pendente, 1 - Pago 
} Reserva;

///////////////////////////  OUTROS  ///////////////////////////

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

// Converte uma data do calendário Gregoriano para o calendário Juliano
// Recebe os valores em string gregoriana 12/34/5678. Retorna o valor em juliana
int gregorianaJuliana(char *grego)
{
    char aux[TAM4-2];

    for(int i = 0, j = 0; i < TAM4-3 && j < TAM4-1; i++, j++)
    {
        if(grego[j] == '/')
            j++;
        aux[i] = grego[j];
    }

    aux[TAM4-3] = '\0';
    int data = atoi(aux);

    int dia = data / 1000000;
    int mes = (data / 10000) - (dia * 100);
    int ano = data % 10000;
	int juliana;

	juliana = (1461 * (ano + 4800 + (mes - 14) / 12)) / 4 + (367 * (mes - 2 - 12 * ((mes - 14) / 12))) / 12 - (3 * ((ano + 4900 + (mes - 14) / 12) / 100)) / 4 + dia - 32075;
	
	return juliana;
}

// Converte uma data do calendário Juliano para o calendário Gregoriano
// Recebe o valor em juliana. Retorna os valores em data gregoriana 12345678 (12/34/5678)
long int julianaGregoriana(int juliana)
{
	int b, n, k, j;
	int dia, mes, ano;

	b = juliana + 68569;
	n = (4 * b) / 146097;
	b = b - ((146097 * n + 3) / 4);
	k = 4000 * (b + 1) / 1461001;
	b = b - (1461 * k) / 4 + 31;
	j = (80 * b) / 2447;
	dia = b - (2447 * j) / 80;
	b = (j / 11);
	mes = j + 2 - (12 * b);
	ano = 100 * (n - 49) + k + b;
	
	return ((dia * 1000000) + (mes * 10000) + ano);
}

// Compara se a diferença entre a data de entrada e de saída é válida
// Recebe as strings de entrada e saída
int comparaData(char *entrada, char *saida)
{
    int data1, data2;

    data1 = gregorianaJuliana(entrada);
    data2 = gregorianaJuliana(saida);

    return data2 - data1;
}

// Compara se a diferença entre a hora de entrada e de saída é válida (caso seja no mesmo dia)
// Recebe as strings de entrada e saída
int comparaHora(char *entrada, char *saida)
{
    char aux[TAM5], aux2[TAM5];

    int i, j;
    for(i = 0, j = 0; i < TAM5 && j < TAM5-1; i++, j++)
    {
        if(entrada[j] == ':')
            j++;
        aux[i] = entrada[j];
        aux2[i] = saida[j];
    }
    aux[i] = '\0';
    aux2[i] = '\0';
    return atoi(aux2) - atoi(aux);
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
    int x, i, tam = strlen(string);
    char aux[3], pos;

    for(int i = 0; i < tam; i++)
    {
        if(string[i] == ':')
        {
            pos = i;
            break;
        }
    }
    // Formatando horas nos formatos 1:23 e 12:3 para 01:23 e 12:03
    if(tam == 4 && (pos == 1 || pos == 2))
    {
        if(pos == 1)
        {
            for(i = tam-1; i >= 0; i--)
                string[i+1] = string[i];
            string[0] = '0';
        }
        else if(pos == 2)
        {
            string[4] = string[3];
            string[3] = '0';
        }

        string[5] = '\0';
        tam++;
    }
    // Formatando horas nos formatos 1:2 01:02
    else if(tam == 3 && pos == 1)
    {
        for(i = tam-1; i >= 0; i--)
            string[i+1] = string[i];
        string[4] = string[3];
        string[0] = '0';
        string[3] = '0';
        string[5] = '\0';
        tam = 5;
    }

    // Verifica se o horário é válido
    if(tam == TAM5-1 && string[2] == ':')
    {
        strncpy(aux, string, 2);
        aux[2] = '\0';
        x = atoi(aux);
        if(verificaInt(aux, 2) && x >= 0 && x <= 23)
        {
            for(i = 3; i < TAM5-1; i++)
                aux[i-3] = string[i];
            
            aux[2] = '\0';
            x = atoi(aux);
            if(verificaInt(aux, 2) && x >= 0 && x <= 59)
                return 1;
        }
    }
    return 0;
}

// Função para verificar se uma data é válida
// Recebe a String a ser verificada
int verificaData(char *string)
{
    char aux[4];
    int tam = strlen(string);

    if(tam == TAM4-1 && string[2] == '/' && string[5] == '/')
    {
        strncpy(aux, string, 2);
        if(verificaInt(aux, 2))
        {
            int dia, mes, ano, data;
            dia = atoi(aux);
            int i;
            for(i = 3; i < 5; i++)
                aux[i-3] = string[i];

            if(verificaInt(aux, 2))
            {
                mes = atoi(aux);
                for(i = 6; i < TAM4-1; i++)
                    aux[i-6] = string[i];
                
                if(verificaInt(aux, 4))
                {
                    ano = atoi(aux);
                    // Monta a data em uma variável, para poder verificar
                    data = (dia * 1000000) + (mes * 10000) + ano;
                    // Irá converter a data em juliana, e desconverter para gregoriana
                    long int gregoriana = julianaGregoriana(gregorianaJuliana(string));
                    // Caso a data digitada e a convertida sejam iguais, então são válidas
                    if(data == gregoriana)
                        return 1;
                }
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

///////////////////////////  ARQUIVO  ///////////////////////////

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

// Função para ler o arquivo do cliente e salvar as informações em um vetor
// Recebe uma variável para guardar o tamanho. Retorna o endereço do vetor
Cliente *lerArquivoCliente(int *tam)
{
    if(arquivoExiste(ARQCLIENTE))
    {
        FILE *arquivo = abrirArquivo(ARQCLIENTE, "rb");
        Cliente aux;


        Cliente *vetor = (Cliente *) malloc(sizeof(Cliente));
        *tam = 0;
        while(fread(&aux, sizeof(Cliente), 1, arquivo) == 1)
        {
            vetor[*tam] = aux;
            (*tam)++;
            // Aumenta o tamanho do vetor dinamicamente
            vetor = realloc(vetor, ((*tam) + 1) * sizeof(Cliente));
        }
        fclose(arquivo);

        return vetor;
    }
    *tam = -1;
    return NULL;
}

// Função para ler o arquivo da reserva e salvar as informações em um vetor
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

// Função para salvar as informações do cliente em um arquivo
// Recebe a variável da pessoa
int salvarCliente(Cliente pessoa)
{
    FILE *arquivo = abrirArquivo(ARQCLIENTE, "ab");

    if(fwrite(&pessoa, sizeof(Cliente), 1, arquivo) < 1)
    {
        printf("\nErro! Não foi possível salvar as informações\n");
        return 0;
    }
    fclose(arquivo);

    return 1;
}

// Função para salvar as informações da reserva em um arquivo
// Recebe a variável do item
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
void refazerArquivoCliente(Cliente *vetor, int tam)
{
    // FILE *arquivo2 = abrirArquivo("clienteBackup.bin", "wb");
    // for(int i = 0; i < tam; i++)
    //     fwrite(&vetor[i], sizeof(Cliente), 1, arquivo2);
    // fclose(arquivo2);

    FILE *arquivo = abrirArquivo(ARQCLIENTE, "wb");
    fclose(arquivo);

    for(int i = 0; i < tam; i++)
        salvarCliente(vetor[i]);
}

// Função para salvar a informação editada, reescrevendo o arquivo
// Recebe o endereço do vetor que guarda todas as informações e seu tamanho
void refazerArquivoReserva(Reserva *vetor, int tam)
{
    FILE *arquivo = abrirArquivo(ARQRESERVA, "wb");
    fclose(arquivo);

    for(int i = 0; i < tam; i++)
        salvarReserva(vetor[i]);
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

///////////////////////////  MENUS  ///////////////////////////

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

// Função para exibir um cliente
// Recebe a variavel do Cliente
void exibirCliente(Cliente pessoa)
{
    printf("\n----------Cliente----------");
    printf("\nNome: %s", pessoa.nome);
    printf("\nCPF: %s", pessoa.cpf);
    printf("\nRG: %s", pessoa.rg);
    printf("\nTelefone: %s", pessoa.telefone);
    printf("\nEndereço: %s", pessoa.endereco);
    printf("\nEmail: %s\n", pessoa.email);
}

// Função para exibir uma reserva
// Recebe a variavel da Reserva
void exibirReserva(Reserva item)
{
    printf("\n----------Reserva----------");
    printf("\nNúmero de reserva: %08d", item.numReserva);
    printf("\nNúmero do quarto: %d", item.numQuarto);
    printf("\nNome do cliente: %s", item.nome);
    printf("\nCPF: %s", item.cpf);
    printf("\nData de entrada: %s", item.dataEntrada);
    printf("\nData de saída: %s", item.dataSaida);

    if(strcmp(item.horaEntrada, "0"))
        printf("\nHora de entrada: %s", item.horaEntrada);

    if(strcmp(item.horaSaida, "0"))
        printf("\nHora de saída: %s", item.horaSaida);

    printf("\nTotal: R$%.2f", item.total);

    if(item.pagamento == 0)
        printf("\nStatus de pagamento: Pendente\n");
    else
        printf("\nStatus de pagamento: Pago\n");
}

// Função para exibir o menu principal do hotel
// Retorna a opção escolhida
int menu()
{
    int op;
    printf("\n----------HOTEL----------");
    printf("\n[1] - Seção dos quartos");
    printf("\n[2] - Seção dos clientes");
    printf("\n[3] - Seção das reservas");
    printf("\n[0] - Finalizar programa");
    op = leiaInt("\nOpção: ");
    return op;
}

// Função para exibir o menu de opções da seção cliente
// Retorna a opção escolhida
int menuCliente()
{
    int op;
    printf("\n-----SEÇÃO DO CLIENTE-----");
    printf("\n[1] - Cadastrar cliente");
    printf("\n[2] - Consultar cliente");
    printf("\n[3] - Editar cliente");
    printf("\n[4] - Excluir cliente");
    printf("\n[5] - Exibir todos");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");
    return op;
}

// Função para exibir o menu de opções para buscar/editar o cliente
// Retorna a opção escolhida
int menuEditCliente()
{
    int op;
    printf("\n[1] - Nome");
    printf("\n[2] - CPF");
    printf("\n[3] - RG");
    printf("\n[4] - Telefone");
    printf("\n[5] - Endereço");
    printf("\n[6] - Email");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");

    return op;
}

// Função para exibir o menu de opções da seção reserva
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
    printf("\n[7] - Exibir todas as reservas");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");
    return op;
}

// Função para exibir o menu de opções para buscar a reserva
// Retorna a opção escolhida
int menuBuscaReserva()
{
    int op;
    printf("\n[1] - Nome");
    printf("\n[2] - Número da reserva");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");
    return op;
}

///////////////////////////  LEITURA  ///////////////////////////

// Função para ler o nome do cliente
// Recebe a variável que vai receber o nome
void leiaNome(char *nome)
{
    int tam, invalido;
    
    do
    {
        printf("\nNome: ");
        scanf(" %[^\n]s", nome);
        limpaBuffer();
        invalido = 0;
        tam = strlen(nome);

        for(int i = 0; i < tam; i++)
        {
            if(nome[i] >= '0' && nome[i] <= '9')
            {
                invalido = 1;
                break;
            }
        }
        if(invalido)
            printf("\nNome inválido!!\n");
    }while(invalido);

    maiusc(nome, nome, tam);
}

// Função para ler o CPF digitado e conferir se está em um formato válido 123.456.789-00 ou 12345678900
// Após a leitura, irá formata-lo e deixar no formato 123.456.789-00
// Recebe a variável que vai receber o CPF
void leiaCPF(char *cpf)
{
    char aux[TAM1];
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

// Função para ler o RG digitado e conferir se está em um formato válido 12.345.678-9 ou 123456789
// Após a leitura, irá formata-lo e deixar no formato 12.345.678-9
// Recebe a variável que vai receber o RG
void leiaRG(char *rg)
{
    char aux[TAM1];
    int tam, invalido = 1;

    do
    {
        printf("\nRG: ");
        scanf(" %[^\n]s", aux);
        limpaBuffer();

        tam = strlen(aux);

        // Caso em que foi digitado apenas o RG numérico - 123456789
        if((tam == TAM3-4) && verificaInt(aux, TAM3-4))
        {
            invalido = 0;
            // Formata o RG
            rg[2] = '.';
            rg[6] = '.';
            rg[10] = '-';
            for(int i = 0, j = 0; i < tam; i++, j++)
            {
                if(j == 2 || j == 6 || j == 10)
                    j++;
                rg[j] = aux[i];
            }
        }
        // Caso em que o RG foi digitado completo, verifica se é válido - 12.345.678-9
        else if(tam == TAM3-1)
        {
            // Verifica se os os números e símbolos estão na posição certa
            char aux2[TAM3-4];
            for(int i = 0, j = 0; i < tam; i++)
            {
                // Cria um vetor auxiliar apenas com os números
                if(j != 2 && j != 6 && j != 10)
                {
                    aux2[j] = aux[i];
                    j++;
                }
            }

            // Executa a verificação final se está formatado corretamente
            if(verificaInt(aux2, TAM3-4) && aux[2] == '.' && aux[6] == '.' && aux[10] == '-')
            {
                invalido = 0;
                strcpy(rg, aux);
            }
        }
        if(invalido)
            printf("\nRG inválido!!\n");
    }while(invalido);
    rg[TAM3-1] = '\0';
}

// Função para ler o telefone digitado e conferir se está em um formato válido (00)12345-6789 ou 00123456789 
// Após a leitura, irá formata-lo e deixar no formato (00)12345-6789
// Recebe a variável que vai receber o telefone
void leiaTelefone(char *telefone)
{
    char aux[TAM1];
    int tam, invalido = 1;

    do
    {
        printf("\nTelefone: ");
        scanf(" %[^\n]s", aux);
        limpaBuffer();

        tam = strlen(aux);

        // Caso em que foi digitado apenas o Telefone numérico - 00123456789
        if((tam == TAM2-4) && verificaInt(aux, TAM2-4))
        {
            invalido = 0;
            // Formata o Telefone
            telefone[0] = '(';
            telefone[3] = ')';
            telefone[9] = '-';
            for(int i = 0, j = 1; i < tam; i++, j++)
            {
                if(j == 3 || j == 9)
                    j++;
                telefone[j] = aux[i];
            }
        }
        // Caso em que o Telefone foi digitado completo, verificar se é válido - 123.456.789-00
        else if(tam == TAM2-1)
        {
            // Verifica se os números e símbolos estão na posição certa
            char aux2[TAM2-4];
            for(int i = 1, j = 0; i < tam; i++)
            {
                // Cria um vetor auxiliar apenas com os números
                if(i != 3 && i != 9)
                {
                    aux2[j] = aux[i];
                    j++;
                }
            }
            // Efetua a troca caso o telefone digitado contenha um espaço, para uma melhor estética
            if(aux[9] == ' ')
                aux[9] = '-';

            // Executa a verificação final se está formatado corretamente
            if(verificaInt(aux2, TAM2-4) && aux[0] == '(' && aux[3] == ')' && aux[9] == '-')
            {
                invalido = 0;
                strcpy(telefone, aux);
            }
        }
        if(invalido)
            printf("\nTelefone inválido!!\n");
    }while(invalido);
    telefone[TAM2-1] = '\0';
}

// Função para ler o endereço do cliente
// Recebe a variável que vai receber o endereço
void leiaEndereco(char *endereco)
{
    int tam;
    
    printf("\nEndereço: ");
    scanf(" %[^\n]s", endereco);
    limpaBuffer();

    tam = strlen(endereco);
    maiusc(endereco, endereco, tam);
}

// Função para ler o Email do cliente
// Não permite espaços, símbolos e acentuações no Email
// Recebe a variável que vai receber o Email
void leiaEmail(char *email)
{
    int tam, i, arroba;

    do
    {
        arroba = 0;
        printf("\nE-mail: ");
        scanf(" %[^\n]s", email);
        limpaBuffer();
        tam = strlen(email);
        for(i = 0; i < tam; i++)
        {
            if(email[i] == '@')
                arroba++;
            if(arroba > 1)
                break;
        }
        for(i = 0; i < tam; i++)
        {
            // Blindagem para o email aceitar apenas caracteres, números e os símbolos @ _ .
            if(arroba != 1 || (email[i] != '_' && email[i] != '@' && email[i] != '.' && (email[i] < 'a' || email[i] > 'z') && (email[i] < 'A' || email[i] > 'Z') && (email[i] < '0' || email[i] > '9')))
            {
                printf("\nEmail Inválido!!\n");
                break;
            }
        }
    }while(i != tam);

    maiusc(email, email, tam);
}

// Função para ler a Data digitada e conferir se está em um formato válido 12/34/5678 ou 12345678
// Após a leitura, irá formatá-la e deixar no formato 12/34/5678
// Recebe o texto a ser exibido e a variável que vai receber a data
void leiaData(char *texto, char *data)
{
    int tam, invalido = 1;
    char aux[TAM4];

    do
    {
        printf("%s", texto);
        scanf(" %[^\n]s", aux);
        limpaBuffer();

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
            data[TAM4-1] = '\0';
            if(verificaData(data))
                invalido = 0;
        }

        else if(verificaData(aux))
        {
            strcpy(data, aux);
            data[TAM4-1] = '\0';
            invalido = 0;
        }

        if(invalido)
            printf("\nData inválida!!\n");
    }while(invalido);
}

// Função para ler a Hora digitada e conferir se está em um formato válido 12:34 ou 1234
// Após a leitura, irá formatá-la e deixar no formato 12:34
// Recebe o texto a ser exibido e a variável que vai receber a hora
void leiaHora(char *texto, char *hora)
{
    char aux[TAM5];
    int tam, invalido = 1;

    do
    {
        printf("%s", texto);
        scanf(" %[^\n]s", aux);
        limpaBuffer();

        tam = strlen(aux);

        if(tam == TAM5-2 && verificaInt(aux, tam))
        {
            for(int i = 3; i > 1; i--)
                hora[i+1] = aux[i];

            hora[2] = ':';

            strncpy(hora, aux, 2);
            hora[TAM5-1] = '\0';
            if(verificaHora(hora))
            {
                invalido = 0;
            }
        }
        else if(verificaHora(aux))
        {
            strcpy(hora, aux);
            hora[TAM5-1] = '\0';
            invalido = 0;
        }
        if(invalido)
            printf("\nHora inválida!!\n");
    }while(invalido);
}

///////////////////////////  BUSCA  ///////////////////////////

// Função para buscar um quarto pelo seu número
// Recebe o endereço para "coletar" os dados do quarto, sua posição no vetor e o "user"
// Se o user for falso (0), irá pedir ao usuário para digitar o número do quarto buscado
// Se o user for verdadeiro (!= 0), o número buscado será o do próprio user
int buscaNumQuarto(Quarto *quarto, int *pos, int user)
{
    int tam, i, numero;
    Quarto *vetor = lerArquivoQuarto(&tam);

    if(user == 0)
    {
        numero = leiaInt("Número do quarto: ");
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

// Função para buscar um cliente pelo Nome
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int buscaNomeCliente(Cliente *pessoa, int *pos)
{
    int tam, tam2;
    Cliente *vetor = lerArquivoCliente(&tam);

    char nome[TAM1];
    char ch;

    leiaNome(nome);
    tam2 = strlen(nome);
    printf("\nBuscando por Nome: [%s]...\n", nome);
    for(int i = 0; i < tam; i++)
    {
        if(strncmp(nome, vetor[i].nome, tam2) == 0)
        {
            do
            {
                exibirCliente(vetor[i]);
                printf("\nEsse cliente? ");
                ch = getchar();
                limpaBuffer();
                if(ch == 'S' || ch == 's')
                {
                    *pos = i;
                    *pessoa = vetor[i];
                    return 1;
                }
                else if (ch == 'N' || ch == 'n')
                    printf("\nBuscando por Nome: [%s]...\n", nome);
                else
                    printf("\nOpção inválida!!\n");
            }while(ch != 'N' && ch != 'n');
        }
    }
    printf("Não foi encontrado ninguém com esse nome!!\n");
    free(vetor);
    return 0;
}

// Função para buscar um cliente pelo CPF
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int buscaCPF(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    char cpf[TAM2];

    leiaCPF(cpf);
    strcpy(pessoa->cpf, cpf);
    printf("\nBuscando por CPF: [%s]...\n", cpf);

    for(int i = 0; i < tam; i++)
    {
        if(strcmp(cpf, vetor[i].cpf) == 0)
        {
            *pos = i;
            *pessoa = vetor[i];
            return 1;
        }
    }
    printf("Não foi encontrado ninguém com esse CPF!!\n");
    free(vetor);
    return 0;
}

// Função para buscar um cliente pelo RG
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int buscaRG(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    char rg[TAM3];

    leiaRG(rg);
    printf("\nBuscando por CPF: [%s]...\n", rg);

    for(int i = 0; i < tam; i++)
    {
        if(strcmp(rg, vetor[i].rg) == 0)
        {
            *pos = i;
            *pessoa = vetor[i];
            return 1;
        }
    }

    printf("Não foi encontrado ninguém com esse RG!!\n");
    free(vetor);
    return 0;
}

// Função para buscar um cliente pelo Telefone
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int buscaTelefone(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    char telefone[TAM2];

    leiaTelefone(telefone);
    printf("\nBuscando por Telefone: [%s]...\n", telefone);

    for(int i = 0; i < tam; i++)
    {
        if(strcmp(telefone, vetor[i].telefone) == 0)
        {
            *pos = i;
            *pessoa = vetor[i];
            return 1;
        }
    }

    printf("Não foi encontrado ninguém com esse Telefone!!\n");
    free(vetor);
    return 0;
}

// Função para buscar um cliente pelo Endereço
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int buscaEndereco(Cliente *pessoa, int *pos)
{
    int tam, tam2;
    Cliente *vetor = lerArquivoCliente(&tam);

    char endereco[TAM1];
    char ch;

    leiaEndereco(endereco);
    tam2 = strlen(endereco);
    printf("\nBuscando por Endereco: [%s]...\n", endereco);
    for(int i = 0; i < tam; i++)
    {
        if(strncmp(endereco, vetor[i].endereco, tam2) == 0)
        {
            do
            {
                exibirCliente(vetor[i]);
                printf("\nEsse cliente? ");
                ch = getchar();
                limpaBuffer();
                if(ch == 'S' || ch == 's')
                {
                    *pos = i;
                    *pessoa = vetor[i];
                    return 1;
                }
                else if (ch == 'N' || ch == 'n')
                    printf("\nBuscando por Endereco: [%s]...\n", endereco);
                else
                    printf("\nOpção inválida!!\n");
            }while(ch != 'N' && ch != 'n');
        }
    }
    printf("Não foi encontrado ninguém com esse endereço!!\n");
    free(vetor);
    return 0;
}

// Função para buscar um cliente pelo Email
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int buscaEmail(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    char email[TAM1];

    leiaEmail(email);
    printf("\nBuscando por Email: [%s]...\n", email);

    for(int i = 0; i < tam; i++)
    {
        if(strcmp(email, vetor[i].email) == 0)
        {
            *pos = i;
            *pessoa = vetor[i];
            return 1;
        }
    }

    printf("Não foi encontrado ninguém com esse Email!!\n");
    free(vetor);
    return 0;
}

// Função para buscar uma reserva pelo número
// Recebe o endereço para "coletar" os dados da reserva e posição no vetor  
int buscaNumReserva(Reserva *item, int *pos)
{
    int tam;
    Reserva *vetor = lerArquivoReserva(&tam);
    int numero = leiaInt("\nNúmero da reserva: ");

    printf("\nBuscando por Reserva: [%08d]...\n", numero);
    for(int i = 0; i < tam; i++)
    {
        if(numero == vetor[i].numReserva)
        {
            *pos = i;
            *item = vetor[i];
            return 1;
        }
    }
    printf("Não foi encontrado nenhuma reserva com esse número!!\n");
    free(vetor);
    return 0;
}

// Função para buscar uma reserva pelo Nome do cliente
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int buscaNomeReserva(Reserva *item, int *pos)
{
    int tam, tam2;
    Reserva *vetor = lerArquivoReserva(&tam);

    char nome[TAM1];
    char ch;

    leiaNome(nome);
    tam2 = strlen(nome);
    printf("\nBuscando por Nome: [%s]...\n", nome);
    for(int i = 0; i < tam; i++)
    {
        if(strncmp(nome, vetor[i].nome, tam2) == 0)
        {
            do
            {
                exibirReserva(vetor[i]);
                printf("\nEssa Reserva? ");
                ch = getchar();
                limpaBuffer();
                if(ch == 'S' || ch == 's')
                {
                    *pos = i;
                    *item = vetor[i];
                    return 1;
                }
                else if (ch == 'N' || ch == 'n')
                    printf("\nBuscando por Nome: [%s]...\n", nome);
                else
                    printf("\nOpção inválida!!\n");
            }while(ch != 'N' && ch != 'n');
        }
    }
    printf("Não foi encontrado nenhuma reserva nesse nome!!\n");
    free(vetor);
    return 0;
}

///////////////////////////  CADASTRO  ///////////////////////////

// Função para cadastrar o CPF
// Recebe a variável que vai receber o CPF
void cadastrarCPF(char *cpf)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    int invalido;
    do
    {
        leiaCPF(cpf);
        invalido = 0;
        for(int i = 0; i < tam; i++)
        {
            if(strcmp(vetor[i].cpf, cpf) == 0)
            {
                invalido = 1;
                printf("\nErro! CPF já cadastrado\n");
                break;
            }
        }
    }while(invalido);
    free(vetor);
}

// Função para cadastrar o RG
// Recebe a variável que vai receber o RG
void cadastrarRG(char *rg)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    int invalido;
    do
    {
        leiaRG(rg);
        invalido = 0;
        for(int i = 0; i < tam; i++)
        {
            if(strcmp(vetor[i].rg, rg) == 0)
            {
                invalido = 1;
                printf("\nErro! RG já cadastrado\n");
                break;
            }
        }
    }while(invalido);
    free(vetor);
}

// Função para cadastrar o telefone
// Recebe a variável que vai receber o telefone
void cadastrarTelefone(char *telefone)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    int invalido;
    do
    {
        leiaTelefone(telefone);
        invalido = 0;
        for(int i = 0; i < tam; i++)
        {
            if(strcmp(vetor[i].telefone, telefone) == 0)
            {
                invalido = 1;
                printf("\nErro! Telefone já cadastrado\n");
                break;
            }
        }
    }while(invalido);
    free(vetor);
}

// Função para cadastrar o email
// Recebe a variável que vai receber o email
void cadastrarEmail(char *email)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    int invalido;
    do
    {
        leiaEmail(email);
        invalido = 0;
        for(int i = 0; i < tam; i++)
        {
            if(strcmp(vetor[i].email, email) == 0)
            {
                invalido = 1;
                printf("\nErro! Email já cadastrado\n");
                break;
            }
        }
    }while(invalido);
    free(vetor);
}

////////////////////////  SEÇÃO QUARTO  ////////////////////////



////////////////////////  SEÇÃO CLIENTE  ////////////////////////

// Função para CADASTRAR cliente
// Recebe o endereço para "coletar" os dados do cliente e o "cpf"
// Se o cpf for verdadeiro (!= 0) o usuário não precisa cadastrar o cpf 
void cadastrarCliente(Cliente *pessoa, int cpf)
{
    Cliente aux;

    printf("\nCADASTRO DE CLIENTE\n");
    leiaNome(aux.nome);
    if(cpf == 0)
        cadastrarCPF(aux.cpf);
    else
        strcpy(aux.cpf, pessoa->cpf);
    cadastrarRG(aux.rg);
    cadastrarTelefone(aux.telefone);
    leiaEndereco(aux.endereco);
    cadastrarEmail(aux.email);

    if(salvarCliente(aux))
    {
        printf("\nCliente cadastrado com sucesso!\n");
        *pessoa = aux;
    }
}

// Função para CONSULTAR cliente
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int consultarCliente(Cliente *pessoa, int *pos)
{
    int op, tam;
    Cliente *vetor = lerArquivoCliente(&tam);
    if(vetor != NULL && tam > 0)
    {
        do
        {
            printf("\n-----Buscar cliente por:-----");
            op = menuEditCliente();

            switch(op)
            {
                case 1:
                    if(buscaNomeCliente(pessoa, pos) == 0)
                        op = 0;
                    break;

                case 2:
                    if(buscaCPF(pessoa, pos))
                        exibirCliente(*pessoa);
                    else
                        op = 0;
                    break;

                case 3:
                    if(buscaRG(pessoa, pos))
                        exibirCliente(*pessoa);
                    else
                        op = 0;
                    break;

                case 4:
                    if(buscaTelefone(pessoa, pos))
                        exibirCliente(*pessoa);
                    else
                        op = 0;
                    break;

                case 5:
                    if(buscaEndereco(pessoa, pos) == 0)
                        op = 0;
                    break;

                case 6:
                    if(buscaEmail(pessoa, pos))
                        exibirCliente(*pessoa);
                    else
                        op = 0;
                    break;

                case 0:
                    printf("\nVoltando...\n");
                    break;

                default:
                    printf("\nOpção inválida!\n");
            }
        }while(op < 0 || op > 6);
        return op;
    }
    free(vetor);
    printf("\nNinguém foi cadastrado ainda!!\n");
    return 0;
}

// Função para EDITAR cliente
void editarCliente()
{
    int op, pos, mudou = 0;
    Cliente pessoa, pessoa2, pessoa3;

    if(consultarCliente(&pessoa, &pos))
    {
        pessoa3 = pessoa;
        do
        {
            printf("\n-----Editar-----");
            op = menuEditCliente();
            switch(op)
            {
                case 1:
                    leiaNome(pessoa2.nome);
                    strcpy(pessoa.nome, pessoa2.nome);
                    mudou = 1;
                    break;

                case 2:
                    cadastrarCPF(pessoa2.cpf);
                    strcpy(pessoa.cpf, pessoa2.cpf);
                    mudou = 2;
                    break;

                case 3:
                    cadastrarRG(pessoa2.rg);
                    strcpy(pessoa.rg, pessoa2.rg);
                    break;

                case 4:
                    cadastrarTelefone(pessoa2.telefone);
                    strcpy(pessoa.telefone, pessoa2.telefone);
                    break;

                case 5:
                    leiaEndereco(pessoa2.endereco);
                    strcpy(pessoa.endereco, pessoa2.endereco);
                    break;

                case 6:
                    cadastrarEmail(pessoa2.email);
                    strcpy(pessoa.email, pessoa2.email);
                    break;
                case 0:
                    printf("\nVoltando...\n");
                    break;

                default:
                    printf("\nOpção inválida!\n");
            }
        }while(op < 0 || op > 6);

        if(op > 0)
        {
            printf("\nDados alterados com sucesso\n");
            exibirCliente(pessoa);
            // Lê todos os dados do arquivo
            int tam;
            Cliente *vetor = lerArquivoCliente(&tam);
            vetor[pos] = pessoa;

            // Salva a informação editada, reescrevendo o arquivo
            refazerArquivoCliente(vetor, tam);
            free(vetor);

            if(mudou)
            {
                int tamReserva;
                Reserva *vetorReserva = lerArquivoReserva(&tamReserva);

                for(int i = 0; i < tamReserva; i++)
                {
                    if(strcmp(pessoa3.cpf, vetorReserva[i].cpf) == 0)
                    {
                        if(mudou == 1)
                            strcpy(vetorReserva[i].nome, pessoa.nome);
                        else
                            strcpy(vetorReserva[i].cpf, pessoa.cpf);

                    }
                }
                
                refazerArquivoReserva(vetorReserva, tamReserva);
                free(vetorReserva);
            }
        }
    }
}

// Função para EXCLUIR cliente
void excluirCliente()
{
    int pos, tam, tamReserva, i;
    char ch;
    Cliente pessoa;
    Cliente *vetor = lerArquivoCliente(&tam);
    Reserva *vetorReserva = lerArquivoReserva(&tamReserva);

    if(consultarCliente(&pessoa, &pos))
    {
        for(i = 0; i < tamReserva; i++)
        {
            if(strcmp(vetorReserva[i].cpf, pessoa.cpf) == 0)
                break;
        }
        if(i == tamReserva)
        {

            do
            {
                printf("Tem certeza que deseja excluir? Os dados desse cliente serão perdidos para sempre\n");
                ch = getchar();
                limpaBuffer();
                if(ch == 'S' || ch == 's')
                {
                    tam--;
                    for(int i = pos; i < tam; i++)
                        vetor[i] = vetor[i+1];
                    
                    refazerArquivoCliente(vetor, tam);
                    printf("\nCliente excluído com sucesso\n");
                }
                else if (ch == 'N' || ch == 'n')
                    printf("\nCancelando...\n");
                else
                    printf("\nOpção inválida!!\n");
            }while(ch != 'N' && ch != 'n' && ch != 's' && ch != 'S');
        }
        else
            printf("\nNão é possível excluir um cliente que efetuou uma reserva\n");
    }
    free(vetor);
}

// Função para EXIBIR TODOS os clientes
void exibirTodosClientes()
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    if(vetor != NULL && tam > 0)
    {
        for(int i = 0; i < tam; i++)
            exibirCliente(vetor[i]);
    }
    else
        printf("\nNinguém foi cadastrado ainda!!\n");
    free(vetor);
}

// Função principal dos clientes
int secaoCliente()
{
    int op, pos;
    Cliente pessoa;

    do
    {
        op = menuCliente();
        switch(op)
        {
            case 1:
                cadastrarCliente(&pessoa, 0);
                break;
            
            case 2:
                consultarCliente(&pessoa, &pos);
                break;

            case 3:
                editarCliente();
                break;
            
            case 4:
                excluirCliente();
                break;

            case 5:
                exibirTodosClientes();
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

////////////////////////  SEÇÃO RESERVA  ////////////////////////

// Função para CADASTRAR/REALIZAR reserva
int realizarReserva()
{
    Reserva item;
    Cliente pessoa;
    Quarto quarto;
    int quantDias, pos, pos2, i, j;

    do
    {
        leiaData("\nData de entrada: ", item.dataEntrada);
        leiaData("\nData de saída: ", item.dataSaida);
        quantDias = comparaData(item.dataEntrada, item.dataSaida);
        if(quantDias < 0)
            printf("\nDatas inválidas! Digite uma data de entrada menor que a data de saída\n");
    }while(quantDias < 0);
    
    int tamQuarto, tamReserva;
    Quarto *vetorQuarto = lerArquivoQuarto(&tamQuarto);
    Reserva *vetorReserva = lerArquivoReserva(&tamReserva);
    int livres[tamQuarto], quant;

    // Exibir quartos livres no período descrito
    do
    {
        quant = 0;
        printf("\nQuartos livres nesse período: ");
        for(i = 0; i < tamQuarto; i++)
        {
            // Exibe os quartos com status livres
            if(vetorQuarto[i].status == LIVRE)
            {
                exibirQuarto(vetorQuarto[i], 0);
                livres[quant] = vetorQuarto[i].numero;
                quant++;
            }
            // Exibe os quartos com status reservado, mas estão livres nessa data
            else if(vetorQuarto[i].status == RESERVADO)
            {
                for(j = 0; j < tamReserva; j++)
                {
                    // O quarto está livre caso a data inserida comece e termine ANTES da entrada que está salva nas outras reservas
                    // OU, o quarto está livre caso a data inserida comece e termine DEPOIS da saída que está salva nas outras reservas
                    if(!((gregorianaJuliana(item.dataEntrada) < gregorianaJuliana(vetorReserva[j].dataEntrada) && gregorianaJuliana(item.dataSaida) < gregorianaJuliana(vetorReserva[j].dataEntrada)) || (gregorianaJuliana(item.dataEntrada) > gregorianaJuliana(vetorReserva[j].dataSaida) && gregorianaJuliana(item.dataSaida) > gregorianaJuliana(vetorReserva[j].dataSaida))))
                        break;
                }
                // Caso a nova reserva não entre em conflito com as existentes, exibe o quarto como livre
                if(j == tamReserva)
                {
                    exibirQuarto(vetorQuarto[i], 0);
                    livres[quant] = vetorQuarto[i].numero;
                    quant++;
                }
            }
        }
        // Caso não existam quartos livres nesse perídoo, a reserva é cancelada
        if(quant == 0)
        {
            printf("\nNão há quartos livres nesse período\n");
            printf("\nCancelando reserva...\n");
            return 0;
        }
        // Após exibir os quartos livres, o usuário deve digitar sua escolha
        item.numQuarto = leiaInt("\nDigite o número do quarto: ");
        // Verifica a disponibilidade do quarto
        for(i = 0; i < quant; i++)
        {
            if(item.numQuarto == livres[i])
                break;
        }
        if(i == quant)
            printf("\nNão é possível escolher esse quarto!!\n");
    }while(i == quant);
    
    free(vetorReserva);
    free(vetorQuarto);

    // CPF é digitado e o programa busca por clientes cadastrados
    // Caso não esteja, cadastra o cliente
    if(buscaCPF(&pessoa, &pos2) == 0)
    {
        char ch;
        do
        {
            printf("\nVocê não está cadastrado. Deseja se cadastrar?\n");
            ch = getchar();
            limpaBuffer();
            if(ch == 'S' || ch == 's')
                cadastrarCliente(&pessoa, 1);
            else if (ch == 'N' || ch == 'n')
            {
                printf("\nCancelando reserva...\n");
                return 0;
            }
            else
                printf("\nOpção inválida!!\n");
        }while(ch != 'N' && ch != 'n' && ch != 'S' && ch != 's');
    }
    else
        printf("\nCliente [%s] já possui um cadastro\n", pessoa.nome);

    // Salva o nome e cpf do cliente
    strcpy(item.nome, pessoa.nome);
    strcpy(item.cpf, pessoa.cpf);

    // Como não foi realizado Check-in nem Check-out, os horários não podem ser definidos agora
    strcpy(item.horaEntrada, "0");
    strcpy(item.horaSaida, "0");

    // Coloca o quarto como reservado
    buscaNumQuarto(&quarto, &pos, item.numQuarto);
    quarto.status = RESERVADO;
    // Mesmo que o cliente entra e saia no mesmo dia, o preço mínimo a se pagar é o de 1 diária
    if(quantDias == 0)
        quantDias++;
    item.total = quantDias * quarto.preco; // Total - Calcula automático
    item.pagamento = PENDENTE;

    // Gera a "semente" para aleatorizar os números
    srand(time(NULL));
    
    vetorReserva = lerArquivoReserva(&tamReserva);
    do // Gera automático - Verifica se não é repetido
    {
        item.numReserva = (rand() % 99999999) + 1; // Gera de 1 a 99999999
        if(vetorReserva != NULL && tamReserva > 0)
        {
            for(i = 0; i < tamReserva; i++)
            {
                if(vetorReserva[i].numReserva == item.numReserva)
                    break;
            }
        }
    }while(vetorReserva != NULL && i < tamReserva);
    free(vetorReserva);

    if(salvarReserva(item))
        printf("\nReserva feita com sucesso!!\n");

    vetorQuarto = lerArquivoQuarto(&tamQuarto);
    vetorQuarto[pos] = quarto;
    refazerArquivoQuarto(vetorQuarto, tamQuarto);
    free(vetorQuarto);
    return 1;
}

// Função para CONSULTAR reserva
// Recebe o endereço para "coletar" os dados da reserva e posição no vetor  
int consultarReservas(Reserva *item, int *pos)
{
    int op, tam;
    Reserva *vetor = lerArquivoReserva(&tam);
    if(vetor != NULL && tam > 0)
    {
        do
        {
            printf("\n-----Buscar reserva por:-----");
            op = menuBuscaReserva();

            switch(op)
            {
                case 1:
                    if(buscaNomeReserva(item, pos) == 0)
                        op = 0;
                    break;

                case 2:
                    if(buscaNumReserva(item, pos))
                        exibirReserva(*item);
                    else
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
    printf("\nNão há nenhuma reserva ainda!!\n");
    return 0;
}

// Função para EXCLUIR reserva
void excluirReserva()
{
    int posReserva, posQuarto, tamReserva, tamQuarto, i;
    char ch;
    Reserva item;
    Reserva *vetorReserva = lerArquivoReserva(&tamReserva);
    Quarto quarto;
    Quarto *vetorQuarto = lerArquivoQuarto(&tamQuarto);

    if(consultarReservas(&item, &posReserva))
    {

        buscaNumQuarto(&quarto, &posQuarto, item.numQuarto);
        if(item.pagamento == PENDENTE)
        {
            if(strcmp(item.horaEntrada, "0") == 0)
            {
                do
                {
                    printf("Tem certeza que deseja excluir? Os dados dessa reserva serão perdidos para sempre\n");
                    ch = getchar();
                    limpaBuffer();
                    if(ch == 'S' || ch == 's')
                    {
                        tamReserva--;
                        for(i = posReserva; i < tamReserva; i++)
                            vetorReserva[i] = vetorReserva[i+1];
                            
                        refazerArquivoReserva(vetorReserva, tamReserva);
                            printf("\nReserva excluída com sucesso\n");

                        vetorReserva = lerArquivoReserva(&tamReserva);
                        for(i = 0; i < tamReserva; i++)
                        {
                            if(item.numQuarto == vetorReserva[i].numQuarto && vetorReserva[i].pagamento == PENDENTE && strcmp(item.dataEntrada, vetorReserva[i].dataEntrada) != 0 && strcmp(item.dataSaida, vetorReserva[i].dataSaida) != 0)
                                break;
                        }
                        if(i == tamReserva)
                        {
                            quarto.status = LIVRE;
                            vetorQuarto[posQuarto] = quarto;
                            refazerArquivoQuarto(vetorQuarto, tamQuarto);
                        }
                    }
                    else if (ch == 'N' || ch == 'n')
                        printf("\nCancelando...\n");
                    else
                        printf("\nOpção inválida!!\n");
                }while(ch != 'N' && ch != 'n' && ch != 's' && ch != 'S');
            }
            else
                printf("\nNão é possível excluir uma reserva de um cliente que já efetuou check-in!!\n");
        }
        else
            printf("\nNão é possível excluir uma reserva em que o cliente já efetuou o pagamento!!\n");
        free(vetorReserva);
        free(vetorQuarto);
    }
}

// Função para REALIZAR CHECK-IN
void realizarCheckin()
{
    int tamReserva, tamQuarto, posReserva, posQuarto;

    Reserva item;
    Reserva *vetorReserva = lerArquivoReserva(&tamReserva);

    Quarto quarto;
    Quarto *vetorQuarto = lerArquivoQuarto(&tamQuarto);

    if(consultarReservas(&item, &posReserva))
    {
        buscaNumQuarto(&quarto, &posQuarto, item.numQuarto);
        if(item.pagamento == PENDENTE && strcmp(item.horaEntrada, "0") == 0)
        {
            if(quarto.status == RESERVADO)
            {
                leiaHora("\nHora de entrada: ", item.horaEntrada);
                vetorReserva[posReserva] = item;

                quarto.status = OCUPADO;
                vetorQuarto[posQuarto] = quarto;

                refazerArquivoQuarto(vetorQuarto, tamQuarto);
                refazerArquivoReserva(vetorReserva, tamReserva);
                printf("\nCheck-in feito com sucesso!!\n");
            }
            else if(quarto.status == OCUPADO)
                printf("\nNo momento o quarto está ocupado, efetue o check-in mais tarde\n");
        }
        else if(strcmp(item.horaEntrada, "0") != 0)
            printf("\nVocê já realizou check-in anteriormente\n");
        else if(item.pagamento == PAGO)
            printf("\nNão é possível realizar check-in em reservas que o cliente já efetuou o pagamento\n");
    }
    free(vetorQuarto);
    free(vetorReserva);
}

// Função para REALIZAR pagamento / CHECK-OUT
void realizarPagamento()
{
    int quantMinutos, tamReserva, tamQuarto, posReserva, posQuarto;

    Reserva item;
    Reserva *vetorReserva = lerArquivoReserva(&tamReserva);

    Quarto quarto;
    Quarto *vetorQuarto = lerArquivoQuarto(&tamQuarto);

    
    if(consultarReservas(&item, &posReserva))
    {
        buscaNumQuarto(&quarto, &posQuarto, item.numQuarto);
        if(item.pagamento == PENDENTE && strcmp(item.horaSaida, "0") == 0 && strcmp(item.horaEntrada, "0") != 0)
        {
            if(quarto.status == OCUPADO)
            {
                do
                {
                    leiaHora("\nHora de saída: ", item.horaSaida);
                    quantMinutos = comparaHora(item.horaEntrada, item.horaSaida);
                    if((strcmp(item.dataEntrada, item.dataSaida) == 0) && quantMinutos < 0)
                        printf("\nHoras inválidas! Digite uma hora de saída maior que a hora de entrada [%s]\n", item.horaEntrada);
                }while((strcmp(item.dataEntrada, item.dataSaida) == 0) && quantMinutos < 0);
                
                printf("\nValor total da estadia: R$%.2f\n", item.total);
                item.pagamento = PAGO;
                
                vetorReserva[posReserva] = item;

                int i;
                // Verifica se existe mais alguma reserva registrada.
                // Caso exista, o status do quarto troca de OCUPADO para RESERVADO
                for(i = 0; i < tamReserva; i++)
                {
                    if(item.numQuarto == vetorReserva[i].numQuarto && vetorReserva[i].pagamento == PENDENTE && strcmp(item.dataEntrada, vetorReserva[i].dataEntrada) != 0 && strcmp(item.dataSaida, vetorReserva[i].dataSaida) != 0)
                        break;
                }
                if(i == tamReserva)
                    quarto.status = LIVRE;
                else
                    quarto.status = RESERVADO;
                
                vetorQuarto[posQuarto] = quarto;

                refazerArquivoQuarto(vetorQuarto, tamQuarto);
                refazerArquivoReserva(vetorReserva, tamReserva);
                printf("\nPagamento feito com sucesso!!\n");
            }
            else if(quarto.status == RESERVADO)
                printf("\nNão é possível realizar pagamento sem efetuar Check-in\n");
        }
        else if(item.pagamento == PAGO && strcmp(item.horaEntrada, "0") != 0)
            printf("\nPagamento já foi efetuado anteriormente\n");
        else if(strcmp(item.horaEntrada, "0") == 0)
                printf("\nNão é possível realizar pagamento sem efetuar Check-in\n");
    }
    free(vetorQuarto);
    free(vetorReserva);
}

// Função para EXIBIR os valores recebidos dentro de um período
void valoresRecebidos()
{
    char inicio[TAM1], fim[TAM1];

    int tam;
    Reserva *vetor = lerArquivoReserva(&tam);
    if(vetor != NULL && tam > 0)
    {
        do
        {
            leiaData("\nInício do período: ", inicio);
            leiaData("\nFim do período: ", fim);
            
            if(comparaData(inicio, fim) < 0)
                printf("\nPeríodo inválido! Digite um início menor que o fim\n");
        }while(comparaData(inicio, fim) < 0);

        float total = 0;
        printf("\nLUCROS DO PERÍODO: [%s - %s]", inicio, fim);
        printf("\n---------------------------------------------");
        for(int i = 0; i < tam; i++)
        {
            if(vetor[i].pagamento == PAGO && gregorianaJuliana(vetor[i].dataSaida) >= gregorianaJuliana(inicio) && gregorianaJuliana(vetor[i].dataSaida) <= gregorianaJuliana(fim))
            {
                printf("\nReserva de %s a %s - R$%.2f", vetor[i].dataEntrada, vetor[i].dataSaida, vetor[i].total);
                total += vetor[i].total;
            }
        }
        printf("\n---------------------------------------------");
        printf("\nTotal recebido: R$%.2f\n", total);
    }
    else
        printf("\nNão há nenhuma reserva ainda!!\n");
    free(vetor);
}

// Função para exibir TODAS as reservas
void exibirTodasReservas()
{
    int tam;
    Reserva *vetor = lerArquivoReserva(&tam);

    if(vetor != NULL && tam > 0)
    {
        for(int i = 0; i < tam; i++)
            exibirReserva(vetor[i]);
    }
    else
        printf("\nNão há nenhuma reserva ainda!!\n");
    free(vetor);
}

// Função principal das reservas
int secaoReserva()
{
    Reserva item;
    int op, pos;

    do
    {
        op = menuReserva();
        switch (op)
        {
            case 1:
                realizarReserva();
                break;

            case 2:
                excluirReserva();
                break;
                
            case 3:
                realizarCheckin();
                break;
                
            case 4:
                realizarPagamento();
                break;
                
            case 5:
                consultarReservas(&item, &pos);
                break;
                
            case 6:
                valoresRecebidos();
                break;
                
            case 7:
                exibirTodasReservas();
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

////////////////////////  INT MAIN  ////////////////////////
int main()
{
    int op;
    do
    {
        op = menu();
        
        switch(op)
        {
            case 1:
                // secaoQuarto();
                break;

            case 2:
                secaoCliente();
                break;

            case 3:
                secaoReserva();
                break;

            case 0:
                printf("\nFinalizando programa...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }
    }while(op != 0);

    return 0;
}
