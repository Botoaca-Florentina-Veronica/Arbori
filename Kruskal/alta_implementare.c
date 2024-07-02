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

// Funcția pentru algoritmul lui Kruskal
void kruskal(Graf *graf)
{
    int set[graf->nr_noduri];
    int inclus[graf->nr_noduri][graf->nr_noduri]; // Inițializăm vectorul de muchii incluse
    for (int i = 0; i < graf->nr_noduri; i++)
    {
        for (int j = 0; j < graf->nr_noduri; j++)
        {
            inclus[i][j] = 0;
        }
    }
    // Inițializăm seturile disjuncte
    for (int i = 0; i < graf->nr_noduri; i++)
    {
        set[i] = i;
    }
    for (int pas = 0; pas < graf->nr_noduri - 1; pas++)
    {
        int min = 9999, mini = -1, minj = -1;
        // Găsim muchia minimă care nu formează un ciclu și nu a fost inclusă
        for (int i = 0; i < graf->nr_noduri; i++)
        {
            for (int j = 0; j < graf->nr_noduri; j++)
            {
                if (graf->Arce[i][j] != -1 && set[i] != set[j] && graf->Arce[i][j] < min && !inclus[i][j])
                {
                    min = graf->Arce[i][j];
                    mini = i;
                    minj = j;
                }
            }
        }
        // Verificăm dacă am găsit o muchie validă
        if (mini != -1 && minj != -1)
        {
            // Actualizăm seturile pentru a reflecta adăugarea muchiei
            int oldSet = set[minj], newSet = set[mini];
            for (int i = 0; i < graf->nr_noduri; i++)
            {
                if (set[i] == oldSet)
                {
                    set[i] = newSet;
                }
            }
            // Afișăm muchia adăugată în arbore
            printf("(%d, %d) - (%d)\n", mini, minj, min);
            // Marcăm muchia ca fiind inclusă
            inclus[mini][minj] = inclus[minj][mini] = 1;
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

    // Apelarea algoritmului lui Kruskal
    kruskal(graf);

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
