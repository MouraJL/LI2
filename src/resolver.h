#ifndef ___RESOLVER_H___
#define ___RESOLVER_H___

ESTADO colocar_pecaX(ESTADO e, int l, int c);

ESTADO colocar_pecaO(ESTADO e, int l, int c);

ESTADO apagar_pecaO(ESTADO e, int l, int c);

ESTADO casa_valida(ESTADO e, int l, int c, int peca);

int finish_resolucao(ANCORA a, int *count, ESTADO *sol);

ESTADO resolver_GandaGalo(ESTADO e, int* count, int ficheiro);

#endif