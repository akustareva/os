all: sigusr
clean:
	rm -f sigusr.o sigusr
sigusr.o: sigusr.c
	gcc -c sigusr.c -o sigusr.o
sigusr: sigusr.o
	gcc -s sigusr.o -o sigusr