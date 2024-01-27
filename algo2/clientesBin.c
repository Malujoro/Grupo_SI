#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQCLIENTE "clientes.bin"
#define TAM1 100 // Tamanho do Nome, Email e Endereço
#define TAM2 15 // Tamanho do CPF e Telefone
#define TAM3 13 // Tamanho do RG

typedef struct
{
    char nome[TAM1];
    char cpf[TAM2]; // 123.456.789-00
    char rg[TAM3];  // 12.345.678-9
    char telefone[TAM2]; // (00)12345-6789
    char endereco[TAM1];
    char email[TAM1];
} Cliente;

///////////////////////////  OUTROS  //////////////////////////////////////////

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

///////////////////////////  CLIENTE  //////////////////////////////////////////

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

// Função para exibir o menu de opções para buscar/editar
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

// Função para ler um arquivo e salvar as informações em um vetor
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

// Função para salvar a informação editada, reescrevendo o arquivo
// Recebe o endereço do vetor que guarda todas as informações e seu tamanho
void refazerArquivoCliente(Cliente *vetor, int tam)
{
    FILE *arquivo = abrirArquivo(ARQCLIENTE, "wb");
    fclose(arquivo);

    for(int i = 0; i < tam; i++)
        salvarCliente(vetor[i]);
}

///////////////////////////  LEITURA  //////////////////////////////////////////
/* ==============Sugestão - Usar o stringDinâmica para pegar os itens dos Leia ================= */

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

///////////////////////////  BUSCA  //////////////////////////////////////////

// Função para buscar um cliente pelo Nome
// Recebe o endereço para "coletar" os dados do cliente e posição no vetor  
int buscaNomeCliente(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    char nome[TAM1];
    char ch;

    leiaNome(nome);
    printf("\nBuscando por Nome: [%s]...\n", nome);
    for(int i = 0; i < tam; i++)
    {
        if(strcmp(nome, vetor[i].nome) == 0)
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

// Função para buscar um cliente com pelo CPF
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

// Função para buscar um cliente com pelo RG
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

// Função para buscar um cliente com pelo Telefone
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
    int tam;
    Cliente *vetor = lerArquivoCliente(&tam);

    char endereco[TAM1];
    char ch;

    leiaEndereco(endereco);
    printf("\nBuscando por Endereco: [%s]...\n", endereco);
    for(int i = 0; i < tam; i++)
    {
        if(strcmp(endereco, vetor[i].endereco) == 0)
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

// Função para buscar um cliente com pelo Email
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

///////////////////////////  CADASTRO  //////////////////////////////////////////

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

///////////////////////////  ESTRUTURA  //////////////////////////////////////////

// Função para CADASTRAR cliente
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
    printf("\nNinguém foi cadastrado ainda!!\n");
    return 0;
}

// Função para EDITAR cliente
void editarCliente()
{
    int op, pos;
    Cliente pessoa, pessoa2;

    if(consultarCliente(&pessoa, &pos))
    {
        do
        {
            printf("\n-----Editar-----");
            op = menuEditCliente();
            switch(op)
            {
                case 1:
                    leiaNome(pessoa2.nome);
                    strcpy(pessoa.nome, pessoa2.nome);
                    break;

                case 2:
                    cadastrarCPF(pessoa2.cpf);
                    strcpy(pessoa.cpf, pessoa2.cpf);
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
        }
    }
}

// Função para EXCLUIR cliente
void excluirCliente()
{
    int pos, tam;
    char ch;
    Cliente pessoa;
    Cliente *vetor = lerArquivoCliente(&tam);

    if(consultarCliente(&pessoa, &pos))
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
            }
            else if (ch == 'N' || ch == 'n')
                printf("\nCancelando...\n");
            else
                printf("\nOpção inválida!!\n");
        }while(ch != 'N' && ch != 'n' && ch != 's' && ch != 'S');
    }
    free(vetor);
}

// Função para exibir TODOS os clientes
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

int main()
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