#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Nod
{
    struct Nod *alfabet[27]; // 26 litere + 1 pentru '[', deci 27
} Nod;

void Initializare(Nod *Nod)
{
    // ATENȚIE, aceasta funcție trebuie apelată cu *Nod deja alocat
    char c;
    for (c = 'A'; c <= '['; c++)
    {
        // imi iterez alfabetic prin toate cele 27 de caractere, de la 0 la 26
        // și mi le inițializez cu NULL, fiind de tip char, adică pointer
        // indicând că nu există niciun copil (subarbore) pentru acel caracter
        Nod->alfabet[c - 'A'] = NULL;
    }
}

void Atribuie(Nod *nod, char c, Nod *p)
{
    nod->alfabet[c - 'A'] = p;
}

// Această funcție returnează pointerul către nodul asociat cu caracterul c din nodul curent
// Practic, verifică dacă există un subarbore pentru acel caracter și returnează adresa acelui subarbore
Nod *Valoare(Nod *Nod, char c)
{
    return Nod->alfabet[c - 'A'];
}

void NodNou(Nod *nod, char c)
{
    nod->alfabet[c - 'A'] = (Nod *)malloc(sizeof(Nod));
    Initializare(nod->alfabet[c - 'A']);
}

// Această funcție adaugă un cuvânt în arborele de regăsire. Parcurge fiecare caracter al cuvântului x,
// creând noduri noi dacă acestea nu există și avansează în arbore până la sfârșitul cuvântului
// La sfârșit, adaugă un terminator ('[') pentru a marca sfârșitul cuvântului
void Adauga(char *x, Nod *cuvinte) // x = cuvântul, cuvinte = rădăcina arborelui de regăsire
{
    unsigned i;
    Nod *t;
    t = cuvinte;

    for (i = 0; i < strlen(x); i++)
    {
        if (Valoare(t, x[i]) == NULL) // nodul curent nu are fiu pentru caracterul x[i], deci se crează unul nou
        {
            NodNou(t, x[i]);
        }
        t = Valoare(t, x[i]); // avansez în arborele de regăsire
    }
    Atribuie(t, '[', t); // se face o buclă pentru '[', pentru a marca un nod terminal
}

// Această funcție parcurge arborele și afișează toate cuvintele stocate
// Utilizând un buffer cuv și un nivel niv, funcția adaugă litere în buffer și avansează recursiv în subarbori
// Dacă găsește un terminator ('['), marchează sfârșitul unui cuvânt și îl afișează
void Parcurge(Nod *Nod, char cuv[], int niv) // nodul curent, un buffer pentru construirea cuvântului, nivelul curent
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
    Nod *radacina = (Nod *)malloc(sizeof(Nod));
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
