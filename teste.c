#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>

// Declaração das funções
void moveXY(int x, int y, char t[10]);
void menu();

// Variáveis globais para controlar a posição do cursor
int posX = 30;
int posY = 5;

int main()
{
    system("color 4F");
    char a;

    // Loop para manter o programa em execução até que o usuário escolha sair
    do
    {		
        // Limpa a tela e exibe o menu
        system("cls");
        menu();
        // Move o cursor para a posição atual
        moveXY(posX, posY, "->");

        // Captura a entrada do usuário
        a = toupper(getch());
		
        // Processa a entrada do usuário
        switch(a)
        {
            case 'W': // Tecla W para cima
                if (posY > 5)
                {
                    // Limpa a seta atual
                    moveXY(30, posY, "  ");
                    // Move para cima e exibe a seta
                    posY -= 5;
                    moveXY(30, posY, "->");
                    // Emite um som (se você tiver a função _beep implementada)
                    // _beep(400, 100);
                }
                break;

            case 'S': // Tecla S para baixo
                if (posY < 15)
                {
                    // Limpa a seta atual
                    moveXY(30, posY, "  ");
                    // Move para baixo e exibe a seta
                    posY += 5;
                    moveXY(30, posY, "->");
                    // Emite um som (se você tiver a função _beep implementada)
                    // _beep(400, 100);
                }
                break;
                
            // Adicione mais casos para processar outras teclas, se necessário

            case 27: // Tecla Esc (ASCII 27) para sair do programa
                exit(0);
                break;
        }
    } while(1); // Loop infinito até que o usuário escolha sair
}

// Função para mover o cursor para uma posição específica e imprimir um texto
void moveXY(int x, int y, char t[10])
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};
    SetConsoleCursorPosition(hStdout, position);
    printf("%s", t);
}

// Função para exibir o menu
void menu()
{
    moveXY(32, 1, "::: MENU MOVEL :::");	
    moveXY(33, 5, "1- Cadastro");
    moveXY(33, 10, "2- Relatorio");
    moveXY(33, 15, "3- Sair");
}
