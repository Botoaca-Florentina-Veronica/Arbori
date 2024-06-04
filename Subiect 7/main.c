/*
  Se da un graf neorientat ponderat conex, cu n varfuri si m muchii-in care fiecare muchie are asociat un cost, numar natural
strict pozitiv
  Folosind algoritmul lui Prim, determinati un arbore partial de cost minim
*/

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
Graph* createGraph(int numVertices, int numEdges) 
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
void primMST(Graph *graph) 
{
    int i, u, e, v;
    int numVertices = graph->numVertices;
    int parent[MAX_VERTICES];
    int key[MAX_VERTICES];
    bool mstSet[MAX_VERTICES];

    for (i = 0; i < numVertices; i++) 
    {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }
    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < numVertices - 1; count++) 
    {
        u = minKey(key, mstSet, numVertices);
        mstSet[u] = true;

        for (v = 0; v < numVertices; v++) 
        {
            for (e = 0; e < graph->numEdges; e++) 
            {
                Edge edge = graph->edges[e];
                if (edge.source == u && !mstSet[edge.destination] && edge.weight < key[edge.destination]) 
                {
                    parent[edge.destination] = u;
                    key[edge.destination] = edge.weight;
                } 
                else if (edge.destination == u && !mstSet[edge.source] && edge.weight < key[edge.source]) 
                {
                    parent[edge.source] = u;
                    key[edge.source] = edge.weight;
                }
            }
        }
    }
    printMST(parent, graph->edges, numVertices);
}

// Functia principala
int main(void) 
{
    int i;
    int numVertices = 5;
    int numEdges = 7;

    Graph *graph = createGraph(numVertices, numEdges);

    int edges[][3] = 
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

    primMST(graph);
    free(graph->edges);
    free(graph);
    return 0;
}
