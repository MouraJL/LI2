#include <string.h>

#include "auxiliares.h"
#include "estado.h"
#include "cgi.h"


VALOR txt_peca(char s){
	switch(s)
	{
		case 'X':		return FIXO_X;
		case 'O': 	    return FIXO_O;
		case '#':       return BLOQUEADA;
		case '.':       return VAZIA;
		default:  		return VAZIA;
	}
}

char peca_text(VALOR p){
	switch(p){
		case SOL_X:     return 'X';
		case SOL_O:     return 'O';
		case FIXO_X:     return 'X';
		case FIXO_O:     return 'O';
		case BLOQUEADA: return '#';
		case VAZIA:     return '.';
		default:        return '.';
	}
}


ESTADO resetEstado(){
	ESTADO e_reset;
	e_reset.num_lins = 0;
	e_reset.num_cols = 0;
	int l, c;
	for (l = 0; l < MAX_GRID; ++l)
	{
		for (c = 0; c < MAX_GRID; ++c)
		{
			e_reset.grelha[l][c] = 0;
		}
	}
	return e_reset;
}

VALOR act_estado(VALOR v){
	switch(v)
	{
		case VAZIA:		return SOL_X;
		case SOL_X: 	return SOL_O;
		case SOL_O: 	return VAZIA;
		default:  		return VAZIA;
	}
}

void criaLink (int l, int c, ESTADO e){
	ESTADO e2 = e;
	e2.grelha[l][c] = act_estado((VALOR) e2.grelha[l][c]);
	char *s = estado2str (e2);
	char buffer[MAX_BUFFER]; // array de caracteres de tamanho fixo
	sprintf(buffer,"GandaGalo?%s", s);
	ABRIR_LINK (buffer);
}

int char_number_2_int(char str[]){
	int count = 0;

	while(str[count] <= '9' && str[count] >= '0'){
		count++;
	}

	int final = 0;
 	int i, multiplicar = 1, a;

 	for (i = 0; str[i] <= '9' && str[i] >= '0'; ++i, count--)
 	{
 		multiplicar = 1;
 		a = count - 1;
 		while(a != 0){
 			multiplicar *= 10;
 			a--;
 		}

 		final += ((int) (str[i] - 48) * multiplicar);
 	}

 	return final;
}

int estado_completo(ESTADO e){
	for (int l = 0; l < e.num_lins; ++l)
	{
		for (int c = 0; c < e.num_cols; ++c)
		{
			if (e.grelha[l][c] == VAZIA) return 0;
		}
	}
	return 1;
}