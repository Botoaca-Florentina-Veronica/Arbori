#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"


int main(void) 
{
    Student *radacina = NULL;

    // Exemplu de adăugare a unor noduri
    radacina = insertNode(radacina, "Alice", 9, 20);
    radacina = insertNode(radacina, "Bob", 8, 22);
    radacina = insertNode(radacina, "Charlie", 7, 21);

    printf("Parcurgere inordine:\n");
    inOrderTraversal(radacina);

    // Exemplu de ștergere a unui nod
    radacina = deleteNode(radacina, "Bob");

    printf("\nParcurgere inordine dupa stergere:\n");
    inOrderTraversal(radacina);

    // Exemplu de cautare a unui nod
    char searchName[] = "Alice";
    Student* foundNode = searchNode(radacina, searchName);
    if (foundNode != NULL) 
    {
        printf("\n%s gasit: Nota: %d, Varsta: %d\n", searchName, foundNode->nota, foundNode->varsta);
    } 
    else 
    {
        printf("\n%s nu a fost gasit in arbore.\n", searchName);
    }

    // Eliberare memorie
    freeTree(radacina);
    return 0;
}
