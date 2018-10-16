#ifndef ___AUXILIARES_H___
#define ___AUXILIARES_H___

/**
@file auxiliares.h
Definição de funções que auxiliam as funções pricipais, normalmente reset de ESTADO ou return de peças
*/

#include "estado.h"

#define MAX_BUFFER		10240

/**
\brief Estrutura que armazena as informações de cada utilizador
*/
typedef struct user_id{
	char user[11];
	int level;
	char query[805];
	int hint_num;
}*ID;

/**
\brief Função que transforma um carater numa peça
@param s O carater guardado num ficheiro
@returns A peça (VALOR) correspondente ao carater
*/
VALOR txt_peca(char s);

/**
\brief Função que transforma uma peça num carater
@param p A peça do jogo
@returns O carater correpondente à peça do jogo
*/
char peca_text(VALOR p);

/**
\brief Função que altera um estado e dá o valor zero a todos os seus componentes
@returns O estado com todos os seus componentes a zero
*/
ESTADO resetEstado();

/**
\brief Atualiza o valor para o próximo
@VAZIA -> SOL_X ; SOL_X -> SOL_O ; SOL_O -> VAZIA 
@param v A peça atual
@returns A peeça atualizada
*/
VALOR act_estado(VALOR v);

/**
\brief Função que associa a cada imagem um link para o próximo estado
@modifica a imagem ao clickar (com a ajuda da função atualiza)
@param l A linha onde se encontra a peça a ser alterada
@param c A coluna onde se encontra a peça a ser alterada
@param e O estado atual 
*/
void criaLink (int l, int c, ESTADO e);

int char_number_2_int(char str[]);

/**
\brief Função que verifica se um puzzle está completo
@param e O estado atual 
@return 1 se o puzzle estiver completo e 0 cado contrário
*/
int estado_completo(ESTADO e);

#endif