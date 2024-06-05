/*  Se da harta unei regiuni, reprezentata prin matricea de adiacenta a unui graf neorientat, ponderat, nodurile reprezentand orasele din regiune
si ponderile reprezentand distantele dintre orase 
    Sa se scrie un program care determina lungimea minima necesara de teava care trebuie folosita de o companie de apa care conecteaza toate orasele.
*/

//vom folosi algoritmul lui Prim
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX 100

// Funcția pentru a găsi vârful cu valoarea cheie minimă din setul de vârfuri care nu sunt incluse în MST
// (MST- minimal spanning tree)
int minKey(int key[], bool mstSet[], int n) 
{
    int v;
    int min = INT_MAX;
    int min_index;

    for (v = 0; v < n; v++)
    {
        if (mstSet[v] == false && key[v] < min)
        {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}

// Funcția pentru a construi și afișa MST utilizând matricea de adiacență reprezentată ca graful
int primMST(int graph[MAX][MAX], int n) 
{
    int i, u, v;
    int parent[MAX]; // Array pentru a stoca MST-ul construit
    int key[MAX]; // Valori cheie utilizate pentru a alege vârfurile minime
    bool mstSet[MAX]; // Pentru a reprezenta setul de vârfuri incluse în MST

    // Inițializăm toate cheile ca INFINIT
    for (i = 0; i < n; i++)
    {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    // Întotdeauna includem primul vârf în MST
    key[0] = 0; // Facem valoarea cheie a acestui vârf 0 pentru a-l alege primul
    parent[0] = -1; // Primul nod este întotdeauna rădăcina MST-ului

    // MST-ul va avea n vârfuri
    for (int count = 0; count < n - 1; count++) 
    {
        u = minKey(key, mstSet, n);
        mstSet[u] = true;
        //inca nu am pus drumul gasit in u in graful minim de acoperire, doar am cautat si gasit cel mai scurt drum(indexul din matricea de adiacenta)
        // si l-am retinut in u
        //urmeaza sa introduc acest drum in arborele minim aici:

        // Actualizați valoarea cheii și indexul părintelui vârfurilor adiacente vârfului ales
        for (v = 0; v < n; v++)

            // graph[u][v] este diferit de 0 doar pentru vârfurile adiacente
            // mstSet[v] este fals pentru vârfurile încă neincluse în MST
            // Actualizați cheia doar dacă graph[u][v] este mai mică decât key[v]
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
            {
                parent[v] = u;
                key[v] = graph[u][v];
            }
    }

    // Calculăm suma totală a MST-ului
    int total_weight = 0;
    for (i = 1; i < n; i++)
    {
        total_weight += graph[i][parent[i]];
    }
    return total_weight;
}

int main(void) 
{
    // Inițializăm numărul de orașe
    int n = 5;

    // Inițializăm matricea de adiacență
    int graph[MAX][MAX] = 
    {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0},
    };

    // Calculăm și afișăm lungimea totală a țeavii necesare
    int total_length = primMST(graph, n);
    printf("Lungimea minima de teava necesara pentru a conecta toate orasele este: %d\n", total_length);

    return 0;
}
