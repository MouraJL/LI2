/**
@file exemplo.c
Esqueleto do programa
*/

#include <stdlib.h>
#include <string.h>

#include "cgi.h"
#include "estado.h"
#include "ajuda.h"
#include "ficheiro.h"
#include "auxiliares.h"
#include "ancoras.h"
#include "resolver.h"

#define GRELHA			4
#define TAM			40

const char *ficheiros[] = {"bloq.png","X.png","O.png","vazio.png","X2.png","O2.png"};

/**
Função que inicializa o username
*/

void username(int check) {
	COMECAR_HTML;
	printf("<title>GandaGalo - O Jogo</title>\n");

	printf("<head>\n");
	printf("<style>\n");
	printf("h1{background-color: blue; color: white;font-family: Comic Sans MS}\n");
	printf("h2{background-color: blue; color: red;font-family: Comic Sans MS}\n");
	printf("</style>\n");
	printf("</head>\n");

	printf("<body>\n");
	printf("<center>\n");
	printf("<h1>GandaGalo</h1>\n");


	printf("<form name=\"login\">\nUsername: <input type=\"text\" name=\"user\"/>\n");
	printf("</form>\n");
	if(check){
		printf("<h1>%s</h1>\n", "Enjoy the Game");
	}else{
		printf("<h2>%s</h2>\n", "Username - only letters, max 10 charecters min 3 charecters");
	}
	printf("</center>\n");
	printf("</body>\n");
	FECHAR_HTML;
}

/**
Função que inicializa o menu
*/
void menu(ID current_user, int validar) {
	FILE * levels;
	
	char file[50];
	sprintf(file, "/var/www/html/puzzles/levels.txt"); //funciona !!!!

	levels = fopen(file, "w");

	COMECAR_HTML;
	printf("<title>GandaGalo - User: %s</title>\n", current_user->user);

	printf("<head>\n");
	if(validar == 1){
		printf("<meta http-equiv=\"refresh\" content=\"5;url=http://localhost/cgi-bin/GandaGalo?6\" />");
	}
	printf("<style>\n");
	if(validar == 1){
		printf("h1{background-color: green; color: white;font-family: Comic Sans MS}\n");
	}else{
		printf("h1{background-color: blue; color: white;font-family: Comic Sans MS}\n");
	}
	printf("h2{background-color: white; color: blue;font-family: Comic Sans MS}\n");

	printf("</style>\n");
	printf("</head>\n"); 

	printf("<body>\n");
	printf("<center>\n");
	printf("<h1>GandaGalo</h1>\n");

	for (int i = 1; check_file(i) && i <= current_user->level; ++i)
	{
		printf("<p>\n<a href=\'");
		
		char s[808];
		strcpy(s, estado2str(ler_ficheiro(i)));
		char buffer[MAX_BUFFER];
		sprintf(buffer,"GandaGalo?%s", s);

		if(current_user->level == i && current_user->query[0] != '\0'){
			printf("http://localhost/cgi-bin/GandaGalo?%s\'", current_user->query);
		} else{
			printf("http://localhost/cgi-bin/%s\'", buffer);
		}

		//escreve no ficheiro a string do nivel, na respetiva linha
		fprintf(levels, "%s\n", s);

		printf(">LEVEL %d\n", i);
		FECHAR_LINK;
		printf("</p>\n");
	}

	if(validar == 1){
		printf("<h1>Very Good !</h1>\n");
	} else{
		printf("<h1>Menu</h1>\n");
	}
	printf("<h2  style = \"font-size:15px\">\n<a href=\'http://localhost/cgi-bin/GandaGalo?9\'");
	printf(">Instructions\n</h2>\n");
	printf("<h2  style = \"font-size:15px\">\n<a href=\'http://localhost/cgi-bin/GandaGalo?\'");
	printf(">Logout\n</h2>\n");
	printf("</center>\n");
	printf("</body>\n");
	FECHAR_HTML;
	fclose(levels);
}

