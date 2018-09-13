
FLAG = -O3 -m64 -Wall

Matching.o: ./cpp/Matching.cpp ./cpp/Matching.h ./cpp/BinaryHeap.h ./cpp/Globals.h ./cpp/Graph.h 
	g++ $(FLAG) -c ./cpp/Matching.cpp -o Matching.o

BinaryHeap.o: ./cpp/BinaryHeap.h ./cpp/BinaryHeap.cpp ./cpp/Globals.h
	g++ $(FLAG) -c ./cpp/BinaryHeap.cpp -o BinaryHeap.o

Graph.o: ./cpp/Graph.h ./cpp/Graph.cpp
	g++ $(FLAG) -c ./cpp/Graph.cpp -o Graph.o

Example.o: Example.cpp Dijkstra.h ./cpp/Matching.h ./cpp/Graph.h ChinesePostman.h
	g++ $(FLAG) -c Example.cpp -o Example.o

chinese: Matching.o BinaryHeap.o Graph.o Example.o 
	g++ $(FLAG) Matching.o BinaryHeap.o Graph.o Example.o -o chinese
	@echo "Os arquivos acima foram usados como ajuda para implementar esse algoritmo. Devido à dificuldade dele, procurei soluções para o problema que pudessem ajudar na implementação da solução da questão. "

