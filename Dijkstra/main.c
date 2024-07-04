// algoritm Dijkstra

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nod
{
    int cheie;
} Nod;

typedef struct Graf
{
    int nr_noduri;
    int **Arce;
    Nod *noduri;
} Graf;

// functie de creeare a unui Graf
Graf *creeareGraf(int nr_noduri)
{
    Graf *grafNou;
    grafNou = (Graf *)malloc(sizeof(Graf));
    if (grafNou == NULL)
    {
        perror("Eroare la alocarea dinamica!!");
        exit(EXIT_FAILURE);
    }
    grafNou->nr_noduri = nr_noduri;
    grafNou->noduri = (Nod *)malloc(nr_noduri * sizeof(Nod));
    grafNou->Arce = (int **)malloc(nr_noduri * sizeof(int *));

    int i, j;
    for (i = 0; i < nr_noduri; i++)
    {
        grafNou->Arce[i] = (int *)malloc(nr_noduri * sizeof(int));
        for (j = 0; j < nr_noduri; j++)
        {
            grafNou->Arce[i][j] = -1; // imi marchez că nu există muchii în graf la început
        }
    }
    return grafNou;
}

// functie pentru adaugarea unei muchii ponderate într-un graf orientat
void adaugareMuchie(Graf *graf, int source, int destination, int weight)
{
    graf->Arce[source][destination] = weight;
}

// functie pentru a afisa drumul de la un nod sursa, la un alt nod dat
void afiseazaDrum(int t[], int j)
{
    if (t[j] == -1)
    {
        return;
    }
    afiseazaDrum(t, t[j]);
    printf("%d ", j);
}

// implementarea algoritmului Dijkstra
void Dijkstra(Graf *graf, int nod) // îmi dau nodul de la care vreau sa încep parcurgerea ca parametru
{
    int d[graf->nr_noduri];
    // d vine de la distante, in el stocăm distanțele minime de la nodul sursă la fiecare nod din graf
    int t[graf->nr_noduri];
    // t vine de la "predecesori" (sau "trasare"). Aceasta este folosită pentru a reconstrui cel mai scurt drum de la nodul sursă la orice alt nod
    int vizitat[graf->nr_noduri]; // vector de noduri vizitați
    int i, pas;

    for (i = 0; i < graf->nr_noduri; i++)
    {
        d[i] = INT_MAX; // inițializăm toate distanțele cu INT_MAX, însemnând că toate distanțele sunt 'infinite'
        // cu excepția nodului sursă care este setat la 0 (d[nod] = 0)
        // pe măsură ce algoritmul progresează, d se actualizează cu cea mai scurtă distanță cunoscută către fiecare nod
        // când algoritmul se încheie, d[i] conține cea mai scurtă distanță de la nodul sursă la nodul i
        t[i] = -1; // inițial, toate elementele din t sunt setate la -1, indicând că niciun nod nu are predecesori încă
        // Pe măsură ce algoritmul găsește drumuri mai scurte către noduri, actualizează t cu indexul nodului care precede nodul curent,
        // cel precedent având drumul cel mai scurt
        // Astfel, t[i] conține indexul nodului care vine chiar înainte de nodul i pe drumul cel mai scurt de la sursă
        vizitat[i] = -1;
    }

    d[nod] = 0;

    for (pas = 0; pas < graf->nr_noduri; pas++)
    {
        int min, mini;
        min = 999;
        mini = -1;
        // mini reprezintă nodul curent pentru care s-a determinat că are cea mai mică distanță (dintre toate nodurile nevizitate) de la nodul sursă

        for (i = 0; i < graf->nr_noduri; i++)
        {
            // daca nodul curent nu a fost vizitat și distanța de la indexul curent e minimă, atunci am gasit muchia minimă căutată
            if (vizitat[i] == -1 && d[i] < min)
            {
                min = d[i];
                mini = i;
            }
        }
        if (mini == -1)
        {
            // dacă nodul mini e neschimbat, înseamnă că nu există altă muchie mai mică
            // deci ies din buclă
            break;
        }

        // îmi marchez nodul mini ca fiind vizitat, unde mini este indexul muchiei minime
        // găsite mai sus
        vizitat[mini] = 1;

        //  De exemplu, am un graf  cu nodurile A, B, C care începe din nodul A. Din A pot ajunge la C,
        // iar din C pot ajunge la B. Drumul dintre A și B e mai mare decăt cel care trece prin A->C->B. Algoritmul va verifica întâi
        // ce legaturi există între A si alte noduri, și îl va găsi pe B și C. Reținem ca d[mini] între A și oricare alt nod e C
        // deci îl inserăm pe C în lista de noduri vizitate. Apoi măresc pasul în listă și verific ce legături există între C și alte noduri,
        // astfel îl găsesc pe B. Parcurg la fel ca înainte să văd unde găsesc muchia minimă dintre C și alt nod și descopăr
        // legătură doar între C și B deci o rețin.
        // Avănd până acum drumul minim dintre A și C, adica d[mini] și
        /// drumul minim între C și B, adică graf->Arce[mini][i], le adun și dacă rezultatul e mai mic decăt
        // orice altă astfel de combinație între alte drumuri posibile(pe care le voi efectua ulterior), o rețin în variabila min

        /*
        Nodurile: A, B, C.
        Legături: A -> C, C -> B, A -> B.
        Se inițializează o coadă de priorități (sau un set de noduri nevizitate) și se adaugă nodul A în aceasta
        Costurile: cost(A,C), cost(C,B), cost(A,B).
        Algoritmul începe cu nodul sursă A:
        Pasul 1: Se verifică vecinii nodului A, adică B și C. Se inițializează distanțele: d[A]=0, d[B]=cost(A,B), d[C]=cost(A,C).
        Pasul 2: Se alege nodul cu distanța minimă din coada de priorități, să zicem că C are distanța mai mică decât B.
        Pasul 3: Se verifică vecinii nodului C. Singurul vecin este B. Se calculează noua distanță de la A la B prin C:
        d[C]+cost(C,B). Dacă aceasta este mai mică decât distanța curentă d[B], se actualizează d[B], iar t[B]=C.
        Pasul 4: Se continuă până când toate nodurile au fost procesate.

*/
        for (i = 0; i < graf->nr_noduri; i++)
        {
            /*
            1) graf->Arce[mini][i] != -1: Verifică dacă există o cale directă (arc) de la nodul mini la nodul i. Dacă Arce[mini][i] este -1, înseamnă că nu există o astfel de cale
            2) vizitat[i] == -1: Verifică dacă nodul i nu a fost încă vizitat. Acest lucru asigură că nu se vor actualiza distanțele pentru nodurile deja procesate
            3) d[mini] + graf->Arce[mini][i] < d[i]: Verifică dacă noua distanță calculată de la sursă la i prin mini(nou apărut acum în schema de calcul a distanței, deci poate schimba daca avem un drum mai optim sau nu)
            este mai mică decât distanța curent cunoscută. Dacă este adevărat, înseamnă că s-a găsit un drum mai scurt către i.
            */
            if (graf->Arce[mini][i] != -1 && vizitat[i] == -1 && d[mini] + graf->Arce[mini][i] < d[i])
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

int main(void)
{
    int N = 5;
    Graf *graf;
    graf = creeareGraf(N);
    adaugareMuchie(graf, 0, 2, 5);
    adaugareMuchie(graf, 0, 3, 2);
    adaugareMuchie(graf, 3, 2, 2);

    Dijkstra(graf, 0);
    return 0;
}
