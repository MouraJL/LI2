#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "../ficheiro.h"
#include "../estado.h"
#include "../auxiliares.h"
#include "../ajuda.h"
#include "../ancoras.h"
#include "../resolver.h"

void printEstado(ESTADO e){
	int l,c;
	for (l = 0; l < e.num_lins; ++l)
	{
		for (c = 0; c < e.num_cols; ++c)
		{
			VALOR p = e.grelha[l][c];
			char c =  peca_text(p);
			printf("%c", c);
		}
		if(l == e.num_lins - 1) return;
		printf("\n");
	}
}

ESTADO estado_vazio(int l, int c){
	ESTADO e;
	e.num_lins = l;
	e.num_cols = c;
	for (int l2 = 0; l2 < e.num_lins; ++l2)
	{
		for (int c2 = 0; c2 < e.num_cols; ++c2)
		{
			e.grelha[l2][c2] = VAZIA;
		}
	}
	return e;
}


ESTADO gerar_puzzle_completo(int le, int ce){
	ESTADO e;
	int count_bloqueadas = 0;
	int max = le+ce;
	e.num_lins = le;
	e.num_cols = ce;
	srand(time(NULL));
	for (int l = 0; l < e.num_lins; ++l)
	{
		for (int c = 0; c < e.num_cols; ++c)
		{
			int r = 1+(rand()%2);
			//printf("%d\n",r);
			switch(r){
				case 1: 
					e.grelha[l][c] = FIXO_X;
					break;
				case 2: 
					e.grelha[l][c] = FIXO_O;
					break;
			}
			if (peca_valida(e,l,c,1) == 0)
			{
				e.grelha[l][c] = contraria(e.grelha[l][c]);
				if(peca_valida(e,l,c,1) == 0){
					e.grelha[l][c] = BLOQUEADA;
					count_bloqueadas++;
					if(max<count_bloqueadas){
						e = resetEstado();
						e.num_lins = le;
						e.num_cols = ce;
						l = -1;
						count_bloqueadas = 0;
						break;
					}
				} 
			}
		}
	}
	return e;
}

ESTADO apagar_pecas(ESTADO e, int nivel){
	int l,c;
	int lMax = e.num_lins;
	int cMax = e.num_cols;
	int num_sol = 0;
	int num_vazios = 0;
	srand(time(NULL));
	int max = lMax * cMax, ant;
	int count = max;
	int finish = (max * 3 / 5);

	ESTADO e2;

	while(count > 0 || num_vazios < finish){
		l = (rand()%lMax);
		c = (rand()%cMax);
		if(e.grelha[l][c] != (BLOQUEADA && VAZIA)){
			ant = e.grelha[l][c];
			e.grelha[l][c] = VAZIA;
			count--;
			if(nivel){
				resolver_GandaGalo(e,&num_sol, 0);
				if(count < 0 && num_vazios >= finish - 1){
					if(num_sol != 1) e.grelha[l][c] = ant;
					num_sol = 0;
					e2 = e;
					e2 = solucao_puzzle(e2, 0);
					if(estado_completo(e2) == 0) return e;
					else{
						for(l = 0;l< e.num_lins;l++){
							for(c = 0;c< e.num_cols;c++){
								ant = e.grelha[l][c];
								e.grelha[l][c] = VAZIA;
								resolver_GandaGalo(e,&num_sol, 0);
								if(num_sol != 1) e.grelha[l][c] = ant;
								num_sol = 0;
							}
						}
						return e;
					}
				}
			}
			else{
				e2 = e;
				e2 = solucao_puzzle(e2, 0);
				if(estado_completo(e2)) num_sol = 1;
				else num_sol = 0;
			}
			if(num_sol != 1) e.grelha[l][c] = ant;
			else num_vazios++;
			num_sol = 0;
		}
	}
	return e;
}


//int main(int argc, char **argv)
int main(int argc, char **argv)
{
	if(argc != 4){
		printf("Numero de argumentos errados\nExemplo:	\"./gerar (Nivel) (Linhas) (Colunas)\"\n");
		return 0;
	}
	int check = 1;
	ESTADO e;
	int nivel = char_number_2_int(*argv++);
	nivel = char_number_2_int(*argv++) - 1;
 	int l = char_number_2_int(*argv++);
 	int c = char_number_2_int(*argv++);
 	if(nivel < 0 || nivel > 1 || l < 3 || l > 20 || c < 3 || c > 20){
 		printf("Argumentos invalidos\nNivel:	  1/2\nLinhas:	  3-20\nColunas:  3-20\n");
 		return 0;
 	}
    printf("%d %d\n", l, c);
    int count, count2, l1, c1;
	while(check){
    	e = gerar_puzzle_completo(l,c);
    	e = apagar_pecas(e, nivel);
    	if(nivel == 0) break;
    	ESTADO e2 = e;
        e2 = solucao_puzzle(e2, 0);
        check = estado_completo(e2);
        if(check == 0 && (e.num_lins >= 5 && e.num_cols >= 5)){
        	count = 0;
        	count2 = 0;
        	for(l1 = 0;l1 < e2.num_lins;l1++){
        		for(c1 = 0;c1 < e2.num_cols;c1++){
        			if(e2.grelha[l1][c1] ==  VAZIA) count2++;
        			if(e.grelha[l1][c1] == VAZIA) count++;
        		}
        	}
        	if(count2*6 < count){
        		check = 1;
        	} 
        }
    }
	printEstado(e);
	printf("\n");
	
    return 0;
}