all:
	gcc -Wall -c src/common.c -o obj/common.o
	gcc -Wall -c src/serv_utils.c -o obj/serv_utils.o
	gcc -Wall -c src/clt_utils.c -o obj/clt_utils.o
	gcc -Wall src/client.c obj/common.o obj/clt_utils.o -o bin/client
	gcc -Wall src/server.c obj/common.o obj/serv_utils.o -o bin/server

clean:
	rm obj/common.o obj/clt_utils.o obj/serv_utils.o bin/client bin/server
