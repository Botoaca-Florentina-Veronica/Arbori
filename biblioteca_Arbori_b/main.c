#include <stdio.h>
#include <stdlib.h>
#include "arbori_b.h"

#define NUMAR_INREGISTRARI 1000
#define NUMAR_CHEI_CAUTARE 1

Pagina *radacina;

int main(void) 
{
    const char *numeFisier = "inregistrari.bin";
    int numarComparatiiFisierBinar, numarComparatiiArboreB;

    // Creează și populează fișierul binar cu înregistrări aleatoare
    populeazaFisierBinar(numeFisier, NUMAR_INREGISTRARI);

    // Citire înregistrări din fișierul binar
    Nod inregistrari[NUMAR_INREGISTRARI];
    citesteFisierBinar(numeFisier, inregistrari, NUMAR_INREGISTRARI);

    // Construire arbore B
    radacina = NULL;
    for (int i = 0; i < NUMAR_INREGISTRARI; i++)
    {
        radacina = insereaza(radacina, inregistrari[i].cheie, &inregistrari[i]);
    }
    // Afisare arbore B initial
    printf("Arborele-B initial:\n");
    afisare(radacina, 1);
    printf("\n\n");

    // Exemplu de stergere si afisare a arborelui B
    /*
    for (int i = 1; i <= NUMAR_INREGISTRARI; i++) 
    {
        printf("Dupa stergerea lui %d:\n", i);
        radacina = suprima(radacina, i, NULL);
        afisare(radacina, 1);
        printf("\n\n");
    }
    */

    // Parcurgere arbore B
    
    printf("Parcurgere in inordine:\n");
    inordine(radacina);
    printf("\n\n");

    printf("Parcurgere in preordine:\n");
    preordine(radacina);
    printf("\n\n");

    printf("Parcurgere in postordine:\n");
    postordine(radacina);
    printf("\n\n");
    

     int i;
    // Exemplu de cautare în arbore B și afișare a numărului de comparații
    for (i = 0; i < NUMAR_CHEI_CAUTARE; i++) 
    {
        TipCheie cheieCautare = rand() % 10000; // Chei aleatoare între 0 și 9999
        cautareFisierBinar(numeFisier, cheieCautare, &numarComparatiiFisierBinar);
        cautare(radacina, cheieCautare, &numarComparatiiArboreB);
        printf("Numarul de comparatii pentru cautarea cheii %d in fisierul binar: %d\n", cheieCautare, numarComparatiiFisierBinar);
        printf("Numarul de comparatii pentru cautarea cheii %d in arborele B: %d\n", cheieCautare, numarComparatiiArboreB);
    }

    return 0;
}
