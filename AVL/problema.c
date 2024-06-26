/*
  Se da un arbore AVL de studenti in care sunt stocate datele acestuia (marca: char,
anul nasterii: int, admin/respins: boolean). Sa se stocheze doar studentii admisi
intr-un arbore de regasire si sa se calculeze inaltimea celor 2 arbori.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structura pentru un nod din arborele AVL
typedef struct TNodAVL {
    int cheie;
    int ech;
    struct TNodAVL* stg;
    struct TNodAVL* dr;
} TNodAVL;

// Structura pentru un student
typedef struct {
    char marca[12];
    int anul_nasterii;
    bool admis;
} Student;

// Structura pentru un nod din arborele de regăsire
typedef struct NodArboreDeRegasire {
    struct NodArboreDeRegasire* alfabet[27];
} NodArboreDeRegasire;

#define TRUE 1
#define FALSE 0
#define MAX(a,b) ((a) > (b) ? (a) : (b))

// Funcția pentru inițializarea unui nod din arborele de regăsire
void Initializare(NodArboreDeRegasire *Nod) 
{
    char c;
    for (c = 'A'; c <= '['; c++)
    {
        Nod->alfabet[c - 'A'] = NULL;
    }
}

// Funcția pentru atribuirea unui nod din arborele de regăsire
void Atribuie(NodArboreDeRegasire *Nod, char c, NodArboreDeRegasire *p) 
{
    Nod->alfabet[c - 'A'] = p;
}

// Funcția pentru obținerea valorii unui nod din arborele de regăsire
NodArboreDeRegasire *Valoare(NodArboreDeRegasire *Nod, char c) 
{
    return Nod->alfabet[c - 'A'];
}

// Funcția pentru crearea unui nod nou în arborele de regăsire
void NodNou(NodArboreDeRegasire *Nod, char c) 
{
    Nod->alfabet[c - 'A'] = (NodArboreDeRegasire *)malloc(sizeof(NodArboreDeRegasire));
    Initializare(Nod->alfabet[c - 'A']);
}

// Funcția pentru adăugarea unui cuvânt în arborele de regăsire
void Adauga(char *x, NodArboreDeRegasire *cuvinte) 
{
    unsigned i;
    NodArboreDeRegasire *t;
    t = cuvinte;
    for (i = 0; i < strlen(x); i++) 
    {
        if (Valoare(t, x[i]) == NULL)
        {
            NodNou(t, x[i]);
        }
        t = Valoare(t, x[i]);
    }
    Atribuie(t, '[', t);
}

// Funcția pentru calculul înălțimii arborelui de regăsire
void CalculInaltime(NodArboreDeRegasire *Nod, int *inaltime_max, int nivel) 
{
    bool isLeaf = true;
    for (char c = 'A'; c <= 'Z'; c++) 
    {
        if (Valoare(Nod, c)) 
        {
            isLeaf = false;
            CalculInaltime(Valoare(Nod, c), inaltime_max, nivel + 1);
        }
    }
    if (isLeaf && Valoare(Nod, '[')) 
    {
        if (nivel > *inaltime_max) 
        {
            *inaltime_max = nivel;
        }
    }
}



// Funcția pentru a distruge un arbore AVL
void DistrugeAVL(TNodAVL *r) 
{
    if(!r)
    {
        return;
    }
    DistrugeAVL(r->stg);
    DistrugeAVL(r->dr);
    free(r);
}

// Funcția insertEchilibrat de pe cv
TNodAVL* InsertEchilibrat(int x, TNodAVL *p, int *h)
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

    if (x < p->cheie)
    {
        p->stg = InsertEchilibrat(x, p->stg, h);
        if (*h == TRUE) 
        {
            switch (p->ech) 
            {
                case 1:         
                    p->ech = 0; 
                    *h = FALSE;
                    break;
                case 0:  
                    p->ech = -1; 
                    break;
                case -1: 
                    p1 = p->stg;
                    if (p1->ech == -1) //cazul 1 stanga(LL), dezechilibru la fiu in stanga
                    {
                        p->stg = p1->dr;
                        p1->dr = p;
                        p->ech = 0;
                        p = p1;
                    }
                    else // cazul 2 stanga(LR), dezechilibru la fiu in dreapta
                    {
                        p2 = p1->dr;
                        p1->dr = p2->stg;
                        p2->stg = p1;
                        p->stg = p2->dr;
                        p2->dr = p;

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
                    p->ech = 0; 
                    *h = FALSE; 
                    break;
            }// switch
        }
    }
    else
        if (x > p->cheie)
        {
            p->dr = InsertEchilibrat(x, (TNodAVL *)p->dr, h);
            if (*h) 
                switch (p->ech)
                {
                    case -1:      
                        p->ech = 0; 
                        *h = FALSE;
                        break;
                    case 0:          
                        p->ech = +1; 
                        break;
                    case +1: 
                        p1 = p->dr;
                        if (p1->ech == +1) // cazul 1 dreapta(RR)
                        {
                            p->dr = p1->stg;
                            p1->stg = p;
                            p->ech = 0;
                            p = p1;
                        }
                        else // cazul 2 dreapta(RL)
                        {
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
TNodAVL* AdaugaAVL(TNodAVL *r, int k) {
    int h = FALSE;
    return InsertEchilibrat(k, r, &h);
}

// Funcția pentru calculul înălțimii unui arbore AVL
int InaltimeAVL(TNodAVL *r) {
    if (!r)
        return 0;
    return (MAX(InaltimeAVL(r->stg), InaltimeAVL(r->dr))) + 1;
}

int main(void) {
    // Definirea și inițializarea studenților
    int i;
    Student studenti[] = 
    {
        {"A1", 1995, true},
        {"A2", 1996, false},
        {"A3", 1994, true},
        {"A4", 1993, false},
        {"A5", 1992, true}
    };

    // Calculul numărului de studenți
    int nrStudenti = sizeof(studenti) / sizeof(studenti[0]);

    // Inițializarea arborilor AVL
    TNodAVL* arboreStudenti = NULL;
    NodArboreDeRegasire* arboreRegasire = (NodArboreDeRegasire*)malloc(sizeof(NodArboreDeRegasire));
    Initializare(arboreRegasire);

    // Adăugarea tuturor studenților în arborele AVL și arborele de regăsire
    for (i = 0; i < nrStudenti; i++) 
    {
        arboreStudenti = AdaugaAVL(arboreStudenti, studenti[i].anul_nasterii);
        if (studenti[i].admis) 
        {
            Adauga(studenti[i].marca, arboreRegasire);
        }
    }

    // Afișarea înălțimii arborelui AVL
    printf("Inaltimea arborelui AVL: %d\n", InaltimeAVL(arboreStudenti));

    // Calculul și afișarea înălțimii arborelui de regăsire
    int inaltime_max = 0;
    CalculInaltime(arboreRegasire, &inaltime_max, 0);
    printf("Inaltimea arborelui de regasire: %d\n", inaltime_max);

    // Eliberarea memoriei pentru arborele AVL
    DistrugeAVL(arboreStudenti);
    
    return 0;
}

                           
/*
Arbore AVL:
         1995
        /    \
    1993      1996
   /   \       
1992   1994   


Arbore de regasire:
(root)
  └─ '199'
      ├─ '5'
      ├─ '4'
      └─ '2'

*/
