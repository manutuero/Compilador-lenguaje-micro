#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "symbol.h"

void generar(char *op, char *a, char *b, char *c){
    printf("%s %s,%s,%s \n",op,a,b,c);
}

void comenzar(void);  /* inicializaciones semánticas */

void terminar(void){
      /* genera la instrucción para terminar la ejecución del programa */
     generar("Detiene", "", "", "");
}

void asignar(struct reg_expr *vizq, struct reg_expr *vder){
     /* genera la instrucción para la asignación */
     generar("Almacena", extraer(vder), vizq->nombre, "");
}

char *extraer(struct reg_expr *preg){
    char *r = preg->nombre;
    return r;
}

void leer_id(struct reg_expr *pin){
     /* genera la instrucción para leer */
    generar("Leer", pin->nombre, "Entera", "");
}

void escribir_exp(struct reg_expr *pout){
    generar("Escribir", extraer(pout), "Entera", "");
}

struct reg_expr procesar_cte(void){
     // convierte cadena que representa número a número entero y construye un registro semántico
      struct reg_expr t;
      t.clase = CONSTANTE;
      strcpy(t.nombre,yytext);
      sscanf(yytext, "%d", &t.valor);
      return t;
}

struct reg_expr procesar_id(void){
     // Declara ID y construye el correspondiente registro semántico
      struct reg_expr t;
      chequear(yytext);
      t.clase = ID;
      strcpy(t.nombre, yytext);
      return t;
}

struct reg_op procesar_op(void){
    struct reg_op t;
    t.cod_oper = tokenActual;
    return t;
};

struct reg_expr gen_infijo(struct reg_expr *pei, struct reg_op *op, struct reg_expr *ped){
/* Genera la instrucción para una operación infija y construye un     registro semántico con el resultado */
    struct reg_expr r;
    static int i = 1;
    char c[8];
    sprintf(c,"Temp&%d",i);
    i++;
    chequear(c);
    strcpy(r.nombre,c);
    switch(op->cod_oper){
        case SUMA:
            r.valor = pei->valor + ped->valor;
            break;
        case RESTA:
            r.valor = pei->valor - ped->valor;
            break;
        case MULTIPLICACION:
            r.valor = pei->valor * ped->valor;
            break;
        case DIVISION:
            r.valor = pei->valor / ped->valor;
            break;
    }
    generar(tok_names[op->cod_oper],pei->nombre,ped->nombre,r.nombre);
    return r;
};
