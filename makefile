all: server servers semaphores cliente
server:
	gcc -o server server.c
servers:
	gcc -o servers servers.c
semaphores:
	gcc -o semaphores semaphores.h
cliente:
	gcc -o cliente cliente.c
