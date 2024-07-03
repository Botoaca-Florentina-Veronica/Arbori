#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Nod
{
    int cheie;
} Nod;

typedef struct Graf
{
    int nr_noduri;
    Nod *Noduri;
    int **Arce; // Matricea de adiacență
} Graf;

// ARC==MUCHIE

// functie pentru a creea un graf
Graf *creeareGraf(int nr_noduri)
{
    Graf *grafNou;
    grafNou = (Graf *)malloc(sizeof(Graf));
    grafNou->nr_noduri = nr_noduri;
    grafNou->Noduri = (Nod *)malloc(nr_noduri * sizeof(Nod));
    grafNou->Arce = (int **)malloc(nr_noduri * sizeof(int *));

    int i, j;
    for (i = 0; i < nr_noduri; i++)
    {
        grafNou->Arce[i] = (int *)malloc(nr_noduri * sizeof(int));
        for (j = 0; j < nr_noduri; j++)
        {
            grafNou->Arce[i][j] = -1;
            // Inițializăm cu -1 pentru a indica absența unei muchii între i și j
        }
    }
    return grafNou;
}

// functie pentru a adauga o muchie in graf
void adaugaMuchie(Graf *graf, int source, int destination, int weight)
{
    graf->Arce[source][destination] = weight;
    graf->Arce[destination][source] = weight;
    // graful e neorientat, deci pot merge si de la stg->dr si de la dr->stg
}

// algoritmul lui Prim
void Prim(Graf *graf)
{
    int vizitat[graf->nr_noduri]; // în vectorul vizitat îmi rețin numarul de noduri
    int i, j, pas;

    for (i = 0; i < graf->nr_noduri; i++)
    {
        vizitat[i] = 0; // inițializăm toate nodurile din graf ca fiind nevizitate, adica egale cu 0
    }
    vizitat[0] = 1; // însă primul nod îl voi introduce mereu în arborele de acoperire minim
    // de aceea îl inițializez cu 1, ca să îl marchez ca fiind vizitat

    // parcurg graful dat ca parametru
    for (pas = 1; pas < graf->nr_noduri; pas++)
    {
        int min = 9999, mini = -1, minj = -1;
        // NEAPĂRAT, aceasta linie de cod sa fie în interiorul for-ului principal, pentru a le reseta la fiecare pas
        // și deci pentru a găsi noua muchie minimă la fiecare iterație

        for (i = 0; i < graf->nr_noduri; i++)
        {
            // verific daca nodul la care mă aflu este vizitat
            // începând chiar cu primul nod, deci nu mai e necesară ramura de else
            // fiindcă știu sigur că primul nod din graf va fi mereu vizitat
            if (vizitat[i] == 1)
            {
                for (j = 0; j < graf->nr_noduri; j++)
                {
                    // 1) Dacă nodul j este nevizitat(= 0)
                    // 2) și există muchie între j și i, unde i este deja existent în arborele de
                    // acoperire minim, în urma verificării de mai sus
                    // 3) și această muchie este minimă
                    if (vizitat[j] == 0 && graf->Arce[i][j] != -1 && graf->Arce[i][j] < min)
                    {
                        min = graf->Arce[i][j];
                        mini = i;
                        minj = j;
                    }
                }
            }
        }
        // Dacă rămân -1, înseamnă că nu s-a găsit nicio muchie validă care să fie adăugată în MST în acest pas
        //  MST-minimal spanning tree
        if (mini != -1 && minj != -1) // deci mini și minj s-au schimbat, așa că
        {
            vizitat[minj] = 1;
            // Aceasta marchează nodul minj (nodul destinație al muchiei cu cost minim) ca fiind vizitat
            // Astfel, acest nod este acum inclus în MST și va fi luat în considerare în iterațiile viitoare ale algoritmului.
            printf("(%d, %d) - (%d)\n", mini, minj, min);
        }
    }
}

// Funcția principală
int main(void)
{
    int i;
    int nr_noduri = 5;
    Graf *graf = creeareGraf(nr_noduri);

    // Inițializarea nodurilor
    for (i = 0; i < nr_noduri; i++)
    {
        graf->Noduri[i].cheie = 100; // Să zicem că fiecare nod are inițial 100 puncte
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
    Prim(graf);

    // Eliberarea memoriei
    for (i = 0; i < nr_noduri; i++)
    {
        free(graf->Arce[i]);
    }
    free(graf->Arce);
    free(graf->Noduri);
    free(graf);

    return 0;
}
