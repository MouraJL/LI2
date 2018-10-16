#include <stdio.h>

#include "ficheiro.h"
#include "ajuda.h"
#include "estado.h"
#include "auxiliares.h"

#define MAX_BUFFER		10240

int comparar_estados(ESTADO a, ESTADO b){
	if(a.num_cols != b.num_cols || a.num_lins != b.num_lins) return 0;

	int l,c;
	int check = 1;

	for(l = 0; l < a.num_lins; l++){
		for(c = 0; c < a.num_cols; c++){
			if(a.grelha[l][c] != b.grelha[l][c]){
				check = 0;
			}
			if(!check) break;
		}
		if(!check) break;
	}
	return check;
}

int check_preenchido(ESTADO e){
	for (int l = 0; l < e.num_lins; ++l)
	{
		for (int c = 0; c < e.num_cols; ++c)
		{
			if(e.grelha[l][c] == VAZIA) return -1;
		}
	}
	return -2;
}

VALOR contraria(VALOR v){
	switch(v){
		case FIXO_X: return SOL_O;
		case FIXO_O: return SOL_X;
		case SOL_O:  return SOL_X;
		case SOL_X:  return SOL_O;
		default   :  return v;
	}
}

VALOR bloqueada2sol(VALOR v){
	switch(v){
		case FIXO_X: return SOL_X;
		case FIXO_O: return SOL_O;
		default   :  return v;
	}
}

VALOR sol2bloqueada(VALOR v){
	switch(v){
		case SOL_X: return FIXO_X;
		case SOL_O: return FIXO_O;
		default   :  return v;
	}
}

int ajuda_assitente(ESTADO *e, int l, int c, int x, int y){
	VALOR p0 = bloqueada2sol(e->grelha[l][c]);
	VALOR p1 = bloqueada2sol(e->grelha[l+1*x][c+1*y]);
	VALOR p2 = bloqueada2sol(e->grelha[l+2*x][c+2*y]);
	if(p0 != BLOQUEADA && p0 != FIXO_O && p0 != FIXO_X && (VALOR) p0 != contraria(p1) && p1 != BLOQUEADA && p1 != VAZIA && p1 == p2){
		e->grelha[l][c] = contraria(p1);
		return 1;
	}

	if(p1 != BLOQUEADA && p1 != FIXO_O && p1 != FIXO_X && (VALOR) p1 != contraria(p0) && p0 != BLOQUEADA && p0 != VAZIA && p0 == p2){
		e->grelha[l+1*x][c+1*y] = contraria(p0);
		return 1;
	}

	if(p2 != BLOQUEADA && p2 != FIXO_O && p2 != FIXO_X && (VALOR) p2 != contraria(p0) && p0 != BLOQUEADA && p0 != VAZIA && p0 == p1){
		e->grelha[l+2*x][c+2*y] = contraria(p0);
		return 1;
	}
	return 0;
}


int ajuda_diagonal_esq(ESTADO *e){
	int l, c;
	for (l = 2; l < e->num_lins; ++l){
		for (c = e->num_cols -1; c >= 2; --c){
			if(ajuda_assitente(e,l,c,-1,-1)) return 1;
		}
	}
	return 0;
}

int ajuda_diagonal_dir(ESTADO *e){
	int l,c;
	for (l = 2; l < e->num_lins; ++l){
		for (c = 0; c < e->num_cols - 2; ++c){
			if(ajuda_assitente(e,l,c,-1,1)) return 1;
		}
	}
	return 0;
}

int ajuda_vertical(ESTADO *e){
 	for (int c = 0; c < e->num_cols; ++c){
		for (int l = 0; l < e->num_lins-2; ++l){
			if(ajuda_assitente(e,l,c,1,0)) return 1;
		}
	}
	return 0;
 }

int ajuda_horizontal(ESTADO *e){
	for (int l = 0; l < e->num_lins; ++l)
	{
		for (int c = 0; c < e->num_cols-2; ++c)
		{
			if(ajuda_assitente(e,l,c,0,1)) return 1;
		}
	}
	return 0;
}

ESTADO ajuda(ESTADO e){
	int r = ajuda_horizontal(&e);

	if (r == 0)
	{
		int r2 = ajuda_vertical(&e);

		if (r2 == 0)
		{
			int r3 = ajuda_diagonal_dir(&e);

			if (r3 == 0)
			{
				ajuda_diagonal_esq(&e);
			}
		}
	}
	return e;
}

