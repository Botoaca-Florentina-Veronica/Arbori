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

// NU UITA, într-un arbore binar ordonat, fiecare nod din subarborele stâng are o valoare mai mică decât valoarea nodului părinte, 
// iar fiecare nod din subarborele drept are o valoare mai mare decât nodul părinte.

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

// Funcție pentru căutarea unei chei în arborele binar ordonat
NodArbore *cauta_cheie(NodArbore *radacina, int cheie) 
{
    if (radacina == NULL || radacina->cheie == cheie) 
    {
        return radacina;
    }

    if (cheie < radacina->cheie) 
    {
        return cauta_cheie(radacina->stanga, cheie);
    }

    return cauta_cheie(radacina->dreapta, cheie);
}

// Funcție pentru crearea și adăugarea unui nod în arborele binar
void adauga_nod_arbore(NodArbore **radacina, int cheie, int index)
{
    // daca radacina este egala cu NULL, inseamna ca arborele este gol, deci noul nod pe care
    // vreau sa il adaug, devine chiar radacina arborelui
    if (*radacina == NULL)
    {
        *radacina = (NodArbore *)malloc(sizeof(NodArbore));
        (*radacina)->cheie = cheie;
        (*radacina)->index = index;
        (*radacina)->stanga = NULL;
        (*radacina)->dreapta = NULL;
    }
    // daca radacina nu este egala cu NULL, inseamna ca arborele nu este gol, deci va trebui sa decid, 
    // in ce parte voi adauga nodul
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
float cautare_fisier_binar(const char *nume_fisier, int cheie, int *nr_comparatii)
{
    FILE *f = fopen(nume_fisier, "rb");
    if (f == NULL)
    {
        printf("Eroare la deschiderea fisierului.\n");
        return -1; // Valoare invalidă pentru a1
    }

    Inregistrare inregistrare;
    *nr_comparatii = 0; // Initializam numarul de comparatii cu 0

    int index = 0;
    while (fread(&inregistrare, sizeof(Inregistrare), 1, f))
    {
        (*nr_comparatii)++;
        if (inregistrare.h1 == cheie)
        {
            // când găsim o înregistrare care corespunde cheii, cursorul fișierului este deja poziționat la 
            // începutul următoarei înregistrări, care nu este cea pe care tocmai am găsit-o,
            // deci, pentru a obține valoarea lui a1 corespunzătoare cheii căutate, trebuie să citim din nou înregistrarea corectă din fișier
            
            fseek(f, index * sizeof(Inregistrare), SEEK_SET); // Ne pozitionam inapoi la inceputul inregistrarii corecte
            return inregistrare.a1;
        }
        index++;
    }

    fclose(f);
    return -1; // Valoare invalidă pentru a1
}


// Funcție pentru căutarea și afișarea valorii a1 din arborele binar ordonat
// bazată pe cheia h1, și calcularea numărului de comparații
float cautare_arbore(NodArbore *radacina, int cheie, int *nr_comparatii)
{
    if (radacina == NULL)
    {
        *nr_comparatii = 1;
        return -1; // Valoare invalidă pentru a1
    }

    if (radacina->cheie == cheie)
    {
        *nr_comparatii = 1;
        return radacina->index;
    }
    else if (cheie < radacina->cheie)
    {
        *nr_comparatii++;
        return cautare_arbore(radacina->stanga, cheie, nr_comparatii);
    }
    else
    {
        *nr_comparatii++;
        return cautare_arbore(radacina->dreapta, cheie, nr_comparatii);
    }
}

// Functia pentru gasirea nodului cu cheia minima in subarborele dat

// fiindca toate valorile mai mici ca nodul dat ca parametru se gasesc in subarborele stang mereu, 
// pentru a gasi minimul, voi parcurge rescursiv partea stanga a nodului
NodArbore* FindMin(NodArbore* nod) 
{
    while (nod->stanga != NULL)
    {
        nod = nod->stanga;
    }
    return nod;
}

NodArbore *stergere_nod_arbore(NodArbore *radacina, int cheie)
{
    if (radacina == NULL)
    {
        return NULL;
    }

    if (cheie < radacina->cheie)
    {
        // daca cheia este mai mica decat cea la care ma aflu acum cu parcurgerea, inseamna ca trebuie sa merg in subarborele stang, unde
        // se afla valori mai mici, asa ca merg recursiv pana ajung la el
        radacina->stanga = stergere_nod_arbore(radacina->stanga, cheie);
    }
    else if (cheie > radacina->cheie)
    {
        // altfel, cheia se cauta în subarborele stâng
        radacina->dreapta = stergere_nod_arbore(radacina->dreapta, cheie);
    }
    // ajung aici când cheie == radacina->cheie, deci am ajuns la nodul pe care vreau să îl șterg, dar, inainte de a-l șterge am cateva cazuri:
    else
    {
        // CAZ 1: nodul căutat nu are niciun copil
        if (radacina->stanga == NULL && radacina->dreapta == NULL)
        {
            //înseamnă ca doar sterg nodul la care mă aflu acum, fiind cel cautat, el nealterând restul arborelui
            free(radacina);
            radacina = NULL;
        }
        // CAZ 2:  nodul cautat are un copil (dreapta), deci acest copil va lua locul nodului pe care il vom sterge(al parintelui)
        // ma bazez deci pe acelasi principiu al stergerii unui nod dintr-o lista simplu înlânțuită, caci vreau sa nu pierd restul de arbore, dupa stergere
        else if (radacina->stanga == NULL)
        {
            NodArbore *temp = radacina->dreapta;  // retin intr-un nod temporar, radacina curenta
            free(radacina);                       // eliberez memoria ocupata de radacina pe care o vreau stearsa
            radacina = temp;                      // actualizez valoarea radacinii, pentru a contine copilul drept
        }
        // CAZ 3: un copil (stânga), la fel, copilul va lua locul parintelui ce urmeaza a fi șters
        else if (radacina->dreapta == NULL)
        {
            NodArbore *temp = radacina->stanga;   
            free(radacina);
            radacina=temp;
        }
        // CAZ 4: 2 copii
        else
        {
            NodArbore *temp = FindMin(radacina->dreapta);                          // gasim valoarea minima din subarborele drept
            radacina->cheie = temp->cheie;                                         // copiem valoarea gasita, in radacina curenta
            radacina->dreapta = stergere_nod_arbore(radacina->dreapta, temp->cheie); // stergem nodul la care am gasit valoarea corecta, pentru ca
                                                                                   // nu mai avem nevoie de el
        }
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
