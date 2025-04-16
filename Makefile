generic_stack_test_.exe: main.o
	  g++ -o generic_stack_test.exe main.o

main.o: main.cpp
	  g++ -c main.cpp -o main.o

clean:
	rm *.o *.exe