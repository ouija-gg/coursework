prog2: server client
	
server:
	gcc -std=c99 -o server server.c

client: 
	gcc -std=c99 -o client client.c

test: prog2
	./server &
	sleep 1
	./client 4 5 6 1 &
	sleep 1
	./client 88 48 29 399 &
	sleep 1
	./client 93 23 56 294

clean:
	rm client server fifo_to_* 
