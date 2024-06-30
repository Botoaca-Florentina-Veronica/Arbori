#include <stdio.h>
#include <stdlib.h>

// Structura pentru reprezentarea muchiilor
typedef struct Edge
{
    int source;
    int destination;
    int weight;
} Edge;

// Structura pentru reprezentarea unui graf
typedef struct Graph
{
    int numVertices;
    int numEdges;
    Edge *edges;
} Graph;

// Structura pentru reprezentarea unui subset pentru union-find
typedef struct Subset
{
    int parent;
    int rank;
} Subset;

// Functie pentru a crea un graf
Graph *createGraph(int numVertices, int numEdges)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->numEdges = numEdges;
    graph->edges = (Edge *)malloc(numEdges * sizeof(Edge));
    return graph;
}

// Functie pentru a adauga o muchie in graf
void addEdge(Graph *graph, int edgeIndex, int source, int destination, int weight)
{
    graph->edges[edgeIndex].source = source;
    graph->edges[edgeIndex].destination = destination;
    graph->edges[edgeIndex].weight = weight;
}

// Find-Union este o structură de date utilizată pentru a gestiona și uni seturi disjuncte (neintersectate), și este optimizată pentru a
// suporta rapid operațiile de unire și de găsire a reprezentantului unui set.

// Funcția find este utilizată pentru a găsi reprezentantul (sau rădăcina) setului căruia îi aparține un anumit element
// Aceasta ajută la verificarea dacă două elemente sunt în același set
int find(Subset subsets[], int i)
{
    // Dacă acest element (i) este propriul său părinte, atunci este reprezentantul setului
    if (subsets[i].parent != i)
    {
        // Dacă nu este, atunci caut recursiv părintele
        subsets[i].parent = find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

// Funcția union este utilizată pentru a uni două seturi. Aceasta funcție utilizează o tehnică numită "uniunea după rang"
// pentru a menține arborele cât mai plat, ceea ce îmbunătățește performanța operațiilor de find
void Union(Subset subsets[], int x, int y)
{
    int rootX = find(subsets, x);
    int rootY = find(subsets, y);

    // Atașează arborele cu rang mai mic sub rădăcina arborelui cu rang mai mare
    if (subsets[rootX].rank < subsets[rootY].rank)
    {
        subsets[rootX].parent = rootY;
    }
    else if (subsets[rootX].rank > subsets[rootY].rank)
    {
        subsets[rootY].parent = rootX;
    }
    else
    {
        subsets[rootY].parent = rootX;
        subsets[rootX].rank++;
    }
}

// Functie de comparare pentru sortarea muchiilor dupa greutate
int compareEdges(const void *a, const void *b)
{
    Edge *edgeA = (Edge *)a;
    Edge *edgeB = (Edge *)b;
    return edgeA->weight - edgeB->weight;
}

// Functie pentru a afisa arborele partial de cost minim
void printMST(Edge result[], int numEdges)
{
    printf("Muchiile arborelui partial de cost minim:\n");
    for (int i = 0; i < numEdges; i++)
    {
        printf("%d - %d: %d\n", result[i].source, result[i].destination, result[i].weight);
    }
}

// Functie pentru a calcula si a afisa arborele partial de cost minim folosind algoritmul lui Kruskal
void kruskalMST(Graph *graph)
{
    int numVertices = graph->numVertices;
    Edge result[numVertices]; // va stoca muchiile din arborele partial de cost minim
    int e = 0;                // numar de muchii in result
    int i = 0;                // index pentru muchiile sortate

    // Sortam toate muchiile dupa greutate
    qsort(graph->edges, graph->numEdges, sizeof(Edge), compareEdges);

    // Alocam memorie pentru numVertices subseturi
    Subset *subsets = (Subset *)malloc(numVertices * sizeof(Subset));
    for (int v = 0; v < numVertices; v++)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < numVertices - 1 && i < graph->numEdges)
    {
        // Alege cea mai mica muchie. Daca o includem in rezultat, nu va forma un ciclu
        Edge nextEdge = graph->edges[i++];

        int x = find(subsets, nextEdge.source);
        int y = find(subsets, nextEdge.destination);

        if (x != y)
        {
            result[e++] = nextEdge;
            Union(subsets, x, y);
        }
    }

    printMST(result, e);
    free(subsets);
}

// Functia principala
int main(void)
{
    int i;
    int numVertices = 5;
    int numEdges = 7;

    Graph *graph = createGraph(numVertices, numEdges);

    int edges[7][3] = {
        {0, 1, 5},
        {1, 4, 10},
        {1, 2, 2},
        {2, 4, 3},
        {2, 3, 9},
        {3, 4, 11},
        {4, 0, 7}};

    for (i = 0; i < numEdges; i++)
    {
        addEdge(graph, i, edges[i][0], edges[i][1], edges[i][2]);
    }

    kruskalMST(graph);
    free(graph->edges);
    free(graph);
    return 0;
}
