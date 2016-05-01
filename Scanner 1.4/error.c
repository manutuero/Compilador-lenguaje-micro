#include <stdio.h>
#include "error.h"

void error_lexico(token tok){
    printf("Error lexico al recibir %s en la linea %d\n",yytext,nroLinea);
}

void error_sintactico(token tok){
    printf("Error sintactico en la linea %d se esperaba %s en vez de %s \n",nroLinea,tok_names[tok],yytext);
}
