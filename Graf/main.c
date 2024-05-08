#include<stdio.h>
#include<stdlib.h>
#include"graf.h"

int main(void) 
{
    Graf g;
    Initializare(&g);

    // Inserare noduri
    Nod nod1 = {1, 10};
    Nod nod2 = {2, 20};
    Nod nod3 = {3, 30};

    InsertNod(&g, nod1);
    InsertNod(&g, nod2);
    InsertNod(&g, nod3);

    // Inserare arce
    Arc arc1 = {0, 1};
    Arc arc2 = {1, 2};

    InsertArc(&g, arc1);
    InsertArc(&g, arc2);

    // Afisare matrice de adiacenta
    Afiseaza_Arce(&g);

    // Suprimare nod și arc
    SuprimaArc(&g, arc1);
    SuprimaNod(&g, 1);

    // Afisare matrice de adiacenta dupa stergere
    printf("\nDupa stergere:\n");
    Afiseaza_Arce(&g);

    return 0;
}
