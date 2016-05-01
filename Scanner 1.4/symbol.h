#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED

#define TAMLEX 32+1
#include "scanner.h"


struct ts_entry{
	char lexema[TAMLEX];
	token tok; /* tok=0, 1, 2, 3 Palabra Reservada, tok=ID=4 Identificador */
};

extern struct ts_entry tabla_simb[1000];

int buscar(char *id, struct ts_entry *ts, token *tok);
void colocar(char *id, struct ts_entry *ts);
void chequear(char *id);

#endif // SYMBOL_H_INCLUDED
