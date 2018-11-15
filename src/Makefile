CFLAGS=-std=c11 -Wall -Wextra -pedantic -g
FICHEIROS=cgi.h estado.c estado.h interface.c ajuda.h ajuda.c ficheiro.c ficheiro.h auxiliares.c auxiliares.h ancoras.h resolver.c resolver.h Makefile 
EXECUTAVEL=GandaGalo

install: $(EXECUTAVEL)
	sudo cp $(EXECUTAVEL) /usr/lib/cgi-bin
	make imagens
	make ficheiros
	make puzzles
	touch install

$(EXECUTAVEL): interface.o estado.o ajuda.o ficheiro.o auxiliares.o resolver.o
	cc -o $(EXECUTAVEL) interface.o estado.o ajuda.o ficheiro.o auxiliares.o resolver.o

imagens:
	sudo mkdir -p /var/www/html/images
	sudo cp *.png /var/www/html/images

ficheiros:
	sudo mkdir -p /var/www/html/puzzles
	sudo cp *.txt /var/www/html/puzzles
	sudo chmod 777 /var/www/html/puzzles/*.txt
	
puzzles:
	sudo cp puzzle_*.txt /var/www/html/puzzles
	sudo chmod 777 /var/www/html/puzzles/puzzle_*.txt


GandaGalo.zip: $(FICHEIROS)
	zip -9 GandaGalo.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o $(EXECUTAVEL) Doxyfile latex html install
	sudo rm -rf /var/www/html/puzzles
	sudo rm -rf /var/www/html/imagens

estado.o: estado.c estado.h
ajuda.o: estado.h auxiliares.h ajuda.c ajuda.h ficheiro.c ficheiro.h
ficheiro.o: estado.h auxiliares.h ficheiro.c ficheiro.h
resolver.o: estado.h auxiliares.h ficheiro.h resolver.c resolver.h ancoras.h
auxiliares.o: estado.h auxiliares.c auxiliares.h
interface.o: interface.c auxiliares.h cgi.h estado.h ajuda.h ancoras.h
