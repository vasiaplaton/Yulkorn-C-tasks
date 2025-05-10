all: build run

build: app.out

app.out: operations.o token.o tree.o queue.o stack.o stackTree.o lexer.o treeBuilder.o processTree.o main.o
	gcc operations.o token.o tree.o queue.o stack.o stackTree.o lexer.o treeBuilder.o processTree.o main.o -o app.out

operations.o: operations.c
	gcc -c operations.c -o operations.o

token.o: token.c
	gcc -c token.c -o token.o

tree.o: tree.c
	gcc -c tree.c -o tree.o

queue.o: queue.c
	gcc -c queue.c -o queue.o

stack.o: stack.c
	gcc -c stack.c -o stack.o

stackTree.o: stackTree.c
	gcc -c stackTree.c -o stackTree.o

lexer.o: lexer.c
	gcc -c lexer.c -o lexer.o

treeBuilder.o: treeBuilder.c
	gcc -c treeBuilder.c -o treeBuilder.o

processTree.o: processTree.c
	gcc -c processTree.c -o processTree.o

main.o: main.c
	gcc -c main.c -o main.o
	
run:
	./app.out

clean:
	rm *.o