#include "parser.h"
#include "semantic.h"

void objetivo(void){
    programa();
    match(FDT);
    terminar();
}

void programa(void){
    //comenzar();
    match(INICIO);
    lista_sentencias();
    match(FIN);
    terminar();
}

void lista_sentencias(void){
    sentencia();
    while (1) {
        switch (prox_token()) {
            case ID:
            case LEER:
            case ESCRIBIR:
                 sentencia();
                 break;
            default:
                 return;
        }
    }
}

void sentencia(void){
        struct reg_expr ident;
        struct reg_expr expr;
        switch (prox_token()) {
            case ID:
                identificador(&ident);
                match(ASIGNACION);
                expresion(&expr);
                match(PUNTOYCOMA);
                asignar(&ident,&expr);
                break;
            case LEER:
                match(LEER);
                match(PARENIZQUIERDO);
                lista_identificadores();
                match(PARENDERECHO);
                match(PUNTOYCOMA);
                break;
            case ESCRIBIR:
                match(ESCRIBIR);
                match(PARENIZQUIERDO);
                lista_expresiones();
                match(PARENDERECHO);
                match(PUNTOYCOMA);
                break;
            default:
                error_sintactico(prox_token());
                break;
        }
}

void lista_identificadores(void){
    struct reg_expr id;
    identificador(&id);
    leer_id(&id);
    while(prox_token()==COMA){
        match(COMA);
        identificador(&id);
        leer_id(&id);
    }
}

void identificador(struct reg_expr *preg){
    match(ID);
    *preg = procesar_id();
}

void lista_expresiones(void){
    struct reg_expr expr;
    expresion(&expr);
    escribir_exp(&expr);
    while(prox_token()==COMA){
        match(COMA);
        expresion(&expr);
        escribir_exp(&expr);
    }
}

void expresion(struct reg_expr *preg){
    struct reg_expr t1;
    struct reg_expr t2;
    struct reg_op op;
    termino(&t1);
    token t = prox_token();
    while(t == SUMA || t == RESTA){
        operador_aditivo(&op);
        termino(&t2);
        t1 = gen_infijo(&t1,&op,&t2);
        t = prox_token();
    }
    *preg = t1;
}
void termino(struct reg_expr *preg){
    struct reg_expr p1;
    struct reg_expr p2;
    struct reg_op op;
    primaria(&p1);
    token t = prox_token();
    while(t == MULTIPLICACION || t == DIVISION){
        operador_multiplicativo(&op);
        primaria(&p2);
        p1 = gen_infijo(&p1,&op,&p2);
        t = prox_token();
    }
    *preg = p1;
}

void primaria(struct reg_expr *preg){
    struct reg_expr pri;
    switch(prox_token()){
        case ID:
            identificador(&pri);
            break;
        case CONSTANTE:
            match(CONSTANTE);
            pri = procesar_cte();
            break;
        case PARENIZQUIERDO:
            match(PARENIZQUIERDO);
            expresion(&pri);
            match(PARENDERECHO);
            break;
        default:
            error_sintactico(prox_token());
            break;
    }
    *preg = pri;
}

void operador_aditivo(struct reg_op *preg){
    token t = prox_token();
    if(t == SUMA || t == RESTA){
        match(t);
        *preg = procesar_op();
    }
    else
        error_sintactico(prox_token());
}

void operador_multiplicativo(struct reg_op *preg){
    token t = prox_token();
    if(t == MULTIPLICACION || t == DIVISION){
        match(t);
        *preg = procesar_op();
    }
    else
        error_sintactico(prox_token());
}
