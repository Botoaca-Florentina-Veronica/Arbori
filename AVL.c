#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct nodAVL
{
    int cheie;
    int ech;
    struct nodAVL *stg;
    struct nodAVL *dr;
} nodAVL;

// functie de echilibrare arbore avl
nodAVL *InsertEchilibrat(int cheie, nodAVL *p, int *h)
{
    nodAVL *p1, *p2;
    //*h = TRUE;

    if (!p)
    {
        // daca cheia nu e in arbore, o inseram
        p = (nodAVL *)malloc(sizeof(nodAVL)); // alocam memoria necesara
        p->cheie = cheie;
        p->ech = 0; // nodul nou inserat creeaza un subarbore unitar, care e mereu echilibrat
        p->stg = NULL;
        p->dr = NULL;
        *h = TRUE; // prin inserarea noului nod intr-un arbore gol, am modificat implicit si inaltimea
        return p;
    }

    // daca cheia pe care vrem sa o inseram in arbore, este mai
    // mica decat cea curenta din arborele parcurs, atunci o vom insera
    // in partea stanga

    /* ATENTIE:

    ech = 0: arborele este perfect echilibrat
    ech = 1: subarborele drept este cu o unitate mai inalt decat subarborele stang
    ech = -1: subarborele stang este cu o unitate mai inalt decat subarborele drept
    */
    if (cheie < p->cheie)
    {
        p->stg = InsertEchilibrat(cheie, (nodAVL *)p->stg, h);
        if (*h == TRUE)
        {
            // adica daca inaltimea in urma inserarii cheii s-a schimbat
            // vom recurge la urmatoarele situatii:
            switch (p->ech)
            {
            // vom schimba 'gradul' specific echilibrarii arborelui
            case 1:
                // in acest caz, in urma inserarii nodului in arbore,
                // nu am provocat schimbari, deci subarborele nostru drept era dezechilibrat
                // cu o unitate, iar noi l-am reechilibrat prin inserarea la stanga
                p->ech = 0;
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
                // iar prin inserarea nodului in stanga, dezechilibram arborele cu 2 unitati pe
                // aceasta parte, deci e necesară obligatoriu o reechilibrare!
                p1 = p->stg;
                if (p1->ech == -1)
                {
                    // ne aflăm într-un dezechilibru cu 2 unități, prima unitate nu deranjează(ca și în cazul 1 de mai sus
                    // ce nu necesită reechilibrare)
                    // noi ne ghidăm după ce-a de a doua, deci vom avea cazul în care nodul care provoacă
                    // dezechilibrul este fie in dreapa fie in stanga subarborelui stang deja dezechilibrat
                    // acesta este cazul 1 stanga(LL), dezechilibru la fiu in stanga
                    p->stg = p1->dr;
                    p1->dr = p;
                    p->ech = 0;
                    p = p1;
                }
                else // cazul 2 stangaa(LR), dezechilibru la fiu în dreapta
                {
                    p2 = p1->dr;
                    p1->dr = p2->stg;
                    p2->stg = p1;
                    p->stg = p2->dr;
                    p2->dr = p;

                    // acum recalculam gradele pentru evaluarea echilibrului arborelui
                    if (p2->ech == -1)
                    {
                        p->ech = +1;
                    }
                    else
                    {
                        p->ech = 0;
                    }

                    if (p2->ech == +1)
                    {
                        p1->ech = -1;
                    }
                    else
                    {
                        p1->ech = 0;
                    }

                    p = p2;
                } // caz 2 stanga
                p->ech = 0; // în urma pașilor de mai sus am reechilibrat arborele
                *h = FALSE; // nu s-a schimbat diferenta de nivel
                break;
            }
        }
    }
    else
    {
        if (cheie > p->cheie)
        {
            p->dr = InsertEchilibrat(cheie, (nodAVL *)p->dr, h);
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
    }
    return p;
}

void preOrder(nodAVL *root)
{
    if (root != NULL)
    {
        printf("%d ", root->cheie);
        preOrder(root->stg);
        preOrder(root->dr);
    }
}

int main(void)
{
    int i;
    nodAVL *root = NULL;
    int h = FALSE;
    int keys[] = {10, 20, 5, 6, 15, 30, 25, 35, 40};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (i = 0; i < n; i++)
    {
        root = InsertEchilibrat(keys[i], root, &h);
    }

    printf("Preorder traversal of the constructed AVL tree is: ");
    preOrder(root);

    return 0;
}
