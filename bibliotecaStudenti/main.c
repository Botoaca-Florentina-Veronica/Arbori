#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

int main(void) 
{
    // Inițializare arbore binar ordonat
    Arbore arbore = initializareArbore();

    // Citirea datelor din fișierul binar într-un tablou de structuri
    const char *fileName = "studenti.dat";
    int nrInregistrari = 10; // presupunând că avem 10 înregistrări în fișier
    Student studenti[1000]; // declararea tabloului pentru a stoca datele
    readingFromBinaryFile(fileName, studenti, nrInregistrari);

    // Adăugarea datelor în arbore
    int i;
    for (i = 0; i < nrInregistrari; i++) 
    {
        addNode(&arbore, studenti[i]);
    }

    // Citirea unei anumite înregistrări din fișier
    int nrInregistrareCautata = 17; // de exemplu, citim înregistrarea cu numărul 17
    Student studentCautat;
    readRegistration(fileName, &studentCautat, nrInregistrareCautata);

    // Afișarea arborelui
    printf("Parcurgere inordine a arborelui:\n");
    inOrderTraversal(arbore);

    // Eliberarea memoriei alocate pentru arbore
    freeTree(arbore);
    return 0;
}
