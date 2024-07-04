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
    // incep de la 1 fiindcă știu sigur că am deja un nod introdus in arborele minim de acoperire, acela fiind chiar primul nod vizitat mai sus
    for (pas = 1; pas < graf->nr_noduri; pas++)
    {
        int min = 9999, mini = -1, minj = -1;
        // NEAPĂRAT, aceasta linie de cod sa fie în interiorul for-ului principal, pentru a le reseta la fiecare pas
        // și deci pentru a găsi o (posibil) nouă muchie minimă la fiecare iterație

        for (i = 0; i < graf->nr_noduri; i++)
        {
            // verific daca nodul i la care mă aflu este vizitat
            if (vizitat[i] == 1)
            {
                for (j = 0; j < graf->nr_noduri; j++)
                {
                    // 1) Dacă nodul j este nevizitat(= 0)
                    // 2) și există muchie între j și i, unde i este deja existent în arborele de
                    // acoperire minim, în urma verificării de mai sus
                    // 3) și această muchie i-j este minimă
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
            // dacă s-au schimbat în graf nodurile mini si minj, înseamnă că am reușit să găsesc cel mai scurt drum de la i la j
            vizitat[minj] = 1; // așa că, știind că primul nod din graf (mini) este sigur introdus în arborele de acoperire minim,
            // și am găsit mai sus o muchie optimă până la nodul minj, voi marca nodul minj(nodul destinație al muchiei cu cost minim) 
            //ca fiind vizitat și deci, introdus în arbore
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
