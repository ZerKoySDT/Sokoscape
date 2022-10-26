bin/prog: objet/main.o objet/niveau.o objet/affichageLevel.o objet/evenement.o objet/chrono.o
	gcc -o bin/prog objet/main.o objet/niveau.o objet/affichageLevel.o objet/evenement.o objet/chrono.o -lSDL2 -lSDL2_image -lSDL2_ttf
objet/main.o: src/main.c
	gcc -o objet/main.o -c src/main.c
objet/chrono.o: src/chrono.c
	gcc -o objet/chrono.o -c src/chrono.c
objet/evenement.o: src/evenement.c
	gcc -o objet/evenement.o -c src/evenement.c
objet/niveau.o:	src/niveau.c
	gcc -o objet/niveau.o -c src/niveau.c
objet/affichageLevel.o: src/affichageLevel.c
	gcc -o objet/affichageLevel.o -c src/affichageLevel.c

clean:
	rm -f bin/prog objet/*.o
