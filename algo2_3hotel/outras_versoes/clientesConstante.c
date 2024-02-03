#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIENTE "clientes.txt"
#define TAM1 100 // Tamanho do Nome e Email
#define TAM2 15 // Tamanho do CPF e Telefone
#define TAM3 13 // Tamanho do RG

typedef struct
{
    char nome[TAM1];
    char cpf[TAM2]; // 123.456.789-00
    char rg[TAM3];  // 12.345.678-9
    char telefone[TAM2]; // (00)12345-6789
    char email[TAM1];
} Cliente;

///////////////////////////  OUTROS  //////////////////////////////////////////

// Função para limpar o Buffer (caracteres excedentes)
void limpaBuffer()
{
    while(getchar() != '\n');
}

// Função para ler apenas números inteiros
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
void maiusc(char *string, char *string2, int tam)
{
    for(int i = 0; i < tam; i++)
    {
        if(string[i] >= 'a' && string[i] <= 'z')
            string2[i] = string[i] - ('a' - 'A');
    }
}

// Função para exibir um cliente
void exibirCliente(Cliente pessoa)
{
    printf("\nNome: %s", pessoa.nome);
    printf("\nCPF: %s", pessoa.cpf);
    printf("\nRG: %s", pessoa.rg);
    printf("\nTelefone: %s", pessoa.telefone);
    printf("\nEmail: %s\n", pessoa.email);
}

// Função para exibir o menu de opções da seção cliente
int menuCliente()
{
    int op;
    printf("\n-----SEÇÃO DO CLIENTE-----");
    printf("\n[1] - Cadastrar cliente");
    printf("\n[2] - Consultar cliente");
    printf("\n[3] - Editar cliente");
    printf("\n[4] - Excluir cliente");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");

    return op;
}

// Função para exibir o menu de opções para editar
int menuEditCliente()
{
    int op;
    printf("\n[1] - Nome");
    printf("\n[2] - CPF");
    printf("\n[3] - RG");
    printf("\n[4] - Telefone");
    printf("\n[5] - Email");
    printf("\n[0] - Voltar");
    op = leiaInt("\nOpção: ");

    return op;
}

///////////////////////////  ARQUIVO  //////////////////////////////////////////

// Função para abrir arquivos
FILE *abrirArquivo(char *nome, char *modo)
{
    FILE *arquivo = fopen(nome, modo);

    if(arquivo == NULL)
    {
        printf("\nErro ao abrir o arquivo %s", nome);
        exit(EXIT_FAILURE);
    }
    
    return arquivo;
}

// Função para ler um arquivo e salvar as informações em um vetor
Cliente *lerArquivoClinete(int *tam)
{
    FILE *arquivo = abrirArquivo(CLIENTE, "r");
    Cliente aux;

    for(*tam = 0; fscanf(arquivo, "%s %s %s %s %[^\n]s", aux.cpf, aux.rg, aux.telefone, aux.email, aux.nome) != -1; (*tam)++);

    rewind(arquivo);

    Cliente *vetor = (Cliente *) malloc((*tam) * sizeof(Cliente));
    for(int i = 0; i < *tam; i++)
    {
        fscanf(arquivo, "%s %s %s %s %[^\n]s", aux.cpf, aux.rg, aux.telefone, aux.email, aux.nome);
        vetor[i] = aux;
    }

    return vetor;
}

// Função para salvar as inforamções do cliente em um arquivo
int salvarCliente(Cliente pessoa)
{
    FILE *arquivo = abrirArquivo(CLIENTE, "a");

    if(fprintf(arquivo, "%s %s %s %s %s", pessoa.cpf, pessoa.rg, pessoa.telefone, pessoa.email, pessoa.nome) < 0)
    {
        printf("\nErro! Não foi possível salvar as informações\n");
        return 0;
    }
    fputc('\n', arquivo);
    fclose(arquivo);
    return 1;

}

// Salva a informação editada, reescrevendo o arquivo
void refazerArquivoCliente(Cliente *vetor, int tam)
{
    FILE *arquivo = abrirArquivo("clientes.txt", "w");
    fclose(arquivo);

    for(int i = 0; i < tam; i++)
        salvarCliente(vetor[i]);

    free(vetor);
}

///////////////////////////  LEITURA  //////////////////////////////////////////

// Função para ler o nome do cliente
void leiaNome(char *nome)
{
    int tam;
    
    printf("\nNome: ");
    scanf(" %[^\n]s", nome);
    limpaBuffer();

    tam = strlen(nome);
    maiusc(nome, nome, tam);
}

// Função para ler o CPF digitado e conferir se está em um formato válido 123.456.789-00 ou 12345678900
// Após a leitura, irá formata-lo e deixar no formato 123.456.789-00
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
}

// Função para ler o RG digitado e conferir se está em um formato válido 12.345.678-9 ou 123456789
// Após a leitura, irá formata-lo e deixar no formato 12.345.678-9
void leiaRG(char *rg)
{
    char aux[TAM3];
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
}

