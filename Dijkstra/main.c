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

    /*
    Un exemplu simplificat:
      Să presupunem că ai un graf cu nodurile A, B și C, unde A este nodul sursă.
    Dacă drumul cel mai scurt de la A la C trece prin B, atunci d[C] va conține distanța de la A la C, iar t[C] va fi setat la indexul lui B.
    Pentru a imprima drumul cel mai scurt de la A la C, ai folosit t pentru a urmări drumul de la A la C
    */

    d[nod] = 0;

    for (pas = 0; pas < graf->nr_noduri; pas++)
    {
        int min = INT_MAX;
        int mini = -1;

        for (i = 0; i < graf->nr_noduri; i++)
        {
            // daca nodul curent nu a fost vizitat și distanța de la indexul curent e minimă, atunci am gasit muchia minimă căutată
            if (vizitat[i] == -1 && d[i] < min)
            {
                min = d[i];
                mini = i;
                // mini reprezintă nodul curent pentru care s-a determinat că are cea mai mică distanță (dintre toate nodurile nevizitate) de la nodul sursă
            }
        }

        // dacă indexul muchiei minime nu se schimbă, înseamnă că nu există altă muchie mai mică
        // deci ies din buclă
        if (mini == -1)
        {
            break;
        }

        // îmi marchez muchia cu indexul găsit mai sus, ca fiind vizitată
        vizitat[mini] = 1;

        for (i = 0; i < graf->nr_noduri; i++)
        {
            /*
            1) graf->Arce[mini][i] != -1: Verifică dacă există o cale directă (arc) de la nodul mini la nodul i. Dacă Arce[mini][i] este -1, înseamnă că nu există o astfel de cale
            2) vizitat[i] != -1: Verifică dacă nodul i nu a fost încă vizitat. Acest lucru asigură că nu se vor actualiza distanțele pentru nodurile deja procesate
            3) d[mini] + graf->Arce[mini][i] < d[i]: Verifică dacă noua distanță calculată de la sursă la i prin mini(nou apărut acum în schema de calcul a distanței, deci poate schimba daca avem un drum mai optim sau nu)
            este mai mică decât distanța curent cunoscută. Dacă este adevărat, înseamnă că s-a găsit un drum mai scurt către i.
            */
            if (graf->Arce[mini][i] != -1 && vizitat[i] != -1 && d[mini] + graf->Arce[mini][i] < d[i])
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
    int Arce[3][3] =
        {
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
