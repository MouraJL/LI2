#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ficheiro.h"
#include "estado.h"
#include "auxiliares.h"

/**
Função que lê o ficheiro puzzle e o passa para um estado
*/
ESTADO ler_ficheiro(int numero){

	ESTADO e = resetEstado();
	char num_lins[4];
	char num_cols[4];
	char file[50];
	sprintf(file, "/var/www/html/puzzles/puzzle_%d.txt", numero); //funciona !!!!

	int ch;
	FILE *arq;
	arq = fopen(file, "r"); //abre o ficheiro de texto
	//caso o ficheiro esteja vazio retoenar à função main 

	if (!arq) return e;

	int l = 0, c = 0;

	// while ((ch = getc(arq)) != '\n'){
	// 	c = 0;
	// }
	int count = 0;
	while((ch = getc(arq)) <= '9' && ch >= '0') num_lins[count++] = ch;
	num_lins[count] = '\0';
	count = 0;
	while((ch = getc(arq)) <= '9' && ch >= '0') num_cols[count++] = ch;
	num_cols[count] = '\0';

	e.num_lins = char_number_2_int(num_lins);
	e.num_cols = char_number_2_int(num_cols);

	while(ch != '\n'){
		ch = getc(arq);
	}
	count = 1;
	int max = e.num_lins * e.num_cols + e.num_lins-1;

	while ((ch = getc(arq)) != EOF && max >= count){
		count++;
		if ((char) ch != '\n'){
			e.grelha[l][c] = (int) txt_peca((char) ch);
			c++;
		} else{
			c = 0;
			l++;
		} 
	} //EOF = End of File

  	fclose(arq);

  	return e;
}

/**
Função que lê o ficheiro puzzle e verifica se este está vazio
*/
int check_file(int i){
	char file[50];
	sprintf(file, "/var/www/html/puzzles/puzzle_%d.txt", i); //funciona !!!!

	FILE *arq;
	arq = fopen(file, "r"); //abre o ficheiro de texto
	//caso o ficheiro esteja vazio retoenar à função main

	if (!arq) return 0;
	else{
		fclose(arq);
		return 1;
	}
}

/**
Função que verifica se o ficheiro history tem mais de um movimento (sendo que o primeiro é a o puzzle no seu estado inicial)
*/
int verificar_ficheiro(){
	FILE * history;
	char file[50];
	sprintf(file, "/var/www/html/puzzles/history.txt"); //funciona !!!!
	char str[MAX_BUFFER];
	int c;

	history = fopen(file, "r");

	c = fgetc(history);
	if(c == EOF){
		fclose(history);
		return -1;
	}
	int count = 0;
	while((c = fgetc(history)) != '\n' && c != EOF);
	while((c = fgetc(history)) != '\n' && c != EOF) str[count++] = c;
	str[count] = '\0';

	fclose(history);
	
	if(str[0] == '\0') return 0;
	else return 1;
}

/**
Função que verifica se a peça que esta a ser modificada é a mesma que a anterior, se não for guarda no ficheiro a peca atual modificada
*/
int peca_modificada(char * query){
	ESTADO atual, past;
	char str[MAX_BUFFER];
	FILE *check, *fptr1;
	char *file = "/var/www/html/puzzles/history.txt";
	char *file_check = "/var/www/html/puzzles/check.txt";
	int c1, count = 0, f, l, c;
	int change_line, change_cols;
	int check_line = -1, check_cols = -1;

	fptr1 = fopen(file, "r");
	check = fopen(file_check, "r");

	atual = str2estado(query);

	c1 = fgetc(check);
	if (c1 == EOF){
		fclose(check);
		check = fopen(file_check, "w");

		for (l = 0; l < atual.num_lins; ++l){
			for (c = 0; c < atual.num_cols; ++c){
				fprintf(check, "%c", '0');
			}
			if(l != (atual.num_lins - 1)){
				fprintf(check, "\n");
			}
		}
		fclose(check);
	} else{
		f = fgetc(fptr1);
		while(f != '\n' && f != EOF){
			str[count++] = f;
			f = fgetc(fptr1);
		}
		str[count] = '\0';
		if(str[0] == '*') past = str2estado(str+1);
		else past = str2estado(str);

		for (l = 0; l < atual.num_lins; ++l){
			for (c = 0; c < atual.num_cols; ++c){
				if(past.grelha[l][c] != atual.grelha[l][c]){
					change_line = l;
					change_cols = c;
				}
			}
		}

		for (l = 0; l < atual.num_lins; ++l){
			for (c = 0; c < atual.num_cols; ++c){
				if(c1 == '1'){
					check_line = l;
					check_cols = c;
				}
				c1 = fgetc(check);
				if(c1 == '\n') c1 = fgetc(check);
			}
		}

		fclose(check);

		if(check_line == change_line && check_cols == change_cols){
			return 0;
		}
		else{
			check = fopen(file_check, "w");
			for (l = 0; l < atual.num_lins; ++l){
				for (c = 0; c < atual.num_cols; ++c){
					if(c == change_cols && l == change_line) fprintf(check, "%c", '1');
					else fprintf(check, "%c", '0');
				}
				if(l != (atual.num_lins - 1)) fprintf(check, "\n");
			}
			fclose(check);
		}
		return 1;
	}

	fclose(fptr1);
	return 1;
}

