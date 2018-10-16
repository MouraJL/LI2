#ifndef ___AJUDA_H___
#define ___AJUDA_H___

/**
@file ajuda.h
Definição de funções que resolvem/dão hints ao puzzle e de funções que verificam se o puzzle è válido
*/

#include "estado.h"

/**
\brief Função que compara a grelha de dois estados
@param 2 estados
@returns O int 1 representa igualdade e o int 0 representa diferença
*/
int comparar_estados(ESTADO a, ESTADO b);

/**
\brief Função que verifica se o estado tem a grelha totalmente preenchida, sem VAZIO
@param O Estado
@returns Se estiver preenchida return 1, senão return 0;
*/
int check_preenchido(ESTADO e);

/**
\brief Função returna o VALOR contrario e sem ser FIXO. ex.: FIXO_O -> SOL_X || SOL_X -> SOL_O
@param um VALOR
@returns um VALOR
*/
VALOR contraria(VALOR v);

/**
\brief Função trasnforma VALOR FIXO_O e FIXO_X em SOL_O e SOL_X, resptivamente
@param v Um VALOR
@returns um VALOR
*/
VALOR bloqueada2sol(VALOR v);

/**
\brief Função transforma VALOR SOL_O e SOL_X em FIXO_O e FIXO_X, resptivamente
@param v Um VALOR
@returns um VALOR
*/
VALOR sol2bloqueada(VALOR v);

/**
\brief Função tipo auxiliar das ajudas, procura padroes
@param *e endereço para o ESTADO
@param l um int
@param c um int
@param x um int
@param y um int
@returns um int
*/
int ajuda_assitente(ESTADO *e, int l, int c, int x, int y);

/**
\brief Função que procura padroes na diagonal e caso encontre duas peças iguais seguidas ou intercaladas preenche 
o espaço vazio antes depos ou entre as peças com a peça contrária
@param e O Estado
@returns Se alterar o estado return 1, senão return 0;
*/
int ajuda_diagonal_esq(ESTADO *e);

/**
\brief Função que procura padroes na diagonal e caso encontre duas peças iguais seguidas ou intercaladas preenche 
o espaço vazio antes depos ou entre as peças com a peça contrária
@param e O Estado
@returns Se alterar o estado return 1, senão return 0;
*/
int ajuda_diagonal_dir(ESTADO *e);

/**
\brief Função que procura padroes na vertical e caso encontre duas peças iguais seguidas ou intercaladas preenche 
o espaço vazio antes depos ou entre as peças com a peça contrária
@param e O Estado
@returns Se alterar o estado return 1, senão return 0;
*/
int ajuda_vertical(ESTADO *e);

/**
\brief Função que procura padroes na horizontal e caso encontre duas peças iguais seguidas ou intercaladas preenche 
o espaço vazio antes depos ou entre as peças com a peça contrária
@param e O Estado
@returns Se alterar o estado return 1, senão return 0;
*/
int ajuda_horizontal(ESTADO *e);

/**
\brief Função que hierarquiza as funções de ajudas.
@param e O Estado
@returns O estado alterado depois de dada a ajuda.
*/
ESTADO ajuda(ESTADO e);

/**
\brief Função que lê a partir de um ficheiro o estado inicial do puzzle
@returns O estado inicial
*/
ESTADO buscar_raw_current_level();

/**
\brief Função que utiliza as funções de ajudas anteriores para chegar à solução do puzzle
@returns O estado do puzzle resolvido
*/
ESTADO solucao_puzzle(ESTADO raw, int ficheiro);

/**
\brief Função que hierarquiza as funções de ajudas(better).
@param e O Estado
@returns O estado alterado depois de dada a ajuda.
*/
ESTADO ajuda_better(ESTADO e);

/**
\brief Função que procura três peças iguais seguidas verticalmente para testar se o puzzle è válido
@param e O Estado
@returns Se for válido return 1, senão return 0;
*/
int valido_vertical(ESTADO e);

/**
\brief Função que procura três peças iguais seguidas horizontalmente para testar se o puzzle è válido
@param e O Estado
@returns Se for válido return 1, senão return 0;
*/
int valido_horizontal(ESTADO e);

/**
\brief Função que procura três peças iguais seguidas diagonalmente para testar se o puzzle è válido
@param e O Estado
@returns Se for válido return 1, senão return 0;
*/
int valido_diagonal_esq(ESTADO e);

/**
\brief Função que procura três peças iguais seguidas diagonalmente para testar se o puzzle è válido
@param e O Estado
@returns Se for válido return 1, senão return 0;
*/
int valido_diagonal_dir(ESTADO e);

/**
\brief Função que procura se ambas as funções que testam a validade do puzzle na diagonal devolvlem verdadeiro e em caso
afirmativo devolve també verdadeiro
@param e O Estado
@returns Se for válido return 1, senão return 0;
*/
int valido_diagonal(ESTADO e);

/**
\brief Função que chama todas as funções que testam a validade do puzzle e com base nos resultados destas decide se o puzzle è ou
não vàlido
@param e O Estado
@returns Se for válido return 1, senão return 0;
*/
int puzzle_valido(ESTADO e);

/**
\brief Função que verifica se a determinada peça é valida no local
@param e O Estado
@param l um int
@param c um int
@param revert um int (caso diferente de 0 transforma todos os FIXOS em SOL)
@returns Se for válido return 1, senão return 0;
*/
int peca_valida(ESTADO e, int l, int c, int revert);

/**
\brief Função que transforma todos os FIXOS nos respetivos SOL, e os VAZIA em BLOQUEADOS
@param e O Estado
@returns O estado alterado
*/
ESTADO puzzle_preencher_validar(ESTADO e);

#endif