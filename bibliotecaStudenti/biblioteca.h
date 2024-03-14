#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

// Structura pentru înregistrările din fisierul binar
typedef struct {
    int h1;
    float a1;
} Inregistrare;

// Structura pentru nodurile arborelui binar
typedef struct arbore {
    int cheie; // h1
    struct arbore *stanga;
    struct arbore *dreapta;
    int index;
} NodArbore;


// Prototipurile funcțiilor din bibliotecă
int generare_aleatoare(int min, int max);
void creare_fisier_binar(const char *nume_fisier);
void adauga_nod_arbore(NodArbore **radacina, int cheie, int index);
float cautare_fisier_binar(const char *nume_fisier, int cheie, int *nr_comparatii);
float cautare_arbore(NodArbore *radacina, int cheie, int *nr_comparatii);
NodArbore *FindMin(NodArbore* nod);
NodArbore *stergere_nod_arbore(NodArbore *radacina, int cheie);
NodArbore *cauta_cheie(NodArbore *radacina, int cheie);
void preordine(NodArbore *radacina);
void inordine(NodArbore *radacina);
void postordine(NodArbore *radacina);
void freeTree(NodArbore *radacina);

#endif