/**
Função que guarda o estado no ficheiro history
*/
void guardar_string_avancado(char * query, int ancora){

    FILE *fptr1, *fptr2;
    int c, count = 0, check_asterisco = 0, igual;
    char str[MAX_BUFFER], buffer[MAX_BUFFER];
    int check = peca_modificada(query);    

    char *temp = "/var/www/html/puzzles/temp.txt";
	char *file = "/var/www/html/puzzles/history.txt";

	fptr1 = fopen(file, "r");

	c = fgetc(fptr1);
	while(c != '\n' && c != EOF)
	{
		if(c != '*') str[count++] = c;
		else check_asterisco = 1;
		c = fgetc(fptr1);
	}
	str[count] = '\0';
	igual = strcmp(str, query);
	if (igual || c == EOF || ancora){

    	fptr2 = fopen(temp, "w");

    	if(query[0] != '\0'){
    		if(ancora){
    			strcpy(buffer, query);
    			sprintf(query, "*%s", buffer);
    		}
    		fprintf(fptr2, "%s\n", query);
    	}
    	
        if(c != EOF){

        	if(check && query[0] != '\0' && igual){
        		if(check_asterisco) fprintf(fptr2, "*%s\n", str);
        		else fprintf(fptr2, "%s\n", str);
        	} 

        	c = fgetc(fptr1);
			while (c != EOF){	
    			count = 0;
    			while (c != EOF && c != '\n'){
    				str[count++] = c;
    				c = fgetc(fptr1);
    			} 
    			str[count] = '\0';
    			fprintf(fptr2, "%s\n", str);
    			c = fgetc(fptr1);
    		}
    	}
    	fclose(fptr1);
    	fclose(fptr2);

    	fptr1 = fopen(file, "w");
    	fptr2 = fopen(temp, "r");

    	c = fgetc(fptr2);

    	while (c != EOF){
    		count = 0;
    		while (c != EOF && c != '\n'){ 
    			str[count++] = c;
    			c = fgetc(fptr2);
    		}
    		fprintf(fptr1, "%s\n", str);
    		c = fgetc(fptr2);
    	}

    	fclose(fptr1);
    	fclose(fptr2);

    	fptr2 = fopen(temp, "w");
    	fclose(fptr2);
	} else fclose(fptr1);
}

/**
Função que apaga o conteudo do ficheiro history e do ficheiro check
*/
void apagar_history(){
	FILE *file, *check;
    char *filename = "/var/www/html/puzzles/history.txt";
    char *file_check = "/var/www/html/puzzles/check.txt";

    file = fopen(filename, "w");
    check = fopen(file_check, "w");
    fclose(file);
    fclose(check);
}

/**
Função que busca o ultimo movimento feito ao ficheiro history
*/
ESTADO buscar_ultimo_movimento(int check){
	FILE *file;
	ESTADO e;
	char *filename = "/var/www/html/puzzles/history.txt";
	int c, count = 0;
    char str[MAX_BUFFER];

	file = fopen(filename, "r");

	while((c = fgetc(file)) != '\n' && c != EOF) str[count++] = c;
	str[count] = '\0';

	if(str[0] == '\0' && !check) e.num_lins = e.num_cols = 0;
	else{
		if(str[0] == '*') e = str2estado(str+1);
		else e = str2estado(str);
	} 
	
	fclose(file);

	return e;
}

