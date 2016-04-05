all: midtermsh
clean:
	rm -f midtermsh.o midtermsh
midtermsh.o: midtermsh.cpp
	g++ -std=c++11 -c midtermsh.cpp -o midtermsh.o
midtermsh: midtermsh.o
	g++ -s midtermsh.o -o midtermsh
