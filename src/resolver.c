#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ficheiro.h"
#include "ancoras.h"
#include "ajuda.h"
#include "estado.h"
#include "auxiliares.h"
#include "resolver.h"

int finish_resolucao(ANCORA a, int *count, ESTADO *sol){
	ESTADO e = a->e;
	ESTADO f = e;
	int l = 0,c = 0, check = 0;

	for(l = 0; l < f.num_lins; l++){
		for(c = 0; c < f.num_cols; c++){
			if(f.grelha[l][c] == VAZIA){
				f.grelha[l][c] = BLOQUEADA;
				check = 1;
			}
		}
	}

	if(check == 0){
		return 1;
	}

	l = c = check = 0;
	while(l < e.num_lins){
		c = 0;
		while(c < e.num_cols){
			if(e.grelha[l][c] == VAZIA) check = 1;
			if(check) break;
			c++;
		}
		if(check) break;
		l++;
	}

	ANCORA b = malloc(sizeof(struct ancoras));
	b->prox = NULL;
	a->prox = b;

	e.grelha[l][c] = SOL_X;
	b->e = e;
	if(peca_valida(e,l,c,1)){
		if(estado_completo(e)){
			*sol = e;
			*count = *count +1;
		}else{
			if(finish_resolucao(b, count, sol)) return 1;
			else{
				e.grelha[l][c] = SOL_O;
				b->e = e;
			}
		}
	}
	else{
		e.grelha[l][c] = SOL_O;
		b->e = e;
	}
	
	
	if(peca_valida(e,l,c,1)){
		if(estado_completo(e)){
			*sol = e;
			*count = *count +1;
			return 0;
		}else{
			if(finish_resolucao(b,count, sol)) return 1;
			else{
				free(b);
				a->prox = NULL;
				return 0;
			}
		}
	}
	else{
		free(b);
		a->prox = NULL;
		return 0;
	}
	return 0;
}

ESTADO resolver_GandaGalo(ESTADO e, int* count, int ficheiro){
	if(ficheiro) e = solucao_puzzle(e,ficheiro);
	if (estado_completo(e) == 1) return e;
	ANCORA a = malloc(sizeof(struct ancoras));
	ESTADO sol;
	a->prox = NULL;
	a->e = e;
	finish_resolucao(a, count, &sol);
	while(a->prox != NULL){
		ANCORA prox = a->prox;
		free(a);
		a= NULL;
		a = prox;
	} 
	
	free(a);
	return sol;
}