ESTADO procurar_ultima_ancora(){//ainda com problema
	FILE *fptr1, *fptr2;
	ESTADO e;
	char *temp = "/var/www/html/puzzles/temp.txt";
	char *file = "/var/www/html/puzzles/history.txt";
	char str[MAX_BUFFER], buffer[MAX_BUFFER];
	int c, count = 0;

	fptr1 = fopen(file, "r");
	if((c = fgetc(fptr1)) == '*' && !verificar_ficheiro()){}
	else{
		while((c = fgetc(fptr1)) != '*' && c != EOF);
	}
	while((c = fgetc(fptr1)) != '\n' && c != EOF) str[count++] = c;
	str[count] = '\0';

	e = str2estado(str);

	c = fgetc(fptr1);

	strcpy(buffer, str);
    sprintf(str, "*%s", buffer);

	fptr2 = fopen(temp, "w");
	fprintf(fptr2, "%s\n", str);

	if(c != EOF){
		while(c != EOF){
			count = 0;
			while(c != EOF && c != '\n'){
				str[count++] = c;
				c = fgetc(fptr1);
			}
			str[count] = '\0';
			fprintf(fptr2, "%s\n", str);
			c = fgetc(fptr1);
		}
	}

	fclose(fptr1);
	fclose(fptr2);

	fptr1 = fopen(file, "w");
	fptr2 = fopen(temp, "r");

	c = fgetc(fptr2);
	while(c != EOF){
		count = 0;
		while(c != EOF && c != '\n'){
			str[count++] = c;
			c = fgetc(fptr2);
		}
		str[count] = '\0';
		c = fgetc(fptr2);
		fprintf(fptr1, "%s\n", str);
	}

	fclose(fptr1);
	fclose(fptr2);

	return e;
}

void guardar_current_level(char *query){
	ESTADO e = str2estado(query);
	for (int l = 0; l < e.num_lins; ++l)
	{
		for (int c = 0; c < e.num_cols; ++c)
		{
			if(e.grelha[l][c] == SOL_O || e.grelha[l][c] == SOL_X) e.grelha[l][c] = VAZIA;
		}
	}
	char *buffer = estado2str(e);
	//buffer[804] = '\0';
	strcpy(query, buffer);


	FILE * history;
	char file[50];
	sprintf(file, "/var/www/html/puzzles/history.txt"); //funciona !!!!
	int ch;

	history = fopen(file, "r");
	ch = fgetc(history);
	if(ch != '0')
	{
		FILE * levels;
		FILE * current_level;
		char str[MAX_BUFFER];

		char file_levels[50];
		sprintf(file_levels, "/var/www/html/puzzles/levels.txt");

		char file_current_level[50];
		sprintf(file_current_level, "/var/www/html/puzzles/current_level.txt");

		levels = fopen(file_levels, "r");

		int count = 0;
		str[0] = '\0';
		while(strcmp(str, query) != 0)
		{
			fgets(str, MAX_BUFFER, levels);
			str[804] = '\0';
			count++;
		}
		
		str[804] = '\0';
		char test[805];
		strcpy(test, str);

		fclose(levels);


		current_level = fopen(file_current_level, "w");
		fprintf(current_level, "%s\n", test);
		fprintf(current_level, "%d\n", count);

		fclose(current_level);
	}

	fclose(history);
}

/**
Função que verifica se o user se encontra na data base (ficheiro Users) e o adiciona ( se nao estiver), se estiver obtem as suas informações e coloca no ID current_user
*/
void username_file(char *str, ID current_user){
	FILE * users;
	char file[50] = "/var/www/html/puzzles/Users.txt";
	char user1[805];
	user1[0] = '\0';
	int ch, count = 0;

	current_user->user[0] = '\0';
	current_user->level = 0;
	current_user->query[0] = '\0';
	current_user->hint_num = 0;

	users = fopen(file, "a+");
	ch = fgetc(users);
	if(ch != EOF){
		fclose(users);
		users = fopen(file, "a+");

		while(strcmp(str, user1) && ch != EOF){
			count = 0;
			ch = fgetc(users);
			
			while(ch != '\n' && ch != EOF){
				user1[count++] = ch;
				ch = fgetc(users);
			}
			user1[count] = '\0';
		}

		if(ch == EOF){
			fprintf(users, "%s\n", str);
			fprintf(users, "%s\n", "1");
			fprintf(users, "\n");
			fprintf(users, "%s\n", "5");

			strcpy(current_user->user, str);
			current_user->level = 1;
			current_user->query[0] = '\0';
			current_user->hint_num = 5;
		} else{
			strcpy(current_user->user, str);

			count = 0;
			ch = fgetc(users);
			while(ch != '\n' && ch != EOF){
				user1[count++] = ch;
				ch = fgetc(users);
			}
			user1[count] = '\0';

			current_user->level = char_number_2_int(user1);

			user1[0] = '\0';
			count = 0;
			ch = fgetc(users);
			while(ch != '\n' && ch != EOF){
				user1[count++] = ch;
				ch = fgetc(users);
			}
			user1[count] = '\0';

			strcpy(current_user->query, user1);

			count = 0;
			ch = fgetc(users);
			while(ch != '\n' && ch != EOF){
				user1[count++] = ch;
				ch = fgetc(users);
			}
			user1[count] = '\0';

			current_user->hint_num = char_number_2_int(user1);
		}
	} else{
		fprintf(users, "%s\n", str);
		fprintf(users, "%s\n", "1");
		fprintf(users, "\n");
		fprintf(users, "%s\n", "5");

		strcpy(current_user->user, str);
		current_user->level = 1;
		current_user->query[0] = '\0';
		current_user->hint_num = 5;
	}
	
	fclose(users);
}

