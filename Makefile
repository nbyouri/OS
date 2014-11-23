CC=	clang -Weverything -Wno-padded -Wno-missing-noreturn -Wno-format-nonliteral

SERVER_SRCS=	server.c tools.c
SERVER_OUT=	server
CLIENT_SRCS=	pilot.c tools.c
CLIENT_OUT=	pilot
OBJECTS=	${SERVER_OUT} ${CLIENT_OUT}

all:
	> pilot
	@${CC} ${CLIENT_SRCS} -o ${CLIENT_OUT}
	> server
	@${CC} ${SERVER_SRCS} -o ${SERVER_OUT}

clean: delete_objs

delete_objs:
	for file in ${OBJECTS}; do	\
	test -f $$file && rm $$file;	\
	done

