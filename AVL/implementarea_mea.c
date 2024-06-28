// implementare arbore AVL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structura pentru un nod din arborele AVL
typedef struct nodAVL
{
    int cheie;
    int ech;
    struct nodAVL *stg;
    struct nodAVL *dr;
} nodAVL;

#define TRUE 1
#define FALSE 0
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Funcția pentru a distruge un arbore AVL
void DistrugeAVL(nodAVL *r)
{
    if (!r)
    {
        return;
    }
    DistrugeAVL(r->stg);
    DistrugeAVL(r->dr);
    free(r);
}

// Funcția insertEchilibrat de pe cv
nodAVL *InsertEchilibrat(int x, nodAVL *p, int *h)
{
    nodAVL *p1, *p2;

    if (!p) // cheia nu e in arbore; se insereaza
    {
        p = (nodAVL *)malloc(sizeof(nodAVL));
        p->cheie = x;
        p->ech = 0;            // nodul nou inserat creaza un subarbore unitar, care este mereu echilibrat
        p->stg = p->dr = NULL; // noul nod este frunza
        *h = TRUE;             // prin inserarea noului nod intr-un arbore gol, am modificat implicit si inaltimea
        return p;
    }

    if (x < p->cheie)
    {
        // daca cheia de inserat < cheia curenta,
        // se insereaza in subarborele stang

        /*
            ech = 0: Arorele este perfect echilibrat.
            ech = 1: Subarborele drept este cu o unitate mai înalt decât subarborele stâng.
            ech = -1: Subarborele stâng este cu o unitate mai înalt decât subarborele drept.
        */
        p->stg = InsertEchilibrat(x, p->stg, h);
        if (*h == TRUE) // ramura stanga a crescut in inaltime
        {
            // adica daca inaltimea in urma inserarii cheii s-a schimbat (*h==true)
            // vom recurge la urmatoarele situatii:
            switch (p->ech) // vom schimba 'gradul' specific echilibrarii arborelui
            {
            case 1:         // subarborele era dezechilibrat in dreapta
                p->ech = 0; // adaugare in subarborele stang => echilibru
                *h = FALSE;
                break;
            case 0:
                // in acest caz, arborele nostru era perfect echilibrat, insa
                // in urma inserarii in partea stanga a noului nod am provocat dezechilibru
                // cu o unitate, dar in continuare, nu e nevoie de alte interventii pt reechilibrare,
                // fiindca arborele este intr-o stare optima(nu puteam insera nodul intr-un loc mai bun)
                p->ech = -1;
                // nu mai e nevoie sa scriu *h=TRUE, fiindca eu asa l-am initializat
                // il schimb doar daca nu exista schimbari de inaltime
                break;
            case -1:
                // ei bine, aici deja subarborele stang era dezechilibrat cu o unitate,
                // iar prin inserarea noului nod in stanga, dezechilibram arborele cu 2 unitati pe
                // aceasta parte, deci e necesară obligatoriu o reechilibrare!
                p1 = p->stg;
                if (p1->ech == -1) // cazul 1 stanga(LL), dezechilibru la fiu in stanga
                {

                    // ne aflăm într-un dezechilibru cu 2 unități, prima unitate nu deranjează(ca și în cazul -1
                    // de mai sus ce nu necesită reechilibrare)
                    // noi ne ghidăm după ce-a de a doua, deci vom avea cazul în care nodul care provoacă
                    // dezechilibrul este frunza fie in dreapa fie in stanga subarborelui stang deja dezechilibrat
                    // acesta este cazul 1 stanga(LL), dezechilibru la fiu in stanga

                    /*Se efectueaza o rotatie la dreapta a subarborelui p1, astfel:
                       - fiul drept al nodului p1 este "decuplat" de arbore;
                       - nodul p devine fiul drept al nodului p1;
                       - nodul "decuplat" devine fiul stang al nodului p.
                    */
                    p->stg = p1->dr;
                    p1->dr = p;
                    p->ech = 0;
                    p = p1;
                }
                else // cazul 2 stanga(LR), dezechilibru la fiu in dreapta
                {
                    /*Se efectueaza o rotatie la stanga a subarborelui p2, astfel:
                       - fiul stanga al nodului p2 este "decuplat" de arbore;
                       - nodul p2->stg devine fiul drept al nodului p1;
                       - nodul p1 devine fiul stang al nodului "decuplat";
                       - nodul "decuplat" p2 devine fiul stang al nodului p.

                       Se efectueaza o rotatie la dreapta a subarborelui p2, astfel:
                       - fiul stanga al nodului p (p2) este "decuplat" de arbore;
                       - nodul p2->dr devine fiul stang al nodului p;
                       - nodul p devine fiul drept al nodului "decuplat" p2.
                    */
                    p2 = p1->dr;
                    p1->dr = p2->stg;
                    p2->stg = p1;
                    p->stg = p2->dr;
                    p2->dr = p;

                    // acum recalculam gradele pentru evaluarea echilibrului arborelui
                    if (p2->ech == -1)
                        p->ech = +1;
                    else
                        p->ech = 0;

                    if (p2->ech == +1)
                        p1->ech = -1;
                    else
                        p1->ech = 0;

                    p = p2;
                } // caz 2 stanga
                p->ech = 0; // în urma pașilor de mai sus am reechilibrat arborele
                *h = FALSE; // nu s-a schimbat diferenta de nivel
                break;
            } // switch
        }
    }
    else if (x > p->cheie)
    {
        p->dr = InsertEchilibrat(x, (nodAVL *)p->dr, h);
        if (*h) // ramura dreapta a crescut in inaltime
            switch (p->ech)
            {
            case -1:        // subarborele era dezechilibrat in stanga
                p->ech = 0; // adaugare in subarborele drept => echilibru
                *h = FALSE;
                break;
            case 0:          // subarborele era in echilibru
                p->ech = +1; // dezechilibrat cu un nivel in stanga
                break;
            case +1: // subarborele era dezechilibrat in dreapta cu un nivel,
                     // acum e cu doua nivele => reechilibrare
                p1 = p->dr;
                if (p1->ech == +1) // cazul 1 dreapta(RR)
                {
                    /*Se efectueaza o rotatie la stanga a subarborelui p1, astfel:
                       - nodul p1->stg este "decuplat" de arbore;
                       - nodul p devine fiul stang al nodului p1;
                       - nodul "decuplat" devine fiul drept al nodului p.
                    */
                    p->dr = p1->stg;
                    p1->stg = p;
                    p->ech = 0;
                    p = p1;
                }
                else // cazul 2 dreapta(RL)
                {
                    /*Se efectueaza o rotatie la dreapta a subarborelui p2, astfel:
                       - nodul p2->dr este "decuplat" de arbore;
                       - nodul p1 devine fiul drept al nodului p2;
                       - nodul p2 devine fiul drept al nodului p
                       - nodul "decuplat" devine fiul stang al nodului p1.

                      Se efectueaza o rotatie la stanga a subarborelui p2, astfel:
                       - nodul p2->stg este "decuplat" de arbore;
                       - nodul p devine fiul stang al nodului p2;
                       - nodul "decuplat" devine fiul drept al nodului p.
                    */
                    p2 = p1->stg;
                    p1->stg = p2->dr;
                    p2->dr = p1;
                    p->dr = p2->stg;
                    p2->stg = p;

                    if (p2->ech == 1)
                        p->ech = -1;
                    else
                        p->ech = 0;

                    if (p2->ech == -1)
                        p1->ech = +1;
                    else
                        p1->ech = 0;

                    p = p2;
                } // caz 2 dreapta
                p->ech = 0;
                *h = FALSE;
            } // switch
    }
    return p;
}

// Funcția pentru adăugarea unui nod în arborele AVL
nodAVL *AdaugaAVL(nodAVL *r, int k)
{
    int h = FALSE;
    return InsertEchilibrat(k, r, &h);
}

// Funcția pentru calculul înălțimii unui arbore AVL
int InaltimeAVL(nodAVL *r)
{
    if (!r)
    {
        return 0;
    }
    return (MAX(InaltimeAVL(r->stg), InaltimeAVL(r->dr))) + 1;
}

// Functie pentru a afisa arborele (traversare inordine)
void InOrder(nodAVL *root)
{
    if (root)
    {
        InOrder(root->stg);
        printf("%d ", root->cheie);
        InOrder(root->dr);
    }
}

int main(void)
{
    nodAVL *root = NULL;
    int h;
    int chei[] = {20, 4, 26, 3, 9, 15, 30};
    int n = sizeof(chei) / sizeof(chei[0]);

    for (int i = 0; i < n; i++)
    {
        h = FALSE;
        root = InsertEchilibrat(chei[i], root, &h);
    }

    printf("Traversare in ordine: ");
    InOrder(root);
    printf("\n");

    return 0;
}
