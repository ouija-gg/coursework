prog1: server client
	
server:
	gcc -o server server.c

client: 
	gcc -o client client.c

test: prog1
	./server &
	sleep 1
	./client

clean:
	rm client server fifo*