ESTADO buscar_raw_current_level(){
	FILE *file;
	ESTADO e;
	char *filename = "/var/www/html/puzzles/current_level.txt";
	int c, count = 0;
    char str[MAX_BUFFER];

	file = fopen(filename, "r");

	c = fgetc(file);
	while(c != '\n' && c != EOF)
	{
		str[count++] = c;
		c = fgetc(file);
	}
	str[count] = '\0';
	e = str2estado(str);

	fclose(file);

	return e;
}

ESTADO solucao_puzzle(ESTADO raw, int ficheiro){
	int count = 0;
	if(ficheiro) raw = buscar_raw_current_level();
	ESTADO ant;
	ant.num_lins = 0;
	ant.num_cols = 0;
	while(check_preenchido(raw) == -1 && !comparar_estados(raw, ant)){
		ant = raw;
		raw = ajuda(raw);
		count++;
	}
	return raw;
}

ESTADO ajuda_better(ESTADO e){
	ESTADO raw = buscar_raw_current_level();
	ESTADO sol = solucao_puzzle(raw, 0);
	if(comparar_estados(sol,e)) return e;
	int check = 1;
	while(check){
		raw = ajuda(raw);
		check = 0;
		for(int l = 0;l<e.num_lins;l++){
			for(int c = 0;c < e.num_cols;c++){
				int atual = raw.grelha[l][c];
				if(atual == SOL_O || atual == SOL_X){
					if(e.grelha[l][c] != atual){
						e.grelha[l][c] = atual;
						return e;
					}else{
						raw.grelha[l][c] = sol2bloqueada(atual);
						check = 1;
					}
				}
			}
		}
	}

	return e;
}

int puzzle_valido(ESTADO e){
	int check;
	e = puzzle_preencher_validar(e);
	for (int l = 0; l < e.num_lins; ++l){
		for (int c = 0; c < e.num_cols; ++c){
			if(e.grelha[l][c] != BLOQUEADA) check = peca_valida(e,l,c,1);
			if(!check) return 0;
		}
	}

	return 1;
}

ESTADO puzzle_preencher_validar(ESTADO e){
	int atual;
	for (int l = 0; l < e.num_lins; ++l)
	{
		for (int c = 0; c < e.num_cols; ++c)
		{
			atual = e.grelha[l][c];
			if(atual == FIXO_X || atual == FIXO_O) e.grelha[l][c] = bloqueada2sol(atual);
			if (atual == VAZIA) e.grelha[l][c] = BLOQUEADA;
		}
	}
	return e;
}

int peca_valida(ESTADO e, int l, int c, int revert){
	int atual;
	if(revert){
		for (int le = 0; le < e.num_lins; ++le){
			for (int ce = 0; ce < e.num_cols; ++ce){
				atual = e.grelha[le][ce];
				if(atual == FIXO_X || atual == FIXO_O) e.grelha[le][ce] = bloqueada2sol(atual);
			}
		}
	}
	atual = e.grelha[l][c];

	if (c > 0 && c < e.num_cols - 1 && atual == e.grelha[l][c-1] && atual == e.grelha[l][c+1]) return 0;
	if (l > 0 && l < e.num_lins - 1 && atual == e.grelha[l-1][c] && atual == e.grelha[l+1][c]) return 0;
	if (c > 0 && c < e.num_cols - 1 && l > 0 && l < e.num_lins - 1){
		if (atual == e.grelha[l-1][c-1] && atual == e.grelha[l+1][c+1]) return 0;
		if (atual == e.grelha[l+1][c-1] && atual == e.grelha[l-1][c+1]) return 0;
	}

	if(c < e.num_cols-2 && atual == e.grelha[l][c+1] && atual == e.grelha[l][c+2]) return 0;
	if(c > 1 && atual == e.grelha[l][c-1] && atual == e.grelha[l][c-2]) return 0;
	if(l < e.num_lins-2 && atual == e.grelha[l+1][c] && atual == e.grelha[l+2][c]) return 0;
	if(l > 1 && atual == e.grelha[l-1][c] && atual == e.grelha[l-2][c]) return 0;

	if(c < e.num_cols-2 && l > 1 && atual == e.grelha[l-1][c+1] && atual == e.grelha[l-2][c+2]) return 0;
	if(c > 1 && l < e.num_lins-2 && atual == e.grelha[l+1][c-1] && atual == e.grelha[l+2][c-2]) return 0;
	if(c > 1 && l > 1 && atual == e.grelha[l-1][c-1] && atual == e.grelha[l-2][c-2]) return 0;
	if(c < e.num_cols-2 && l < e.num_lins-2 && atual == e.grelha[l+1][c+1] && atual == e.grelha[l+2][c+2]) return 0;


	return 1;
}