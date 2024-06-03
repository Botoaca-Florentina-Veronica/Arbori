/*
   Consideram ca pentru gestionarea angajatilor unei firme se foloseste o structura de tip arbore generalizat,
implementata cu tablouri, in care pentru fiecare angajat, pe langa datele specifice acestuia, se retine si un
indicator catre seful sau direct. Pentru un angajat se memoreaza ID-ul numeric(unic generat) si numele.
 -Sa se deseneze structurile arborescente, pentru un numar de minim 5 noduri, cu date la alegere
 -Sa se implementeze o functie care citeste datele din arborele generalizat si le stocheaza intr-un arbore binar,
astfel incat daca parcurgem ambii arbori in preordine sa obtinem aceeasi secventa de chei

Nota: Este obligatorie comentarea algoritmului si specificarea complexitatii acestuia in termeni de O(f(n)), 
comentarea si reprezentare grafica a structurilor de date alese, precum si justificarea alegerii acestora
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 10

// Structura arbore generalizat utilizând tablouri:
typedef struct Angajat {
    int id;
    char nume[50];
    int sef_direct_id; // părintele
    int copii[MAX]; 
    int nr_copii;
} Angajat;

// Structura arbore binar utilizând liste:
typedef struct nodArboreBinar {
    int id;
    char nume[50];
    struct nodArboreBinar *stanga;
    struct nodArboreBinar *dreapta;
} nodArboreBinar;

// Funcție pentru crearea unui nod în arborele binar
// complexitate constantă O(1) deoarece efectuează operații simple de asignare și inițializare
nodArboreBinar *creeareNodArboreBinar(int id, char *nume) 
{
    nodArboreBinar *newnode = (nodArboreBinar *)malloc(sizeof(nodArboreBinar));
    if (newnode == NULL) 
    {
        printf("Eroare la alocarea memoriei pentru nodul arborelui binar.\n");
        exit(EXIT_FAILURE);
    }
    newnode->id = id;
    strcpy(newnode->nume, nume);
    newnode->stanga = NULL;
    newnode->dreapta = NULL;
    return newnode;
}

// Funcție recursivă pentru conversia arborelui generalizat în arbore binar
// Complexitate de timp O(n), unde n reprezintă numărul total de angajați
// Aceasta se datorează faptului că trebuie să iterăm prin toți angajații pentru a construi arborele binar
nodArboreBinar *conversieArbore(Angajat angajati[], int id) 
{
    int i;
    if (id == -1) 
    {
        return NULL; // caz special: arborele este gol
    }

    // Căutăm angajatul în tablou
    Angajat *angajat = NULL;
    for (i = 0; i < MAX; i++) 
    {
        if (angajati[i].id == id) 
        {
            //daca am gasit angajatul cu id-ul cautat, vom retine detaliile despre acesta in variabila angajat creeata mai sus
            angajat = &angajati[i];
            break;
        }
    }

    if (angajat == NULL) 
    {
        return NULL; // angajatul nu a fost găsit
    }

    // Creăm nodul corespunzător în arborele binar
    nodArboreBinar *nodBinar = creeareNodArboreBinar(angajat->id, angajat->nume);

    // Conversia copiilor în subarbori
    if (angajat->nr_copii > 0)
     {
        nodBinar->stanga = conversieArbore(angajati, angajat->copii[0]);

        nodArboreBinar *curr = nodBinar->stanga;
        for (i = 1; i < angajat->nr_copii; i++) 
        {
            curr->dreapta = conversieArbore(angajati, angajat->copii[i]);
            curr = curr->dreapta;
        }
    }
    return nodBinar;
}

// Funcție pentru parcurgerea în preordine a arborelui binar
// Complexitate de timp O(n), deoarece trebuie să parcurgem fiecare nod al arborelui binar exact o dată
void parcurgerePreordine(nodArboreBinar *radacina) 
{
    if (radacina == NULL) 
    {
        return;
    }
    printf("%d %s\n", radacina->id, radacina->nume);
    parcurgerePreordine(radacina->stanga);
    parcurgerePreordine(radacina->dreapta);
}

int main(void) 
{
    // Creare angajați și inițializare arbore generalizat
    Angajat angajati[MAX] = 
    {
        {1, "CEO", -1, {2, 5, 4}, 3},    // CEO cu ID 1, 3 copii: Manager1, Employee2, Manager2
        {2, "Manager1", 1, {3}, 1},      // Manager1 cu ID 2, 1 copil: Employee1
        {3, "Employee1", 2, {8}, 1},     // Employee1 cu ID 3, 1 copil: Employee5
        {4, "Manager2", 1, {6, 7}, 2},   // Manager2 cu ID 4, 2 copii: Employee3, Employee4
        {5, "Employee2", 1, {}, 0},      // Employee2 cu ID 5, fără copii
        {6, "Employee3", 4, {9}, 1},     // Employee3 cu ID 6, 1 copil: Employee6
        {7, "Employee4", 4, {}, 0},      // Employee4 cu ID 7, fără copii
        {8, "Employee5", 3, {}, 0},      // Employee5 cu ID 8, fără copii
        {9, "Employee6", 6, {}, 0}       // Employee6 cu ID 9, fără copii
    };

    // Conversia arborelui generalizat în arbore binar
    nodArboreBinar *radacinaBinar = conversieArbore(angajati, 1);

    // Parcurgerea în preordine a arborelui binar rezultat
    printf("Parcurgerea in preordine a arborelui binar:\n");
    parcurgerePreordine(radacinaBinar);

    // Eliberarea memoriei
    // TODO: Scrie o funcție de eliberare a memoriei pentru arborele binar

    return 0;
}


/* 

Arbore generalizat:
                CEO (1)
        /          |           \
  Manager1 (2)  Employee2 (5)   Manager2 (4)
      /                           /      \
Employee1 (3)           Employee3 (6)  Employee4 (7)
   /                         /
Employee5 (8)           Employee6(9)
(Aici parcurgem radacina, arbore stang, arbore drept si mergem recursiv)


                Arbore binar:
                  CEO (1)
            /                 \
        Manager1(2)           Manager2(4)
        /     \                /         \
Employee1(3)  Employee2(5)    Employee3(6) Employee4(7)
     /                         /          
Employee5(8)                Employee6(9)


  Parcurgere in preordine pentru arborele generalizat si cel binar:
(preordine: radacina-stanga-dreapta)
        1 CEO      
        2 Manager1 
        3 Employee1
        8 Employee5
        5 Employee2
        4 Manager2 
        6 Employee3
        9 Employee6
        7 Employee4
*/
