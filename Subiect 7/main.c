/*
  Se da un graf neorientat ponderat conex, cu n varfuri si m muchii-in care fiecare muchie are asociat un cost, numar natural
strict pozitiv
  Folosind algoritmul lui Prim, determinati un arbore partial de cost minim
*/

/* Algoritmul lui Prim funcționează prin extinderea arborelui de acoperire, adăugând la fiecare pas o nouă muchie minimă care conectează 
un nod din arborele deja construit cu un nod din afara acestuia.*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 100

// Structura pentru reprezentarea muchiilor
typedef struct Edge {
    int source;
    int destination;
    int weight;
} Edge;

// Structura pentru reprezentarea grafului
typedef struct Graph {
    int numVertices;
    int numEdges;
    Edge *edges;
} Graph;

// Functie pentru initializarea grafului
Graph *createGraph(int numVertices, int numEdges) 
{
    Graph *graph = (Graph *) malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->numEdges = numEdges;
    graph->edges = (Edge *) malloc(numEdges * sizeof(Edge));
    return graph;
}

// Functie pentru adaugarea unei muchii in graf
void addEdge(Graph *graph, int edgeIndex, int source, int destination, int weight) 
{
    graph->edges[edgeIndex].source = source;
    graph->edges[edgeIndex].destination = destination;
    graph->edges[edgeIndex].weight = weight;
}

// Functie pentru a gasi varful cu cel mai mic cost
int minKey(int key[], bool mstSet[], int numVertices) 
{
    int v;
    int min = INT_MAX, min_index;
    for (v = 0; v < numVertices; v++) 
    {
        if (mstSet[v] == false && key[v] < min) 
        {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

// Functie pentru a afisa arborele partial de cost minim
void printMST(int parent[], Edge edges[], int numVertices) 
{
    int i;
    printf("Muchiile arborelui partial de cost minim:\n");
    for (i = 1; i < numVertices; i++) 
    {
        printf("%d - %d\n", parent[i], i);
    }
}

// Functie pentru a calcula si a afisa arborele partial de cost minim folosind algoritmul lui Prim
void primMST(Graph *graph, int n) 
{
    int i, u, e, v;
    int parent[MAX_VERTICES];
    int key[MAX_VERTICES];
    bool mstSet[MAX_VERTICES];

    for (i = 0; i < n; i++) 
    {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }
    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < n - 1; count++) 
    {
      //aici practic caut si gasesc ponderea minima de pe muchiile din graf
      //u este un nod sursa, dar in el stocam indexul valorii minime al ponderii de pe muchiile din graf
      //adica nu stocam in el chiar valoarea ponderii, ci un 'indiciu'(indexul) ca din acest nod porneste o muchie cu pondere minima
        u = minKey(key, mstSet, n);
        mstSet[u] = true;

      //introducem in noul arbore de acoperire minima muchia minima recent gasita si stocata in u
        for (v = 0; v < n; v++) 
        {
            for (e = 0; e < graph->numEdges; e++) 
            {
                //creez o variabila edge pentru a pastra si lucra cu indexul e mai usor
                Edge edge = graph->edges[e];
                if (edge.source == u && mstSet[edge.destination]==false && edge.weight < key[edge.destination]) 
                // edge.source == u: Verifica daca nodul din care incepe muchia cu ponderea minima este acelasi cu nodul(cu aceeasi muchie) 
                // din graful dat ca parametru pe care eu il parcurg

                // mstSet[edge.destination]==false: Verifică dacă destinația muchiei edge nu este încă în MST
                // edge.weight < key[edge.destination]: Verifică dacă greutatea muchiei edge este mai mică decât greutatea 
                // curentă stocată în key pentru nodul de destinație
                {
                    parent[edge.destination] = u;
                    key[edge.destination] = edge.weight;
                } 
                else if (edge.destination == u && mstSet[edge.source]==false && edge.weight < key[edge.source]) 
                {
                    parent[edge.source] = u;
                    key[edge.source] = edge.weight;
                }
            }
        }
    }
    printMST(parent, graph->edges, n);
}

// Functia principala
int main(void) 
{
    int i;
    int numVertices = 5;
    int numEdges = 7;

    Graph *graph = createGraph(numVertices, numEdges);

    int edges[7][3] = 
    {
        {0, 1, 5},
        {1, 4, 10},
        {1, 2, 2},
        {2, 4, 3},
        {2, 3, 9},
        {3, 4, 11},
        {4, 0, 7}
    };

    for (i = 0; i < numEdges; i++) 
    {
        addEdge(graph, i, edges[i][0], edges[i][1], edges[i][2]);
    }

    primMST(graph, numVertices);
    free(graph->edges);
    free(graph);
    return 0;
}


/*
  Exemplu pas cu pas:
Inițializare:

Să presupunem că avem un graf cu nodurile 0, 1, 2, 3 și 4.
Alegem nodul 0 ca nod de start.
Inițial, setăm key[0] = 0 și toate celelalte key la infinit (∞).
Inițializăm mstSet (care indică dacă un nod este în MST) cu toate valorile false.

   Primul pas:
u este nodul 0 deoarece key[0] este 0 (cea mai mică valoare).
Adăugăm nodul 0 în mstSet.
Parcurgem toate muchiile care pornesc de la nodul 0 și actualizăm key și parent pentru nodurile vecine dacă găsim o 
greutate mai mică decât valoarea curentă din key.

   Al doilea pas:
Acum, căutăm nodul cu cea mai mică valoare în key care nu este încă în mstSet.
Să zicem că key[1] = 2, key[2] = 3, key[3] = 1, și key[4] = ∞, u va fi nodul 3 deoarece key[3] are cea mai mică valoare.
Adăugăm nodul 3 în mstSet și actualizăm key și parent pentru vecinii lui 3.

   Continuăm până la completarea MST:
Repetăm procesul, selectând mereu nodul cu cea mai mică valoare din key care nu este încă în mstSet, actualizând key și parent pentru 
vecinii acestuia.

*/
