// Implementeaza in C un arbore binar ordonat. Apoi afla 'stramosul comun' la 2 noduri date

#include <stdio.h>
#include <stdlib.h>

// Definim structura nodului arborelui binar
typedef struct Node
{
    int cheie;
    struct Node *stg;
    struct Node *dr;
} Node;

// Funcție pentru a crea un nod nou
Node *createNode(int cheie)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->cheie = cheie;
    newNode->stg = NULL;
    newNode->dr = NULL;
    return newNode;
}

// Funcție pentru a insera un nod în arborele binar ordonat
Node *insertNode(Node *radacina, int cheie)
{
    if (radacina == NULL)
    {
        radacina = createNode(cheie);
    }
    else if (cheie < radacina->cheie)
    {
        radacina->stg = insertNode(radacina->stg, cheie);
    }
    else
    {
        radacina->dr = insertNode(radacina->dr, cheie);
    }
    return radacina;
}

// Funcție pentru a găsi strămoșul comun pentru 2 noduri date
Node *gasesteStramosComun(Node *radacina, int n1, int n2)
{
    if (radacina == NULL)
    {
        printf("Arborele este gol!!");
        return NULL;
    }

    // daca ambele noduri sunt mai mici decat radacina arborelui, atunci ambele se afla in subarborele stang, deci cautam in stanga
    if (n1 < radacina->cheie && n2 < radacina->cheie)
    {
        return gasesteStramosComun(radacina->stg, n1, n2);
    }

    // daca ambele sunt mai mari decat radacina, ele se vor afla in subarborele drept, deci parcurgem recursiv in partea dreapta
    if (n1 > radacina->cheie && n2 > radacina->cheie)
    {
        return gasesteStramosComun(radacina->dr, n1, n2);
    }

    // Daca ma aflu in cazul in care:
    // *fiul din stanga == n1 si < radacina curenta si fiul din dreapta == n2 si > radacina curenta, inseamna ca ma aflu la stramosul comun deci il returnez:
    // *fiul din stanga == n1 si < radacina curenta si fiul din dreapta !=n2 si > radacina curenta
    // (deci nu ma aflu la nodul drept cu cheia corecta, el se afla mai jos in subarbore), dar stramosul comun este tot cel curent deci returnez:
    // *fiul din stanga != n1 si < radacina curenta si fiul din dreapta ==n2 si > radacina curenta
    // (deci nu ma aflu la nodul stang cu cheia corecta, el se afla mai jos in subarbore), dar stramosul comun este tot cel curent deci returnez:
    return radacina;
}

int main(void)
{
    Node *root = NULL;
    root = insertNode(root, 12);
    insertNode(root, 10);
    insertNode(root, 15);
    insertNode(root, 9);
    insertNode(root, 11);
    insertNode(root, 13);
    insertNode(root, 19);
    insertNode(root, 20);

    int n1 = 13, n2 = 19;
    Node *stramos = gasesteStramosComun(root, n1, n2);

    if (stramos != NULL)
    {
        printf("Stramosul comun al nodurilor %d si %d este %d.\n", n1, n2, stramos->cheie);
    }
    else
    {
        printf("Nodurile %d si %d nu au un stramos comun.\n", n1, n2);
    }

    return 0;
}
