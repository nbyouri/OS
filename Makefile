CC=	clang -Weverything -Wno-padded -Wno-missing-noreturn

all:
	> pilot
	@${CC} pilot.c -o pilot
	> server
	@${CC} server.c -o server
