#include "./cpp/Graph.h"
#include "ChinesePostman.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


pair< Graph, vector<double> > ReadWeightedGraph(string filename)
{
	//Ler dados da matriz de adjacência e transformá-la em grafo.

	ifstream fileTest;
	fileTest.open(filename.c_str());
	int countVertices = 0, countArestas = 0;
	string s;
	// contar quantidade de vértices
	while (getline(fileTest, s))
		countVertices++;

	// Testes
	//cout << countVertices << endl;
	//cout << s << endl;

	int valores[countVertices][countVertices];
	
	// inicializando matriz de adjacência
	for (int i = 0; i < countVertices; i++) {
		for (int j = 0; j < countVertices; j++){
			valores[i][j] = 0;
		}
	}

	fileTest.close();


	// Tratamento da matriz de adjacência
	ifstream file;
	file.open(filename.c_str());
	
	// Aqui escreve valores em matriz e conta-se qtd de arestas
	
	for (int y = 0; y < countVertices; y++) {
		for (int x = 0; x < countVertices; x++) {
			file >> valores[y][x];
			if (valores[y][x] != 0) countArestas++;
		}
	}

	cout << "Matriz de Adjacência" << endl;

	for (int y = 0; y < countVertices; y++) {
		for (int x = 0; x < countVertices; x++) {
			cout << valores[y][x];		
		}
		cout << endl;
	}

	countArestas = countArestas / 2;

	cout << "Quantidade de vértices: " << countVertices << endl;
	cout <<"Quantidade de arestas: " << countArestas << endl;


	
	Graph G(countVertices);
	vector<double> cost(countArestas);
	double c;

	for (int i = 0; i < countVertices; i++) {
		for (int j = 0; j < countVertices; j++) {
			if (valores[i][j] != 0) {
				G.AddEdge(i, j);
				c = valores[i][j];
				cost[G.GetEdgeIndex(i, j)] = c;
			}

		}
	}

	file.close();
	return make_pair(G, cost);
}

int main(int argc, char * argv[])
{
	string filename = "";

	int i = 1;
	while(i < argc)
	{
		string a(argv[i]);
		if(a == "-f")
			filename = argv[++i];
		i++;
	}

	if(filename == "")
	{
		cout << endl << "Use o nome do arquivo correto, com usa extensão: grafo.txt !";
		return 1;
	}

	try
	{
	    Graph G;
	    vector<double> cost;
	
	    //Lendo o grafo ...
	    pair< Graph, vector<double> > p = ReadWeightedGraph(filename);
	    G = p.first;
	    cost = p.second;

	    //Solução
     	pair< list<int> , double > sol = ChinesePostman(G, cost);

		cout << "Custo da solução: " << sol.second << endl;

		list<int> s = sol.first;

        //Imprimir arestas em solução
		cout << "Arestas da solução:" << endl;
		for(list<int>::iterator it = s.begin(); it != s.end(); it++)
			cout << *it << " ";
		cout << endl;
	}
	catch(const char * msg)
	{
		cout << msg << endl;
		return 1;
	}

	return 0;
}


