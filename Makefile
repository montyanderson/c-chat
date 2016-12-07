C_FLAGS:=--std=c99 -lpthread

default: client.out server.out

client.o:
	gcc client.c $(C_FLAGS) -o client.o

server.o:
	gcc server.c $(C_FLAGS) -o server.o

client.out:
	ld -lc client.o -o client.out

server.out:
	ld -lc server.o -o server.out
