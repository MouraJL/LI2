CFLAGS=-std=c11 -Wall -Wextra -pedantic -g
FICHEIROS=../estado.c ../estado.h gerar.c ../ajuda.h ../ajuda.c ../ficheiro.c ../ficheiro.h ../auxiliares.c ../auxiliares.h ../resolver.c ../resolver.h Makefile 
EXECUTAVEL=gerar

install: $(EXECUTAVEL)
	sudo cp $(EXECUTAVEL) /usr/lib/cgi-bin
	touch install

$(EXECUTAVEL): gerar.o ../estado.o ../ajuda.o ../ficheiro.o ../auxiliares.o ../resolver.o
	cc -o $(EXECUTAVEL) gerar.o ../estado.o ../ajuda.o ../ficheiro.o ../auxiliares.o ../resolver.o

gerar.zip: $(FICHEIROS)
	zip -9 gerar.zip $(FICHEIROS)

../ajuda.o: ../estado.h ../auxiliares.h ../ajuda.c ../ajuda.h ../ficheiro.c ../ficheiro.h
../auxiliares.o: ../estado.h ../auxiliares.c ../auxiliares.h
../ficheiro.o:../estado.h ../auxiliares.h ../ficheiro.c ../ficheiro.h
../estado.o: ../estado.c ../estado.h
../resolver.o: ../estado.h ../ficheiro.h ../ancoras.h ../ajuda.h ../auxiliares.h ../resolver.h ../resolver.c ../resolver.h
gerar.o: gerar.c ../estado.h ../ajuda.h ../auxiliares.h ../ficheiro.h
