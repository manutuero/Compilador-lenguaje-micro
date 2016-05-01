#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "symbol.h"
#include "semantic.h"

struct ts_entry tabla_simb[1000]={
    {"inicio",0},{"fin",1},{"leer",2},{"escribir",3}
};

int ultimo = 4;
token tokenActual;

int buscar(char *id, struct ts_entry *ts, token *tok){
    int i;
    for(i=0;i<ultimo;i++){
        if(!strcmp(ts[i].lexema,id)){
            *tok = ts[i].tok;
            break;
        }
    }
    return i<ultimo;
}

void colocar(char *id, struct ts_entry *ts){
    struct ts_entry r;
    strcpy(r.lexema,id);
    r.tok = ID;
    ts[ultimo] = r;
    ultimo++;
}

void chequear(char *id){
    if(!buscar(id,tabla_simb,&tokenActual)){
        colocar(id,tabla_simb);
        generar("Declara ",id," Entera","");
    }
}

