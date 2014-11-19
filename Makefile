CC=	clang -Wall

all:
	> pilot
	@${CC} pilot.c -o pilot
	> server
	@${CC} server.c -o server
