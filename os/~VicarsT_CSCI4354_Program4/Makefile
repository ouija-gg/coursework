prog3: server client
	
server:
	gcc -std=c99 -g -o server queue.c server.c

client: 
	gcc -std=c99 -g -o client client.c

test: prog3
	./client 30 &
	sleep .5
	./client 50 &
	sleep .5
	./client 35
	sleep 5
	./client 30
	sleep .5
	./client 40

clean:
	rm client server fifo_to_* 
