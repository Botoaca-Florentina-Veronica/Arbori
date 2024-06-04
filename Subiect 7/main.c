/*
  Se da un gra neorientat ponderat conex, cu n varfuri si m muchii-in care fiecare muchie are asociat un cost, numar natural
strict pozitiv
  Folosind algoritmul lui Prim, determinati un arbore partial de cost minim
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF 999999

// Structura pentru reprezentarea muchiilor
struct Edge {
    int source, destination, weight;
};

// Structura pentru reprezentarea grafului
struct Graph {
    int numVertices, numEdges;
    struct Edge* edge;
};

// Functie pentru initializarea grafului
struct Graph* createGraph(int numVertices, int numEdges) 
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;
    graph->numEdges = numEdges;
    graph->edge = (struct Edge*) malloc(graph->numEdges * sizeof(struct Edge));
    return graph;
}

// Functie pentru adaugarea unei muchii in graf
void addEdge(struct Graph* graph, int source, int destination, int weight) 
{
    graph->edge[source].source = source;
    graph->edge[source].destination = destination;
    graph->edge[source].weight = weight;
}

// Functie pentru a gasi varful cu cel mai mic cost
int minKey(int key[], bool mstSet[], int numVertices) 
{
    int min = INT_MAX, min_index;
    for (int v = 0; v < numVertices; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
    return min_index;
}

// Functie pentru a afisa arborele partial de cost minim
void printMST(int parent[], struct Edge graph[], int numVertices) 
{
    printf("Muchiile arborelui partial de cost minim:\n");
    for (int i = 1; i < numVertices; i++)
        printf("%d - %d\n", parent[i], i);
}

// Functie pentru a calcula si a afisa arborele partial de cost minim folosind algoritmul lui Prim
void primMST(struct Graph* graph) 
{
    int parent[MAX_VERTICES]; // Array pentru a stoca arborele partial de cost minim
    int key[MAX_VERTICES]; // Array pentru a stoca costul minim pentru fiecare varf
    bool mstSet[MAX_VERTICES]; // Array pentru a marca varfurile incluse in arborele partial de cost minim
    
    // Initializare
    for (int i = 0; i < graph->numVertices; i++)
        key[i] = INT_MAX, mstSet[i] = false;
    key[0] = 0; // Primul varf este intotdeauna radacina arborelui
    
    parent[0] = -1; // Niciun varf nu are parinte in arborele partial de cost minim
    
    // Construirea arborelui partial de cost minim
    for (int count = 0; count < graph->numVertices - 1; count++) {
        int u = minKey(key, mstSet, graph->numVertices); // Alegerea varfului cu cel mai mic cost nevizitat
        mstSet[u] = true; // Marcharea varfului ca vizitat
        
        // Actualizarea costului minim pentru varfurile adiacente varfului selectat recent
        for (int v = 0; v < graph->numVertices; v++)
            if (graph->edge[u].weight && mstSet[v] == false && graph->edge[u].weight < key[v])
                parent[v] = u, key[v] = graph->edge[u].weight;
    }
    
    // Afisarea arborelui partial de cost minim
    printMST(parent, graph->edge, graph->numVertices);
}

// Functia principala
int main(void)
{
    int i;
    int numVertices, numEdges;
    printf("Introduceti numarul de varfuri si muchii: ");
    scanf("%d %d", &numVertices, &numEdges);

    struct Graph* graph = createGraph(numVertices, numEdges);

    printf("Introduceti muchiile si costurile asociate:\n");
    for (i = 0; i < numEdges; ++i) 
    {
        int source, destination, weight;
        scanf("%d %d %d", &source, &destination, &weight);
        addEdge(graph, source, destination, weight);
    }

    primMST(graph);

    return 0;
}
