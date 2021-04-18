obj_test = tree.o test.o Akinator.o
obj_main = tree.o main.o Akinator.o

test: $(obj_test)
	g++ -o test $(obj_test)

tree.o: tree.cpp
	g++ -c tree.cpp

test.o: test.cpp
	g++ -c test.cpp

Akinator.o: Akinator.cpp
	g++ -c Akinator.cpp

main: $(obj_main)
	g++ -o main $(obj_main)

main.o:
	g++ -c main.cpp


.PHONY : clean
clean:
	rm -rf *.o test
	rm -rf *.o main