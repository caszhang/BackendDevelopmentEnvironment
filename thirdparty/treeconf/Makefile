CFLAGS = -g -Wall
all: config.h config.c tstring.h
	gcc -c config.c 
	ar rv ./libtreeconf.a config.o	
	rm *.o
clean:
	rm -f config.o
	rm -rf ./libtreeconf.a	
