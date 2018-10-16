#ifndef ___ANCORAS_H___
#define ___ANCORAS_H___

/**
@file ficheiro.h
Definição de funções que lêm/modifica ancoras e os ficheiros de texto nas quais estas estão guardadas
*/

/**
\brief Estrutura que armazena as ancoras de um jogo
*/
typedef struct ancoras{
	ESTADO e;
	struct ancoras *prox;
}*ANCORA;

/**
\brief Função que liberta o espaço de memoria ocupado pelas ancoras do jogo
@param anc O endereço correspondente ao primeiro espaço de memória ocupado pela lista ligada
@returns O endereço de memoria correspondente ao primeiro espaço de memória ocupado pela lista ligada
*/
void anc_free(ANCORA anc);

/**
\brief Função que guarda um estado num ficheiro reservado para as âncoras
@param querry Uma string contendo os argumentos passados à CGI
*/
void guardar_ancora(char *querry);

/**
\brief Função que lẽ as âncoras do jogo a partir de um ficheiro e as coloca numa lista ligada
@param e O estado atual do jogo
@param nAnc O indice da ancora qu se pretende ler
@returns O estado guardado na ancora de indice nAnc
*/
ESTADO ler_ancora(ESTADO e, int nAnc);

/**
\brief Função que conta quantas ancoras foram guardadas
@returns O enumero de ancoras guardadas pelo utilizador
*/
int num_anc();

/**
\brief Função que apaga todas as âncoras guardads em ficheiros
*/
void apagar_ancoras();

#endif