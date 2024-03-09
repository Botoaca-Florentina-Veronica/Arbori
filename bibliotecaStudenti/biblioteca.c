/*
Scrieti o biblioteca in C in care sa implementati urmatoarele:
-creeaza un fisier binar cu 1000 inregistrari cu structura
{
   int h1;
   float a1;
}
fisierul contine valori random!

-functie de creeare si adaugare noduri pentru un arbore binar ordonat
unde cheia este h1. Structura arbore binar:
typedef struct arbore{
   int cheie;  //h1
   arbore *stanga;
   arbore *dreapta;
   int index;
}
unde index este indexul inregistrarii din fisier(incepe de la 0).

-se citeste de la tastatura un un numar intreg, iar cu el se vor face
2 cautari:
	*cautam in fisierul binar, dupa h1 si afisam pe ecran valoarea lui a1,
         si calculam numarul de comparratii
	*cealalta cautare se realizeaza in arborele binar ordonat si se cauta dupa h1,
         si se afiseaza valoarea a1 din fisierul binar, iar apoi se calculeaza 
         numarul de comparatii
cautarile se vor realiza folosind fseek+indice
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "biblioteca.h"

// Funcție pentru generarea unei valori aleatoare între min și max
int generare_aleatoare(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

// Funcție pentru crearea și popularea fișierului binar cu înregistrări random
void creare_fisier_binar(const char *nume_fisier) 
{
    FILE *file;
    file = fopen(nume_fisier, "wb");
    int i;
    if (file == NULL) 
    {
        printf("Eroare la deschiderea fisierului.\n");
        return;
    }

    srand(time(NULL));

    Inregistrare inregistrare;
    for (i = 0; i < 1000; i++) 
    {
        inregistrare.h1 = generare_aleatoare(1, 1000);
        inregistrare.a1 = (float)generare_aleatoare(1, 1000) / 10;
        fwrite(&inregistrare, sizeof(Inregistrare), 1, file);
    }
    fclose(file);
}

// Funcție pentru crearea și adăugarea unui nod în arborele binar
void adauga_nod_arbore(NodArbore **radacina, int cheie, int index) 
{
    if (*radacina == NULL) 
    {
        *radacina = (NodArbore *)malloc(sizeof(NodArbore));
        (*radacina)->cheie = cheie;
        (*radacina)->index = index;
        (*radacina)->stanga = NULL;
        (*radacina)->dreapta = NULL;
    } 
    else 
    {
        if (cheie < (*radacina)->cheie) 
        {
            adauga_nod_arbore(&((*radacina)->stanga), cheie, index);
        } 
        else 
        {
            adauga_nod_arbore(&((*radacina)->dreapta), cheie, index);
        }
    }
}

// Funcție pentru căutarea și afișarea valorii a1 din fisierul binar
// bazată pe cheia h1, și calcularea numărului de comparații
float cautare_fisier_binar(const char *nume_fisier, int cheie, int *numar_comparatii) 
{
    FILE *f = fopen(nume_fisier, "rb");
    if (f == NULL) 
    {
        printf("Eroare la deschiderea fisierului.\n");
        return -1; // Valoare invalidă pentru a1
    }

    Inregistrare inregistrare;
    *numar_comparatii = 0;

    int index = 0;
    while (fread(&inregistrare, sizeof(Inregistrare), 1, f)) 
    {
        (*numar_comparatii)++;
        if (inregistrare.h1 == cheie) 
        {
            fclose(f);
            FILE *f2 = fopen(nume_fisier, "rb");
            if (f2 == NULL) 
            {
                printf("Eroare la deschiderea fisierului.\n");
                return -1; // Valoare invalidă pentru a1
            }
            fseek(f2, index * sizeof(Inregistrare), SEEK_SET);
            fread(&inregistrare, sizeof(Inregistrare), 1, f2);
            fclose(f2);
            return inregistrare.a1;
        }
        index++;
    }
    fclose(f);
    return -1; // Valoare invalidă pentru a1
}


// Funcție pentru căutarea și afișarea valorii a1 din arborele binar ordonat
// bazată pe cheia h1, și calcularea numărului de comparații
float cautare_arbore(NodArbore *radacina, int cheie, int *numar_comparatii) 
{
    if (radacina == NULL) 
    {
        *numar_comparatii = 1;
        return -1; // Valoare invalidă pentru a1
    }

    if (radacina->cheie == cheie) 
    {
        *numar_comparatii = 1;
        return radacina->index;
    } 
    else if (cheie < radacina->cheie) 
    {
        (*numar_comparatii)++;
        return cautare_arbore(radacina->stanga, cheie, numar_comparatii);
    } 
    else 
    {
        (*numar_comparatii)++;
        return cautare_arbore(radacina->dreapta, cheie, numar_comparatii);
    }
}

// Funcție pentru ștergerea unui nod din arbore
NodArbore *sterge_nod_arbore(NodArbore *radacina, int cheie) 
{
    if (radacina == NULL) 
    {
        return radacina;
    }

    if (cheie < radacina->cheie) 
    {
        radacina->stanga = sterge_nod_arbore(radacina->stanga, cheie);
    } 
    else if (cheie > radacina->cheie) 
    {
        radacina->dreapta = sterge_nod_arbore(radacina->dreapta, cheie);
    } 
    else 
    {
        // Nodul are unul sau niciun copil
        if (radacina->stanga == NULL) 
        {
            NodArbore *temp = radacina->dreapta;
            free(radacina);
            return temp;
        } 
        else if (radacina->dreapta == NULL) 
        {
            NodArbore *temp = radacina->stanga;
            free(radacina);
            return temp;
        }

        // Nodul are doi copii: găsește succesorul înordine (cel mai mic nod din subarborele drept)
        NodArbore *succesor = radacina->dreapta;
        while (succesor && succesor->stanga != NULL) 
        {
            succesor = succesor->stanga;
        }

        // Copiază cheia și indexul succesorului în nodul curent
        radacina->cheie = succesor->cheie;
        radacina->index = succesor->index;

        // Șterge succesorul
        radacina->dreapta = sterge_nod_arbore(radacina->dreapta, succesor->cheie);
    }
    return radacina;
}

// Funcție pentru parcurgerea arborelui în preordine
void preordine(NodArbore *radacina) 
{
    if (radacina != NULL) 
    {
        printf("%d ", radacina->cheie);
        preordine(radacina->stanga);
        preordine(radacina->dreapta);
    }
}

// Funcție pentru parcurgerea arborelui în inordine
void inordine(NodArbore *radacina) 
{
    if (radacina != NULL) 
    {
        inordine(radacina->stanga);
        printf("%d ", radacina->cheie);
        inordine(radacina->dreapta);
    }
}

// Funcție pentru parcurgerea arborelui în postordine
void postordine(NodArbore *radacina) 
{
    if (radacina != NULL) 
    {
        postordine(radacina->stanga);
        postordine(radacina->dreapta);
        printf("%d ", radacina->cheie);
    }
}

// Funcție pentru eliberarea memoriei alocate pentru arbore (stergerea completa)
void freeTree(NodArbore *radacina) 
{
    if (radacina != NULL) 
    {
        freeTree(radacina->stanga);
        freeTree(radacina->dreapta);
        free(radacina);
    }
}
