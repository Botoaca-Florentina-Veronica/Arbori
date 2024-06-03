/*
  Se da un arbore AVL de studenti in care sunt stocate datele acestuia (marca: char,
anul nasterii: int, admin/respins: boolean). Sa se stocheze doar studentii admisi
intr-un arbore regasire si sa se calculeze inaltimea celor 2 arbori.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structura pentru un nod din arborele AVL
typedef struct TNodAVL {
    int cheie;
    int ech;
    struct TNodAVL* stg;
    struct TNodAVL* dr;
} TNodAVL;

// Structura pentru un student
typedef struct {
    char marca[10];
    int anul_nasterii;
    bool admis;
} Student;

#define TRUE 1
#define FALSE 0
#define MAX(a,b) ((a) > (b) ? (a) : (b))

// Funcția insertEchilibrat de pe cv
TNodAVL* insertEchilibrat(int x, TNodAVL *p, int *h) 
{
    TNodAVL *p1, *p2;

    if(!p) 
    {
        p = malloc(sizeof(TNodAVL));
        p->cheie = x;
        p->ech = 0;
        p->stg = p->dr = NULL;
        *h = TRUE;
        return p;
    }

    if (x < p->cheie) {
        p->stg = insertEchilibrat(x, p->stg, h);
        if (*h == TRUE) {
            switch (p->ech) {
                case 1:
                    p->ech = 0;
                    *h = FALSE;
                    break;
                case 0:
                    p->ech = -1;
                    break;
                case -1:
                    p1 = p->stg;
                    if (p1->ech == -1) {
                        p->stg = p1->dr;
                        p1->dr = p;
                        p->ech = 0;
                        p = p1;
                    } else {
                        p2 = p1->dr;
                        p1->dr = p2->stg;
                        p2->stg = p1;
                        p->stg = p2->dr;
                        p2->dr = p;

                        if (p2->ech == -1) {
                            p->ech = +1;
                        } else {
                            p->ech = 0;
                        }

                        if (p2->ech == +1) {
                            p1->ech = -1;
                        } else {
                            p1->ech = 0;
                        }

                        p = p2;
                    }
                    p->ech = 0;
                    *h = FALSE;
                    break;
            }
        }
    } else if (x > p->cheie) {
        p->dr = insertEchilibrat(x, p->dr, h);
        if (*h) {
            switch (p->ech) {
                case -1:
                    p->ech = 0;
                    *h = FALSE;
                    break;
                case 0:
                    p->ech = +1;
                    break;
                case +1:
                    p1 = p->dr;
                    if (p1->ech == 1) {
                        p->dr = p1->stg;
                        p1->stg = p;
                        p->ech = 0;
                        p = p1;
                    } else {
                        p2 = p1->stg;
                        p1->stg = p2->dr;
                        p2->dr = p1;
                        p->dr = p2->stg;
                        p2->stg = p;

                        if (p2->ech == 1) {
                            p->ech = -1;
                        } else {
                            p->ech = 0;
                        }

                        if (p2->ech == -1) {
                            p1->ech = +1;
                        } else {
                            p1->ech = 0;
                        }

                        p = p2;
                    }
                    p->ech = 0;
                    *h = FALSE;
            }
        }
    }
    return p;
}

// Funcția pentru a adăuga un nod într-un arbore AVL
TNodAVL* adauga(TNodAVL *r, int k) 
{
    int h = FALSE;
    return insertEchilibrat(k, r, &h);
}

// Funcția pentru a calcula înălțimea unui arbore AVL
int inaltime(TNodAVL *r) 
{
    if (!r)
    {
        return 0;
    }
    return MAX(inaltime(r->stg), inaltime(r->dr)) + 1;
}

// Funcția pentru a distruge un arbore AVL
void distruge(TNodAVL *r) 
{
    if(!r)
    {
        return;
    }
    distruge(r->stg);
    distruge(r->dr);
    free(r);
}

// Funcția pentru a adăuga studenți admiși într-un arbore AVL de regăsire
TNodAVL* adaugaStudentiAdmisi(Student* studenti, int nrStudenti, TNodAVL* arboreRegasire) 
{
    int i;
    for (i = 0; i < nrStudenti; i++) 
    {
        if (studenti[i].admis) 
        {
            arboreRegasire = adauga(arboreRegasire, studenti[i].anul_nasterii);
        }
    }
    return arboreRegasire;
}

int main(void) 
{
    int i;
    Student studenti[] = 
    {
        {"A1", 1995, true},
        {"A2", 1996, false},
        {"A3", 1994, true},
        {"A4", 1993, false},
        {"A5", 1992, true}
    };

    int nrStudenti = sizeof(studenti) / sizeof(studenti[0]);
    TNodAVL* arboreStudenti = NULL;
    TNodAVL* arboreRegasire = NULL;

    for (i = 0; i < nrStudenti; i++) 
    {
        arboreStudenti = adauga(arboreStudenti, studenti[i].anul_nasterii);
    }

    arboreRegasire = adaugaStudentiAdmisi(studenti, nrStudenti, arboreRegasire);

    printf("Inaltimea arborelui initial: %d\n", inaltime(arboreStudenti));
    printf("Inaltimea arborelui de regasire: %d\n", inaltime(arboreRegasire));

    distruge(arboreStudenti);
    distruge(arboreRegasire);

    return 0;
}

/*

Arbore initial:
        1995
       /    \
    1993     1996
   /   \
1992   1994
Inaltime:3
---------------------

Arbore de regasire:
    1994
   /    \
1992    1995
Inaltime:2
*/
