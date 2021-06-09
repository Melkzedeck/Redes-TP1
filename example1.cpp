/*
 * [dkbhit.c]
 * Simula a função kbhit().
 *
 * [Autor]
 * Daemonio (Marcos Paulo Ferreira)
 * undefinido at gmail com
 * https://daemoniolabs.wordpress.com
 *
 * Versão 1.0, by daemonio @ Thu Dec 27 20:40:22 BRST 2012
 */
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

void init_attr(void) ;
void close_attr(void) ;
int kbhit(void) ;

struct termios old_attr, new_attr;

void init_attr(void) {
    /* Obtém as configurações atuais. */
    tcgetattr(0,&old_attr);
    new_attr=old_attr;

    /* Desliga modo canônico. */
    new_attr.c_lflag &=~ICANON ;

    /* Desliga ecoamento. */
    new_attr.c_lflag &= ~ECHO;

    new_attr.c_cc[VTIME]=0 ;
    new_attr.c_cc[VMIN]=0 ;
}

/* Retorna configurações antigas. */
void close_attr(void) {
    tcsetattr(STDIN_FILENO,TCSANOW,&old_attr);
}

int kbhit(void) {
    int c ;

    tcsetattr(STDIN_FILENO,TCSANOW,&new_attr);
    c = getchar() ; /* retorna EOF se nada foi pressionado */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_attr);

    return c ;
}

int main(void) {
    int flag_ler_nome = 0;
    char nome[20] ;

    init_attr() ;

    while(1) {
        flag_ler_nome = kbhit() ;

        if(flag_ler_nome != EOF) {
            printf("Digite seu nome: ") ;
            fgets(nome, sizeof nome, stdin) ;
        } 

        printf("** Outros processamentos **\n") ;
    }

    close_attr() ;

    return 0;
}