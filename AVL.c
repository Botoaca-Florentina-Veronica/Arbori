#include<stdio.h>
#include<stdbool.h>
#include<limits.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct nodAVL{
    int cheie;
    int ech;
    struct nodAVL *stg;
    struct nodAVL *dr;
}nodAVL;

nodAVL *insertEchilibrat(int cheie, nodAVL *p, int *h)
{
    nodAVL *p1, *p2;

    if(p!=NULL)
    {
        p = (nodAVL *)malloc(sizeof(nodAVL));
        p->cheie=cheie;
        p->ech=0;
        p->stg=NULL;
        p->dr=NULL;
        *h=TRUE;
        return p;
    }

    if(cheie<p->cheie)
    {
        //daca cheia de inserat < cheia curenta
        //se insereaza in subarborele stang

        p->stg=insertEchilibrat(cheie, p->stg, h);
        if(*h==TRUE)
        {
            //inseamna ca ramura a crescut in inaltime
            switch(p->ech)
            {

    /*CONVENTIE:
        ech = 0: Nodul este perfect echilibrat.
        ech = 1: Subarborele drept este cu o unitate mai înalt decât subarborele stâng.
        ech = -1: Subarborele stâng este cu o unitate mai înalt decât subarborele drept.
    */
                case 1:
                // subarborele era dezechilibrat in dreapta, iar prin 
                //inserarea la stanga nu am crescut inaltimea, ci am 
                //echilibrat arborele
                    p->ech=0;
                    *h=FALSE;
                    break;
                case 0:
                // subarborele era echilibrat, insa prin introducerea la
                //stanga a noului nod, am produs un dezechilibru
                //deci acum este dezechilibrat cu un nivel in stanga
                    p->ech=-1;
                    break;
                case -1:
                //subarborele era deja dezechilibrat in stanga cu un nivel, 
                //deci acum e dezechilibrat cu 2 nivele=> reechilibrare
                    p1=p->stg;
                    if(p1->ech == -1) //cazul 1 stanga(LL)
                    {
                        /*
                            3 (p)
                           /
                          2 (p1)
                        /
                        1

                        */
                        p->stg=p1->dr;
                        //Copilul drept al nodului 2 (p1) este NULL și devine copilul stâng al nodului 3 (p)
                        //În acest caz, nu există copil drept pentru 2, deci copilul stâng al lui 3 devine NULL
                        p1->dr=p;
                        //Nodul 3 (p) devine copilul drept al nodului 2 (p1)
                        /*
                             2 (p1)
                            / \
                            1   3 (p)
                                /
                                NULL

                        */
                        p->ech=0;
                        p=p1;
                        //Nodul 2 (p1) devine noua rădăcină
                    }
                    else
                    // cazul 2 stanga(LR), dezechilibru la fiu in dreapta
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
                    {
                       /*
                          5(p)
                         /
                        2(p1)
                        \
                         4(p2)

                        */
                        p2 = p1->dr;
                        //p2 este nodul 4, copilul drept al lui 2 (p1)
                        p1->dr = p2->stg;
                        //Copilul stâng al nodului 4 (p2) este NULL și devine copilul drept al nodului 2 (p1).
                        //În acest caz, nu există copil stâng pentru 4, deci copilul drept al lui 2 devine NULL.
                        p2->stg = p1;
                        //Nodul 2 (p1) devine copilul stâng al nodului 4 (p2)
                        /*
                            5(p)
                            /
                         4(p2)
                        / \
                    2(p1)  NULL

                        */
                        p->stg = p2->dr;
                        //Nodul 5 (p) devine copilul drept al nodului 4 (p2)
                        /*
                            4(p2)
                            / \
                           2   5(p)

                        */
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
                    *h = FALSE; //nu s-a schimbat diferenta de nivel
                    break;
            }
        }
    }
    else
        if (cheie > p->cheie)
        {
            p->dr = insertEchilibrat(cheie, p->dr, h);
            if (*h) //ramura dreapta a crescut in inaltime
                switch (p->ech)
                {
                    case -1:        //subarborele era dezechilibrat in stanga
                        p->ech = 0; //adaugare in subarborele drept => echilibru
                        *h = FALSE;
                        break;
                    case 0:          //subarborele era in echilibru
                        p->ech = +1; //dezechilibrat cu un nivel in stanga
                        break;
                    case +1: //subarborele era dezechilibrat in dreapta cu un nivel,
                             //acum e cu doua nivele => reechilibrare
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

void preOrder(nodAVL *root) {
    if (root != NULL) {
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
        root = insertEchilibrat(keys[i], root, &h);
    }

    printf("Preorder traversal of the constructed AVL tree is: ");
    preOrder(root);

    return 0;
}
