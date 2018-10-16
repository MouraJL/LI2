#ifndef ___FICHEIRO_H___
#define ___FICHEIRO_H___

/**
@file ficheiro.h
Definição de funções que lêm/modifica ficheiros txt
*/

#include "estado.h"
#include "auxiliares.h"

/**
\brief Função que lê um ficcheiro com informação relativa a um puzzle (ESTADO)
@param numero O numero do puzzle cujo ficheiro a função vai ler
@returns O estado correspondente à informação guardada no ficheiro
*/
ESTADO ler_ficheiro(int numero);

/**
\brief Função verifica se o ficheiro está vazio
@param numero O numero do puzzle cujo ficheiro a função vai tentar ler
@returns Se estiver vazio return 0, senão return 1;
*/
int check_file(int i);

/**
\brief Função que verifica se o ficheiro history tem mais de um movimento (sendo que o primeiro é a o puzzle no seu estado inicial)
@returns Se houver ficheiro return 1, senão return 0;
*/
int verificar_ficheiro();

/**
\brief Função que verifica se a peça que esta a ser modificada é a mesma que a anterior, se não for guarda no ficheiro a peca atual modificada
@param *query uma str que tem a query
@returns Se peca for a mesma 1, senão return 0
*/
int peca_modificada(char * query);

void guardar_string_avancado(char * query, int ancora);

void apagar_history();

ESTADO buscar_ultimo_movimento(int check);

ESTADO procurar_ultima_ancora();

void guardar_current_level(char *query);

void username_file(char *str, ID current_user);

void salvar_current_user(char user[]);

void escrever_username_file(char current_user[], char escrever[], int onde);

void get_id_username(ID current_user);

void level_atual (char str[]);

#endif