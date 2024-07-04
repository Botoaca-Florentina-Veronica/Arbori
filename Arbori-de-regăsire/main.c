#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Definim structura NodArboreDeRegasire
typedef struct NodArboreDeRegasire
{
    struct NodArboreDeRegasire *alfabet[27]; // 26 litere + 1 pentru '[', deci 27
} NodArboreDeRegasire;

void Initializare(NodArboreDeRegasire *Nod)
{
    // ATENȚIE, aceasta funcție trebuie apelată cu *Nod deja alocat
    char c;
    for (c = 'A'; c <= '['; c++)
        Nod->alfabet[c - 'A'] = NULL;
}

void Atribuie(NodArboreDeRegasire *Nod, char c, NodArboreDeRegasire *p)
{
    Nod->alfabet[c - 'A'] = p;
}

NodArboreDeRegasire *Valoare(NodArboreDeRegasire *Nod, char c)
{
    return Nod->alfabet[c - 'A'];
}

void NodNou(NodArboreDeRegasire *Nod, char c)
{
    Nod->alfabet[c - 'A'] = (NodArboreDeRegasire *)malloc(sizeof(NodArboreDeRegasire));
    Initializare(Nod->alfabet[c - 'A']);
}

void Adauga(char *x, NodArboreDeRegasire *cuvinte) // x = cuvântul, cuvinte = rădăcina arborelui de regăsire
{
    unsigned i;
    NodArboreDeRegasire *t;
    t = cuvinte;

    for (i = 0; i < strlen(x); i++)
    {
        if (Valoare(t, x[i]) == NULL) // nodul curent nu are fiu pentru caracterul x[i], deci se crează unul nou
            NodNou(t, x[i]);
        t = Valoare(t, x[i]); // avansez în arborele de regăsire
    }
    Atribuie(t, '[', t); // se face o buclă pentru '[', pentru a marca un nod terminal
}

void Parcurge(NodArboreDeRegasire *Nod, char cuv[], int niv) // nodul curent, un buffer pentru construirea cuvântului, nivelul curent
{
    char c;
    for (c = 'A'; c <= 'Z'; c++) // pentru toate literele considerate
    {
        if (Valoare(Nod, c)) // dacă există legătură în dicționar
        {
            cuv[niv] = c;                            // adaug litera găsită în buffer
            Parcurge(Valoare(Nod, c), cuv, niv + 1); // avansez
        }
    }
    if (Valoare(Nod, '[')) // există terminator => am găsit un cuvânt întreg
    {
        cuv[niv] = '\0';
        printf("%s\n", cuv); // prelucrez datele
    }
}

int main(void)
{
    NodArboreDeRegasire *radacina = (NodArboreDeRegasire *)malloc(sizeof(NodArboreDeRegasire));
    Initializare(radacina);

    // Adăugăm cuvinte în arbore
    Adauga("CAT", radacina);
    Adauga("DOG", radacina);
    Adauga("COW", radacina);

    // Parcursul arborelui pentru a afișa cuvintele stocate
    char buffer[100]; // buffer suficient de mare pentru a ține cuvintele
    Parcurge(radacina, buffer, 0);

    return 0;
}
