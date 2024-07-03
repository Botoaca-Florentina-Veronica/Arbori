#include <stdio.h>
#include <limits.h>

// Define the Graf structure
typedef struct
{
    int nr_noduri;      // Number of nodes in the graph
    int Arce[100][100]; // Adjacency matrix to store the weights of the edges
} Graf;

// Function to print the path from the source to a given node
void afiseazaDrum(int t[], int j)
{
    if (t[j] == -1)
    {
        return;
    }

    afiseazaDrum(t, t[j]);
    printf("%d ", j);
}

// Dijkstra algorithm implementation
void dijkstra(Graf *graf, int nod)
{
    int d[graf->nr_noduri];
    int t[graf->nr_noduri];
    int vizitat[graf->nr_noduri];
    int i, pas;

    for (i = 0; i < graf->nr_noduri; i++)
    {
        d[i] = INT_MAX;
        t[i] = -1;
        vizitat[i] = 0;
    }

    d[nod] = 0;

    for (pas = 0; pas < graf->nr_noduri; pas++)
    {
        int min = INT_MAX;
        int mini = -1;

        for (i = 0; i < graf->nr_noduri; i++)
        {
            if (!vizitat[i] && d[i] < min)
            {
                min = d[i];
                mini = i;
            }
        }

        if (mini == -1)
        {
            break;
        }

        vizitat[mini] = 1;

        for (i = 0; i < graf->nr_noduri; i++)
        {
            if (graf->Arce[mini][i] != -1 && !vizitat[i] && d[i] > d[mini] + graf->Arce[mini][i])
            {
                d[i] = d[mini] + graf->Arce[mini][i];
                t[i] = mini;
            }
        }
    }

    for (i = 0; i < graf->nr_noduri; i++)
    {
        if (d[i] != INT_MAX && nod != i)
        {
            printf("Distanta de la %d la %d este %d\n", nod, i, d[i]);
            printf("Drumul minim este: ");
            printf("%d ", nod);
            afiseazaDrum(t, i);
            printf("\n");
        }
    }
}

// Function to print a matrix
void printeazaMatrice(int matrix[][3])
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(void)
{
    // Example graph with 3 nodes
    Graf graf;
    graf.nr_noduri = 3;
    int i, j;

    // Initialize the adjacency matrix with -1 indicating no direct path
    int Arce[3][3] = {
        {0, 1, 4},
        {1, 0, 2},
        {4, 2, 0}};

    // Copy the adjacency matrix to the graph structure
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            graf.Arce[i][j] = Arce[i][j];
        }
    }

    // Run Dijkstra's algorithm from node 0
    dijkstra(&graf, 0);

    // Print the adjacency matrix
    printf("Matricea de adiacenta este:\n");
    printeazaMatrice(Arce);

    return 0;
}
