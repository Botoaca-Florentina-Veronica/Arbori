#include <stdio.h>
#include <stdlib.h>

// Structură pentru un nod din graf
typedef struct Nod
{
    int info;
} Nod;

// Structură pentru un graf
typedef struct Graf
{
    int nr_noduri;
    int **Arce; // Matricea de adiacență
    Nod *Noduri;
} Graf;

// Funcție pentru a crea un graf
Graf *creeazaGraf(int nr_noduri)
{
    Graf *graf = (Graf *)malloc(sizeof(Graf));
    graf->nr_noduri = nr_noduri;
    graf->Noduri = (Nod *)malloc(nr_noduri * sizeof(Nod));
    graf->Arce = (int **)malloc(nr_noduri * sizeof(int *));
    for (int i = 0; i < nr_noduri; i++)
    {
        graf->Arce[i] = (int *)malloc(nr_noduri * sizeof(int));
        for (int j = 0; j < nr_noduri; j++)
        {
            graf->Arce[i][j] = -1; // Inițializare cu -1 pentru a indica absența unei muchii
        }
    }
    return graf;
}

// Funcție pentru a adăuga o muchie în graf
void adaugaMuchie(Graf *graf, int src, int dest, int cost)
{
    graf->Arce[src][dest] = cost;
    graf->Arce[dest][src] = cost;
}

// Algoritmul lui Prim
void prim(Graf *graf)
{
    int vizitat[graf->nr_noduri];
    for (int i = 0; i < graf->nr_noduri; i++)
    {
        vizitat[i] = 0;
    }
    vizitat[0] = 1;
    for (int pas = 1; pas < graf->nr_noduri; pas++)
    {
        int min = 9999, mini = -1, minj = -1;
        for (int i = 0; i < graf->nr_noduri; i++)
        {
            if (vizitat[i] == 1)
            {
                for (int j = 0; j < graf->nr_noduri; j++)
                {
                    if (vizitat[j] == 0 && graf->Arce[i][j] != -1 && graf->Arce[i][j] < min && (graf->Arce[i][j] <= graf->Noduri[i].info))
                    {
                        graf->Noduri[i].info -= graf->Arce[i][j];
                        min = graf->Arce[i][j];
                        mini = i;
                        minj = j;
                    }
                    else if (vizitat[j] == 0 && graf->Arce[i][j] != -1 && graf->Arce[i][j] < min && (graf->Arce[i][j] <= graf->Noduri[j].info))
                    {
                        graf->Noduri[j].info -= graf->Arce[i][j];
                        min = graf->Arce[i][j];
                        mini = i;
                        minj = j;
                    }
                }
            }
        }
        if (mini != -1 && minj != -1)
        {
            vizitat[minj] = 1;
            printf("(%d, %d) - (%d)\n", mini, minj, min);
        }
    }
}

// Funcția principală
int main()
{
    int nr_noduri = 5;
    Graf *graf = creeazaGraf(nr_noduri);

    // Inițializarea nodurilor
    for (int i = 0; i < nr_noduri; i++)
    {
        graf->Noduri[i].info = 100; // Să zicem că fiecare nod are inițial 100 puncte
    }

    // Adăugarea muchiilor
    adaugaMuchie(graf, 0, 1, 2);
    adaugaMuchie(graf, 0, 3, 6);
    adaugaMuchie(graf, 1, 2, 3);
    adaugaMuchie(graf, 1, 3, 8);
    adaugaMuchie(graf, 1, 4, 5);
    adaugaMuchie(graf, 2, 4, 7);
    adaugaMuchie(graf, 3, 4, 9);

    // Apelarea algoritmului lui Prim
    prim(graf);

    // Eliberarea memoriei
    for (int i = 0; i < nr_noduri; i++)
    {
        free(graf->Arce[i]);
    }
    free(graf->Arce);
    free(graf->Noduri);
    free(graf);

    return 0;
}
