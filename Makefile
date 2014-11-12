CC=	clang -Wall

all:
	${CC} pilot.c -o pilot
	${CC} server.c -o server
