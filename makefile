all:
	make server servers semaphores cliente serverMsg serversMsg clienteMsg
server:
	gcc -o server server.c
servers:
	gcc -o servers servers.c
semaphores:
	gcc -o semaphores semaphores.h
cliente:
	gcc -o cliente cliente.c
serverMsg:
	gcc -o serverMsg serverMsg.c
serversMsg:
	gcc -o serversMsg serversMsg.c
clienteMsg:
	gcc -o clienteMsg clienteMsg.c