// Função para ler o telefone digitado e conferir se está em um formato válido (00)12345-6789 ou 00123456789 
// Após a leitura, irá formata-lo e deixar no formato (00)12345-6789
void leiaTelefone(char *tel)
{
    char aux[TAM2];
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
            tel[0] = '(';
            tel[3] = ')';
            tel[9] = '-';
            for(int i = 0, j = 1; i < tam; i++, j++)
            {
                if(j == 3 || j == 9)
                    j++;
                tel[j] = aux[i];
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
                strcpy(tel, aux);
            }
        }
        if(invalido)
            printf("\nTelefone inválido!!\n");
    }while(invalido);
    
}

// Função para ler o Email do cliente
// Não permite espaços no Email
void leiaEmail(char *email)
{
    int tam, i;

    do
    {
        printf("\nE-mail: ");
        scanf(" %[^\n]s", email);
        limpaBuffer();
        tam = strlen(email);
        for(i = 0; i < tam; i++)
        {
            if(email[i] == ' ')
            {
                printf("\nEmail Inválido!!\n");
                break;
            }
        }
    }while(i != tam);

    maiusc(email, email, tam);
}

///////////////////////////  BUSCA  //////////////////////////////////////////

// Função para buscar e exibir todos os clientes com um nome inserido
int buscaNome(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

    FILE *arquivo = abrirArquivo("clientes.txt", "r");
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
    fclose(arquivo);
    return 0;
}

// Função para buscar um cliente com pelo CPF
int buscaCPF(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

    FILE *arquivo = abrirArquivo("clientes.txt", "r");
    char cpf[TAM2];

    leiaCPF(cpf);
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
    fclose(arquivo);
    return 0;
}

// Função para buscar um cliente com pelo RG
int buscaRG(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

    FILE *arquivo = abrirArquivo("clientes.txt", "r");
    char rg[TAM2];

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

    printf("Não foi encontrado ninguém com esse CPF!!\n");
    fclose(arquivo);
    return 0;
}

// Função para buscar um cliente com pelo Telefone
int buscaTelefone(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

    FILE *arquivo = abrirArquivo("clientes.txt", "r");
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
    fclose(arquivo);
    return 0;
}

// Função para buscar um cliente com pelo Email
int buscaEmail(Cliente *pessoa, int *pos)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

    FILE *arquivo = abrirArquivo("clientes.txt", "r");
    char email[TAM2];

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
    fclose(arquivo);
    return 0;
}

///////////////////////////  CADASTRO  //////////////////////////////////////////
void leiaNome(char *nome)
{
    leiaNome(nome);
}

void cadastrarCPF(char *cpf)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

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

void cadastrarRG(char *rg)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

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

void cadastrarTelefone(char *telefone)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

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

void cadastrarEmail(char *email)
{
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);

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
void cadastrarCliente()
{
    Cliente pessoa;

    printf("\nCADASTRO DE CLIENTE\n");
    leiaNome(pessoa.nome);
    cadastrarCPF(pessoa.cpf);
    cadastrarRG(pessoa.rg);
    cadastrarTelefone(pessoa.telefone);
    cadastrarEmail(pessoa.email);

    if(salvarCliente(pessoa))
        printf("\nCliente cadastrado com sucesso!\n");

}

// Função para CONSULTAR cliente
int consultarCliente(Cliente *pessoa, int *pos)
{
    int op;

    do
    {
        printf("\nBuscar cliente por:");
        printf("\n[1] - Nome");
        printf("\n[2] - CPF");
        printf("\n[3] - RG");
        printf("\n[4] - Telefone");
        printf("\n[5] - Email");
        printf("\n[0] - Voltar");
        op = leiaInt("\nOpção: ");

        switch(op)
        {
            case 1:
                if(buscaNome(pessoa, pos) == 0)
                    op = 0;
                break;

            case 2:
                if(buscaCPF(pessoa, pos))
                    exibirCliente(*pessoa);
                break;

            case 3:
                if(buscaRG(pessoa, pos))
                    exibirCliente(*pessoa);
                break;

            case 4:
                if(buscaTelefone(pessoa, pos))
                    exibirCliente(*pessoa);
                break;

            case 5:
                if(buscaEmail(pessoa, pos))
                    exibirCliente(*pessoa);
                break;
            case 0:
                printf("\nVoltando...\n");
                break;

            default:
                break;
        }
    }while(op < 0 || op > 5);
    return op;
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
                    cadastrarEmail(pessoa2.email);
                    strcpy(pessoa.email, pessoa2.email);
                    break;
                case 0:
                    printf("\nVoltando...\n");
                    break;

                default:
                    break;
            }
        }while(op < 0 || op > 5);
    }

    // Lê todos os dados do arquivo
    int tam;
    Cliente *vetor = lerArquivoClinete(&tam);
    vetor[pos] = pessoa;

    // Salva a informação editada, reescrevendo o arquivo
    refazerArquivoCliente(vetor, tam);
}

// Função para EXCLUIR cliente
void excluirCliente()
{
    int pos, tam;
    Cliente pessoa;
    char ch;
    Cliente *vetor = lerArquivoClinete(&tam);

    if(consultarCliente(&pessoa, &pos))
    {
        do
        {
            printf("Tem certeza que deseja excluir? ");
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
}

int main()
{
    int op;

    do
    {
        op = menuCliente();
        switch(op)
        {
            case 1:
                cadastrarCliente();
                break;
            
            case 2:
                Cliente pessoa;
                int tam;
                consultarCliente(&pessoa, &tam);
                break;

            case 3:
                editarCliente();
                break;
            
            case 4:
                excluirCliente();
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