shell: main.o logics.o
	g++ -o shell main.o logics.o; rm *.o
logics.o: logics.cpp header.h
	g++ -Wall -c logics.cpp
main.o: main.cpp header.h
	g++ -Wall -c main.cpp
	
