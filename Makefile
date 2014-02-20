CFLAGS = -std=c99

all:
	gcc -std=c99 main.c -o fsdadm
	gcc -std=c99 read.c -o read

clean:
	rm -rf fsdadm read *.o
