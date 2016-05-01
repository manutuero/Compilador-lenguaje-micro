#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "symbol.h"
#include "error.h"


enum estadosDeTrabajo {EST_INICIAL, EST_RECONOCIENDO_ID, EST_RECONOCIENDO_CONSTANTE,
 EST_RECONOCIENDO_ASIGNACION, EST_RECONOCIENDO_DIVISION, EST_RECONOCIENDO_COMENTARIO, EST_RECONOCIENDO_ERRORCTE};

enum estadosAceptores {EST_ID_RECONOCIDO = 50, EST_CONSTANTE_RECONOCIDO, EST_PARENIZQUIERDO_RECONOCIDO,
EST_PARENDERECHO_RECONOCIDO, EST_PUNTOYCOMA_RECONOCIDO,EST_COMA_RECONOCIDO,EST_ASIGNACION_RECONOCIDO,
EST_SUMA_RECONOCIDO, EST_RESTA_RECONOCIDO, EST_MULTIPLICACION_RECONOCIDO, EST_DIVISION_RECONOCIDO,
EST_COMENTARIO_RECONOCIDO, EST_FDT_RECONOCIDO};

enum estadosError {EST_ERRORLEXICO_RECONOCIDO = 100, EST_ERRORASIG_RECONOCIDO, EST_ERRORCTE_RECONOCIDO};

enum {FALSE,TRUE};

#define ESFINAL(e) ((e) >= EST_ID_RECONOCIDO && (e) <= EST_COMENTARIO_RECONOCIDO)
#define ESINDEFINIDO(e) ((e) > EST_RECONOCIENDO_ERRORCTE)

char* yytext;
unsigned int nroLinea = 1, seguirLeyendo = TRUE;
struct ts_entry tabla_simb[1000];
token tok;
token tokenActual;

token scanner()
{
    static char buffer[100];
    char caracter;
    int largo = 0;
    unsigned int estado, proximoEstado;
    unsigned int tablaDeTransicion[7][16] = {
/*	    0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   |   10  |   11  |   12  |   13  |   14  |   15      */
/*	    L   |   D   |   +   |   -   |   *   |   /   |   (   |   )   |   ,   |   ;   |   :   |   =   |   FDT |   \n  |   SP  |   OTRO    */
	{   1   ,   2   ,   57  ,   58  ,   59  ,   4   ,   52  ,   53  ,   55  ,   54  ,   3   ,   100 ,   62  ,   0   ,   0   ,   100 }, // 0- EST_INICIAL
	{   1   ,   1   ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  ,   50  }, // 1  EST_RECONOCIENDO_ID
	{   6   ,   2   ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  ,   51  }, // 2  EST_RECONOCIENDO_CONSTANTE
	{   101 ,   101 ,   101 ,   101 ,   101 ,   101 ,   101 ,   101 ,   101 ,   101 ,   101 ,   56  ,   101 ,   101 ,   101 ,   101 }, // 3  EST_RECONOCIENDO_ASIGNACION
	{   60  ,   60  ,   60  ,   60  ,   60  ,   5   ,   60  ,   60  ,   60  ,   60  ,   60  ,   60  ,   60  ,   60  ,   60  ,   60  }, // 4  EST_RECONOCIENDO_DIVISION
	{   5   ,   5   ,   5   ,   5   ,   5   ,   5   ,   5   ,   5   ,   5   ,   5   ,   5   ,   5   ,   61   ,   61  ,   5   ,   5   }, // 5  EST_RECONOCIENDO_COMENTARIO
	{   6   ,   6   ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 ,   102 }  // 6  EST_RECONOCIENDO_ERRORCT
};  /** Los estados del 50+ al 62+ son estados finales (o aceptores), que no incluimos en la tabla porque freno el automata*/
    /** Los estados del 100+ al 102+ son estados de error, que no incluimos en la tabla porque freno el automata*/

    estado = EST_INICIAL;
    caracter = fgetc(stdin);
    do{
            proximoEstado = tablaDeTransicion[estado][obtenerColumna(caracter)];
            if(ESINDEFINIDO(proximoEstado))
                break;
            if(proximoEstado)
                buffer[largo++] = caracter;//Evita guardar los espacios enter y tab
                estado = proximoEstado;
                caracter = fgetc(stdin);
    }while(caracter != EOF);


    if(proximoEstado<=51 || proximoEstado==60){//Son los que usan centinela
        ungetc(caracter,stdin);
        buffer[largo] = '\0';
        yytext = buffer;
    }else{//Los que no usan centinelas se guardan en este momento
        buffer[largo++] = caracter;
        buffer[largo] = '\0';
        yytext = buffer;
    }

    //esto es para poder tener cual fue el error en yytext quizas no es necesario
    if(ESFINAL(proximoEstado)){
        tok = proximoEstado - (EST_ID_RECONOCIDO - ID);//46 es la constante que hay de diferencia entre el estado reconociendo y el token estado
                                 //si se agrega un nuevo estado basta con respetar el orden y la constante seguira siendo la misma
    }
    else{
        tok = proximoEstado - (EST_ERRORLEXICO_RECONOCIDO - ERRORLEXICO);//IDEM
    }
    return tok;
}

unsigned int obtenerColumna(char c){
    if(isalpha(c))return 0;
    if(isdigit(c))return 1;
    if(c == '+')return 2;
    if(c == '-')return 3;
    if(c == '*')return 4;
    if(c == '/')return 5;
    if(c == '(')return 6;
    if(c == ')')return 7;
    if(c == ',')return 8;
    if(c == ';')return 9;
    if(c == ':')return 10;
    if(c == '=')return 11;
    if(c == EOF)return 12;
    if(c == '\n'){//enter y FDT son los unicos que terminan el comentario
        nroLinea++;
        return 13;
    }
    if(isspace(c))return 14;
    return 15;
};

char* tok_names[] = {
    "INICIO",
    "FIN",
    "LEER",
    "ESCRIBIR",
    "ID",
    "CONSTANTE",
    "PARENIZQUIERDO",
    "PARENDERECHO",
    "PUNTOYCOMA",
    "COMA",
    "ASIGNACION",
    "SUMA",
    "RESTA",
    "MULTIPLICACION",
    "DIVISION",
    "COMENTARIO",
    "FDT",
    "ERRORLEXICO",
    "ERRORASIG",
    "ERRORCTE"
    };

token prox_token(void){
    if(seguirLeyendo){
        tokenActual = scanner();
        if(tokenActual==ID)
        buscar(yytext,tabla_simb,&tokenActual);
        if(tokenActual==COMENTARIO)
        tokenActual = scanner();
        seguirLeyendo = FALSE;
    }
    return tokenActual;
}

void match(token tok){
    if(tok!=prox_token()){
        error_sintactico(tok);
    }
    seguirLeyendo = TRUE;
}
