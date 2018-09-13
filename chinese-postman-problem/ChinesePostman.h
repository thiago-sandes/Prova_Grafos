#include "Dijkstra.h"
#include "./cpp/Matching.h"
#include "./cpp/Graph.h"

bool Connected(const Graph & G)
{
    vector<bool> visited(G.GetNumVertices(), false);
    list<int> L;
    
    int n = 0;
    L.push_back(0);
    while(not L.empty())
    {
        int u = L.back();
        L.pop_back();
        if(visited[u]) continue;
        
        visited[u] = true;
        n++;
        
        for(list<int>::const_iterator it = G.AdjList(u).begin(); it != G.AdjList(u).end(); it++)
		{
			int v = *it;
		    L.push_back(v);
		}
    }
   
    return G.GetNumVertices() == n;
}

/*
Função do Chinese Postman Problem.
*/
pair< list<int>, double > ChinesePostman(const Graph & G, const vector<double> & cost)
{
	//Grafo é conectado?
	if(not Connected(G))
		throw "Error: Grafo não é conectado";

	//Construindo listas de adjacências usando arestas do grafo.
	vector< list<int> > A(G.GetNumVertices(), list<int>());
	for(int u = 0; u < G.GetNumVertices(); u++)
	    A[u] = G.AdjList(u);

	//Encontrando vértices de grau ímpar.
	vector<int> odd;
	for(int u = 0; u < G.GetNumVertices(); u++)
		if(A[u].size() % 2)
			odd.push_back(u);

    //Se há vértice de grau ímpar...
	if(not odd.empty())
	{
		//Crie um grafo com os vértices de graus ímpares
		Graph O(odd.size());
		for(int u = 0; u < (int)odd.size(); u++)
			for(int v = u+1; v < (int)odd.size(); v++)
				O.AddEdge(u, v);

        vector<double> costO(O.GetNumEdges());
        
        //Find the shortest paths between all odd degree vertices
		vector< vector<int> > shortestPath(O.GetNumVertices());
		for(int u = 0; u < (int)odd.size(); u++)
		{
			pair< vector<int>, vector<double> > sp = Dijkstra(G, odd[u], cost);
			
			shortestPath[u] = sp.first ;
			
			//The cost of an edge uv in O will be the cost of the corresponding shortest path in G
			for(int v = 0; v < (int)odd.size(); v++)
			    if(v != u)
    			    costO[ O.GetEdgeIndex(u, v) ] = sp.second[odd[v]];
		}

	    //Encontre a correspondência perfeita de custo mínimo do grafo dos vértices de graus ímpares
	    Matching M(O);
	    pair< list<int>, double > p = M.SolveMinimumCostPerfectMatching(costO);
	    list<int> matching = p.first;
	    
	    //Se uma aresta u-v estiver na correspondência, as arestas no caminho mais curto de u para v devem ser duplicadas em G
	    for(list<int>::iterator it = matching.begin(); it != matching.end(); it++)
	    {
		    pair<int, int> p = O.GetEdge(*it);
		    int u = p.first, v = odd[p.second];
		    
		    //Percorra o caminho adicionando as arestas
		    int w = shortestPath[u][v];
		    while(w != -1)
		    {
		        A[w].push_back(v);
		        A[v].push_back(w);
		        v = w;
		        w = shortestPath[u][v];
		    }
	    }
	}

	//Encontre um ciclo Euleriano no grafo sugerido por A
	list<int> cycle;
	//Isso é para acompanhar quantas vezes podemos percorrer uma aresta
	vector<int> traversed(G.GetNumEdges(), 0);
	for(int u = 0; u < G.GetNumVertices(); u++)
	{
		for(list<int>::iterator it = A[u].begin(); it != A[u].end(); it++)
		{
			int v = *it;
			
			//isso é para que a aresta não seja contada duas vezes
			if(v < u) continue;

			traversed[G.GetEdgeIndex(u, v)]++;
		}
	}

	cycle.push_back(0);
	list<int>::iterator itp = cycle.begin();
	double obj = 0;
	while(itp != cycle.end())
	{
		//Seja v o vértice atual no ciclo, começando no primeiro
		int u = *itp;
		list<int>::iterator jtp = itp;
		jtp++;

		//se houver aresta não atravessadas incidentes em u, encontre um subciclo começando em u substitua u no ciclo pelo subciclo
		while(not A[u].empty())
		{
			while(not A[u].empty() and traversed[ G.GetEdgeIndex(u, A[u].back()) ] == 0)
				A[u].pop_back();

			if(not A[u].empty())
			{
				int v = A[u].back();
				A[u].pop_back();
				cycle.insert(jtp, v);
				traversed[G.GetEdgeIndex(u, v)]--;
		        obj += cost[ G.GetEdgeIndex(u, v) ];
				u = v;
			}
		}

		//Vá para o próximo vértice no ciclo e faça o mesmo
		itp++;
	}

	return pair< list<int>, double >(cycle, obj);
}
