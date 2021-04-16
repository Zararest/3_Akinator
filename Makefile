obj_test = tree.o test.o

test: $(obj_test)
	g++ -o test $(obj_test)

tree.o: tree.cpp
	g++ -c tree.cpp

test.o: test.cpp
	g++ -c test.cpp