/**
Função que guarda o current user no ficheiro current_user
*/
void salvar_current_user(char user[]){
	FILE * users;
	char file[50] = "/var/www/html/puzzles/current_user.txt";
	users = fopen(file, "w");

	fprintf(users, "%s\n", user);

	fclose(users);
}

//onde =0 -> level
//onde =1 -> query
//onde =2 -> hints
/**
Função que modifica no ficheiro Users informações de cada user
*/
void escrever_username_file(char current_abc[], char escrever[], int onde){
	FILE * users;
	FILE * temp;
	char file[50] = "/var/www/html/puzzles/Users.txt";
	char file_temp[50] = "/var/www/html/puzzles/temp.txt";
	char user1[805];
	user1[0] = '\0';
	int ch = '\0', count = 0;


	users = fopen(file, "r");
	temp = fopen(file_temp, "w");
	fclose(temp);
	temp = fopen(file_temp, "a+");

	while(strcmp(current_abc, user1) && ch != EOF){
		count = 0;
		ch = fgetc(users);
			
		while(ch != '\n' && ch != EOF){
			user1[count++] = ch;
			ch = fgetc(users);
		}
		user1[count] = '\0';

		fprintf(temp, "%s\n", user1);
	}

	while(onde){
		count = 0;
		ch = fgetc(users);
			
		while(ch != '\n' && ch != EOF){
			user1[count++] = ch;
			ch = fgetc(users);
		}
		user1[count] = '\0';

		fprintf(temp, "%s\n", user1);
		onde--;
	}

	fprintf(temp, "%s\n", escrever);

	ch = fgetc(users);
	while(ch != '\n' && ch != EOF){
		ch = fgetc(users);
	}

	int check = 0;

	while(ch != EOF){
		if(check){
			fprintf(temp, "\n");
		} else check = 1;
		count = 0;
		ch = fgetc(users);
		while(ch != '\n' && ch != EOF){
			user1[count++] = ch;
			ch = fgetc(users);
		}
		user1[count] = '\0';

		fprintf(temp, "%s", user1);
	}

	fclose(temp);
	fclose(users);

	temp = fopen(file_temp, "r");
	users = fopen(file, "w");
	ch = '\0';

	check = 0;

	while(ch != EOF){
		if(check){
			fprintf(users, "\n");
		} else check = 1;
		count = 0;
		ch = fgetc(temp);
		while(ch != '\n' && ch != EOF){
			user1[count++] = ch;
			ch = fgetc(temp);
		}
		user1[count] = '\0';

		fprintf(users, "%s", user1);
	}

	fclose(users);
	fclose(temp);
}

/**
Função que vai buscar informaçao do current_user quando este já se encontra no ficheiro current_user
*/
void get_id_username(ID current_user){
	FILE * current;
	FILE * current1;
	char file[50] = "/var/www/html/puzzles/current_user.txt";
	char user[11];

	current1 = fopen(file, "r");
	fgets(user, 11, current1);


	char file1[50] = "/var/www/html/puzzles/Users.txt";
	char user1[808];
	user1[0] = '\0';

	current = fopen(file1, "r");

	while(strcmp(user, user1)) fgets(user1,808,current);
	user1[strlen(user1) - 1] = '\0';

	strcpy(current_user->user, user1);
	

	fgets(user1,808,current);
	user1[strlen(user1) - 1] = '\0';
	

	current_user->level = char_number_2_int(user1);

	fgets(user1,808,current);
	user1[strlen(user1) - 1] = '\0';
	

	strcpy(current_user->query, user1);

	fgets(user1,808,current);
	user1[strlen(user1) - 1] = '\0';
	

	current_user->hint_num = char_number_2_int(user1);

	fclose(current1);
	fclose(current);
}

/**
Função que obtem em str o nivel atual
*/
void level_atual (char str[]){
	FILE * current_level;
	current_level = fopen("/var/www/html/puzzles/current_level.txt", "r");

	fgets(str, 810, current_level);
	fgets(str, 20, current_level);
	fclose(current_level);
}