void intrucoes(){
	COMECAR_HTML;
	printf("<title>GandaGalo - Instructions \n");
	printf("</title>\n");
	printf("<head>\n");
	printf("<style>\n");
	printf("h2{background-color: white; color: blue;font-family: Comic Sans MS}\n");
	printf("</style>\n");
	printf("</head>\n");

	printf("<body>\n");

	printf("<center>\n");
	
	printf("<img src=\"%sinstrucoes.png\">\n",IMAGE_PATH);

	printf("<h2  style = \"font-size:15px\">\n<a href=\'http://localhost/cgi-bin/GandaGalo?6\'");
	printf("<h2>Menu</h2>\n");
	printf("</center>\n");
	printf("</body>\n");
	FECHAR_HTML;
}

/**
Função que desenha o puzzle e os botões associados às ações que se podem tomar
*/
//void puzzle (ESTADO e, int validar, int refresh, int hint_num){
void puzzle (ESTADO e, int validar, int hint_num){
	int tamanho_imagens, tamanho_novo;
	if(e.num_cols > 12){
		tamanho_imagens = 60 - e.num_cols - 2;
		tamanho_novo = 80 - e.num_cols;
	}else{
		tamanho_imagens = 60;
		tamanho_novo = 80;
	}

	char str[20];
	level_atual(str);
	int level = char_number_2_int(str);

	COMECAR_HTML;
	printf("<title>GandaGalo - Level %d</title>\n", level);

	printf("<head>\n");
	//if(refresh >= 0){
	//	printf("<meta http-equiv=\"refresh\" content=\"%d;url=http://localhost/cgi-bin/GandaGalo?8\" />", refresh);
	//}
	printf("<style>\n");
	switch (validar)
	{
		case 1 :
					printf("h1{background-color: green; color: white;font-family: Comic Sans MS}\n");
					break;
		case 0 :
					printf("h1{background-color: red; color: white;font-family: Comic Sans MS}\n");
					break;
		default:
					printf("h1{background-color: blue; color: white;font-family: Comic Sans MS}\n");
					break;
	}
	printf("</style>\n");
	printf("</head>\n");

	printf("<body>\n");

	printf("<center>\n");
	switch (validar)
	{
		case 1 :
					printf("<h1>VALIDO</h1>\n");
					break;
		case 0 :
					printf("<h1>ERRADO</h1>\n");
					break;
		default:
					printf("<h1>GandaGalo</h1>\n");
					break;
	}

	ABRIR_SVG((e.num_cols + 2) * (tamanho_imagens), e.num_lins * tamanho_imagens + (3 * tamanho_novo /4));
	int l,c;

	for (l = 0; l < e.num_lins; ++l){
		for (c = 0; c < e.num_cols; ++c){
			if(e.grelha[l][c] == VAZIA || e.grelha[l][c] == SOL_O || e.grelha[l][c] == SOL_X){
				criaLink(l,c,e);
			}

			IMAGEM((c+1),l,tamanho_imagens, ficheiros[(VALOR) e.grelha[l][c]]);

			if(e.grelha[l][c] == VAZIA || e.grelha[l][c] == SOL_O || e.grelha[l][c] == SOL_X){
				FECHAR_LINK;
			}
		}
	}

	int num_cols_buffer = e.num_cols + 2;

	ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?6");
	
	// desenha novo.png em baixo do puzzle no meio
	printf("<image x=%d y=%d width=%d height=%d xlink:href=%snovo.png />\n", \
		((num_cols_buffer * tamanho_imagens) / 2) - tamanho_novo / 2, e.num_lins * tamanho_imagens, tamanho_novo, tamanho_novo, IMAGE_PATH);

	FECHAR_LINK;

	if(validar == -2){
		ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?8");
	
		// desenha verify.png em baixo do puzzle no meio
		printf("<image x=%d y=%d width=%d height=%d xlink:href=%sverify.png />\n", \
			((num_cols_buffer * tamanho_imagens)) - tamanho_novo/2, e.num_lins * tamanho_imagens + tamanho_novo/4, tamanho_novo/2, tamanho_novo/2, IMAGE_PATH);

	FECHAR_LINK;
	}
	if(validar == -1 || validar == 0){
		ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?7");
	
		// desenha verify.png em baixo do puzzle no meio
		printf("<image x=%d y=%d width=%d height=%d xlink:href=%sverify.png />\n", \
			((num_cols_buffer * tamanho_imagens)) - tamanho_novo/2, e.num_lins * tamanho_imagens + tamanho_novo/4, tamanho_novo/2, tamanho_novo/2, IMAGE_PATH);

		FECHAR_LINK;
	}
		char add = 's';
		if(hint_num == 1) add = ' ';
		printf("<a xlink:title=\"%d Hint%c\" href=\"http://localhost/cgi-bin/GandaGalo?2\">\n", hint_num, add);

	// desenha hint.png em baixo do puzzle no meio
		printf("<image x=%d y=%d width=%d height=%d xlink:href=%shint.png />\n", \
			((num_cols_buffer * tamanho_imagens)/2) + ((tamanho_novo/3)*2) , e.num_lins * tamanho_imagens + tamanho_novo/4, tamanho_novo/2, tamanho_novo/2, IMAGE_PATH);
		FECHAR_LINK;
		int verificar = verificar_ficheiro();
		if(verificar > 0){
		ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?5");

		printf("<image x=%d y=%d width=%d height=%d xlink:href=%sancora.png />\n", \
			((num_cols_buffer * tamanho_imagens)/2) - ((tamanho_novo/3)*4) - tamanho_novo/2, e.num_lins*tamanho_imagens + tamanho_novo/4 + 5, tamanho_novo/3, tamanho_novo/3, IMAGE_PATH);
		FECHAR_LINK;
		}

		char send_ancora[MAX_BUFFER];
		sprintf(send_ancora, "http://localhost/cgi-bin/GandaGalo?*%s", estado2str(e));
		ABRIR_LINK(send_ancora);

	// desenha ancora.png em baixo do puzzle no meio
		printf("<image x=%d y=%d width=%d height=%d xlink:href=%sancora.png />\n", \
			0, e.num_lins * tamanho_imagens + tamanho_novo/4 + 5, tamanho_novo/3, tamanho_novo/3, IMAGE_PATH);
		FECHAR_LINK;

	if(verificar > 0){
		ABRIR_LINK("http://localhost/cgi-bin/GandaGalo?3");

		printf("<image x=%d y=%d width=%d height=%d xlink:href=%sreturn.png />\n", \
			((num_cols_buffer * tamanho_imagens)/2) - ((tamanho_novo/3)*2) - tamanho_novo/2, e.num_lins * tamanho_imagens + tamanho_novo/4, tamanho_novo/2, tamanho_novo/2, IMAGE_PATH);

		FECHAR_LINK;
	}

	FECHAR_SVG;


	printf("<h1>Level %d</h1>\n", level);
	printf("</center>\n");
	printf("</body>\n");
	FECHAR_HTML;
}


