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
Fun��o do Chinese Postman Problem.
*/
pair< list<int>, double > ChinesePostman(const Graph & G, const vector<double> & cost)
{
	//Grafo � conectado?
	if(not Connected(G))
		throw "Error: Grafo n�o � conectado";

	//Construindo listas de adjac�ncias usando arestas do grafo.
	vector< list<int> > A(G.GetNumVertices(), list<int>());
	for(int u = 0; u < G.GetNumVertices(); u++)
	    A[u] = G.AdjList(u);

	//Encontrando v�rtices de grau �mpar.
	vector<int> odd;
	for(int u = 0; u < G.GetNumVertices(); u++)
		if(A[u].size() % 2)
			odd.push_back(u);

    //Se h� v�rtice de grau �mpar...
	if(not odd.empty())
	{
		//Crie um grafo com os v�rtices de graus �mpares
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

	    //Encontre a correspond�ncia perfeita de custo m�nimo do grafo dos v�rtices de graus �mpares
	    Matching M(O);
	    pair< list<int>, double > p = M.SolveMinimumCostPerfectMatching(costO);
	    list<int> matching = p.first;
	    
	    //Se uma aresta u-v estiver na correspond�ncia, as arestas no caminho mais curto de u para v devem ser duplicadas em G
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
	//Isso � para acompanhar quantas vezes podemos percorrer uma aresta
	vector<int> traversed(G.GetNumEdges(), 0);
	for(int u = 0; u < G.GetNumVertices(); u++)
	{
		for(list<int>::iterator it = A[u].begin(); it != A[u].end(); it++)
		{
			int v = *it;
			
			//isso � para que a aresta n�o seja contada duas vezes
			if(v < u) continue;

			traversed[G.GetEdgeIndex(u, v)]++;
		}
	}

	cycle.push_back(0);
	list<int>::iterator itp = cycle.begin();
	double obj = 0;
	while(itp != cycle.end())
	{
		//Seja v o v�rtice atual no ciclo, come�ando no primeiro
		int u = *itp;
		list<int>::iterator jtp = itp;
		jtp++;

		//se houver aresta n�o atravessadas incidentes em u, encontre um subciclo come�ando em u substitua u no ciclo pelo subciclo
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

		//V� para o pr�ximo v�rtice no ciclo e fa�a o mesmo
		itp++;
	}

	return pair< list<int>, double >(cycle, obj);
}
