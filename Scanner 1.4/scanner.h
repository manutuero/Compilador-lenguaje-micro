#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

typedef enum { INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
		PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA,
		MULTIPLICACION, DIVISION, COMENTARIO, FDT, ERRORLEXICO,
		ERRORASIG, ERRORCTE
}token;
/** definimos un nuevo tipo de dato "token",
que representa a un numero entero entre 0 y 19 */

extern char* yytext;
extern char* tok_names[];
extern token tokenActual;
extern unsigned int nroLinea;

void match(token tok);
token prox_token(void);
token scanner();
token obtenerToken(unsigned int estado);
token obtenerError(unsigned int estado);
unsigned int obtenerColumna(char);

#endif // SCANNER_H_INCLUDED
