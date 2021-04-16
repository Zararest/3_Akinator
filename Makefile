obj_test = tree.o test.o Akinator.o

test: $(obj_test)
	g++ -o test $(obj_test)

tree.o: tree.cpp
	g++ -c tree.cpp

test.o: test.cpp
	g++ -c test.cpp

Akinator.o: Akinator.cpp
	g++ -c Akinator.cpp

.PHONY : clean
clean:
	rm -rf *.o test
	rm -rf *.o main