#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

int main(void) 
{
    int i;
    creare_fisier_binar("fisier_binar.dat");

    NodArbore *radacina = NULL;
    FILE *file = fopen("fisier_binar.dat", "rb");
    if (file == NULL) 
    {
        printf("Eroare la deschiderea fisierului.\n");
        return 1;
    }

    Inregistrare inregistrare;
    for (i = 0; i < 1000; i++) 
    {
        //citim o inregistrare una, cate una
        fread(&inregistrare, sizeof(Inregistrare), 1, file);
        adauga_nod_arbore(&radacina, inregistrare.h1, i);
    }
    fclose(file);

    // Căutare în fisierul binar și în arbore binar ordonat
    int cheie;
    printf("Introduceti o valoare pentru cautare: ");
    scanf("%d", &cheie);

    int numar_comparatii_fisier, numar_comparatii_arbore;
    float valoare_a1_fisier = cautare_fisier_binar("fisier_binar.dat", cheie, &numar_comparatii_fisier);
    float valoare_a1_arbore = cautare_arbore(radacina, cheie, &numar_comparatii_arbore);

    if (valoare_a1_fisier != -1) 
    {
        printf("Valoare a1 din fisier: %.2f\n", valoare_a1_fisier);
        printf("Numar de comparatii in fisier: %d\n", numar_comparatii_fisier);
    } 
    else 
    {
        printf("Valoarea nu a fost gasita in fisierul binar.\n");
    }

    if (valoare_a1_arbore != -1) 
    {
        printf("Valoare a1 din arbore: %.2f\n", valoare_a1_arbore);
        printf("Numar de comparatii in arbore: %d\n", numar_comparatii_arbore);
    } 
    else 
    {
        printf("Valoarea nu a fost gasita in arborele binar ordonat.\n");
    }

    // Stergere nod din arbore
    int cheie_sters;
    printf("Introduceti o valoare pentru stergere din arbore: ");
    scanf("%d", &cheie_sters);
    radacina = stergere_nod_arbore(radacina, cheie_sters);
    printf("Arborele dupa stergerea nodului %d:\n", cheie_sters);
    inordine(radacina);
    printf("\n");

    // Parcurgerea arborelui în preordine, înordine și postordine
    printf("Parcurgere in preordine:\n");
    preordine(radacina);
    printf("\n");

    printf("Parcurgere in inordine:\n");
    inordine(radacina);
    printf("\n");

    printf("Parcurgere in postordine:\n");
    postordine(radacina);
    printf("\n");

    // Eliberarea memoriei alocate pentru arbore
    freeTree(radacina);
    return 0;
}