/**
Função principal do programa
@returns 0 se tudo correr bem
*/
int main() {
	
	char *query = getenv("QUERY_STRING");

	int validar = -1;
	//ESTADO e2;
	ID current_user = malloc(sizeof(struct user_id));
	char level[808];

	

	
	if (query != NULL && strlen(query) != 0) {
		ESTADO e;
		int count = 0;
		int i = 0;
		
		switch (query[0]) {
			case '9' :
				intrucoes();
				break;
			case '7' :
				get_id_username(current_user);
				e = buscar_ultimo_movimento(1);
				e = resolver_GandaGalo(e, &validar, 1);
				level_atual (level);
				i  = char_number_2_int(level);
				if(i == current_user->level){
					escrever_username_file(current_user->user, estado2str(e), 1);
				}
				if(verificar_ficheiro() == -1){
					guardar_string_avancado("\0", 0);
					guardar_string_avancado(estado2str(e), 1);
				}else{
					guardar_string_avancado(estado2str(e), 0);
				}
				if(check_preenchido(e)) validar = -2;
				else validar = -1;
				puzzle(e, validar, current_user->hint_num);
				break;

			case '8' :
				e = buscar_ultimo_movimento(0);
				if(e.num_lins == 0 && e.num_cols == 0){
					get_id_username(current_user);
					menu(current_user, validar);
					break;
				}

				validar = puzzle_valido(e);
				get_id_username(current_user);
				//puzzle(e, validar);
				if(validar == 1){

					
					level_atual (level);

					i  = char_number_2_int(level);
					if(i == current_user->level){
						sprintf(level, "%d", i+1);
						escrever_username_file(current_user->user, level, 0);
						escrever_username_file(current_user->user, "\0", 1);
						if(current_user->hint_num < 10){
							current_user->hint_num++;
							sprintf(level, "%d", current_user->hint_num);
							escrever_username_file(current_user->user, level, 2);
						}
					}
					
					i =1;
				}
					if(i != 1){
						//puzzle(e, validar, 3, current_user->hint_num);
						puzzle(e, validar, current_user->hint_num);

					} else{
						get_id_username(current_user);
						apagar_history();
						menu(current_user, validar);
					}
				
				break;
			case '2':
				e = buscar_ultimo_movimento(0);

				if(e.num_lins == 0 && e.num_cols == 0){
					get_id_username(current_user);
					menu(current_user, validar);
					break;
				}

				get_id_username(current_user);
				if(current_user->hint_num && !comparar_estados(e, ajuda_better(e))){
					current_user->hint_num--;
					e = ajuda_better(e);
					sprintf(level, "%d", current_user->hint_num);
					escrever_username_file(current_user->user, level, 2);
					escrever_username_file(current_user->user, estado2str(e), 1);
				}
				validar = check_preenchido(e);
				guardar_string_avancado(estado2str(e), 0);
				//puzzle(e, validar, 0, current_user->hint_num);
				puzzle(e, validar, current_user->hint_num);
				break;
			case '3':
				e = buscar_ultimo_movimento(0);

				if(e.num_lins == 0 && e.num_cols == 0){
					get_id_username(current_user);
					menu(current_user, validar);
					break;
				}

				if(verificar_ficheiro() > 0){
					char zero[] = "\0";
					guardar_string_avancado(zero, 0);
				}

				e = buscar_ultimo_movimento(0);
				get_id_username(current_user);
				escrever_username_file(current_user->user, estado2str(e), 1);
				//puzzle(e, validar, 0, current_user->hint_num);
				puzzle(e, validar, current_user->hint_num);
				break;
			case '5':
 			e = procurar_ultima_ancora();
				get_id_username(current_user);
				//puzzle(e,validar, 0, current_user->hint_num);
				puzzle(e,validar, current_user->hint_num);
				break;

			case '6':
				apagar_history();
				get_id_username(current_user);
				menu(current_user, validar);
				break;

			case 'u':
				if(strlen(query) > 15 || strlen(query) < 8){
						username(0);
						return 0;
					}
				count = 5;
				while(query[count] != '\0'){
					if((query[count] <= 'z' && query[count] >= 'a') || (query[count] <= 'Z' && query[count] >= 'A')){
						if(query[count] <= 'Z' && query[count] >= 'A'){
							query[count] += 32;
						}
						query[i++] = query[count++];
					} else {
						username(0);
						return 0;
					}
				}
				query[i] = query[count];

				username_file(query, current_user);
				salvar_current_user(query);
				menu(current_user, validar);
				break;

			default:
				get_id_username(current_user);
				if(query[0] == '*'){
					e = str2estado(query+1);
					validar = check_preenchido(e);
					guardar_current_level(query+1);
					level_atual (level);
					i  = char_number_2_int(level);
					if(i == current_user->level){
						escrever_username_file(current_user->user, estado2str(e), 1);
					}
					guardar_string_avancado("\0", 0);
					guardar_string_avancado(estado2str(e), 1);
				}
				else{
					e = str2estado(query);
					validar = check_preenchido(e);
					guardar_current_level(query);
					level_atual (level);
					i  = char_number_2_int(level);
					if(i == current_user->level){
						escrever_username_file(current_user->user, estado2str(e), 1);
					}
					if(verificar_ficheiro() == -1){
						guardar_string_avancado("\0", 0);
						guardar_string_avancado(estado2str(e), 1);
					}else{
						guardar_string_avancado(estado2str(e), 0);
					}
				}

				//puzzle(e, validar, 0, current_user->hint_num);
				puzzle(e, validar, current_user->hint_num);
				break;
		}
	} else {
		apagar_history();
		salvar_current_user("\0");
		username(1);
	}

	return 0;
}