/*
   Notele de la examenul de admitere trebuie introduse în sistemul facultății care trebuie
să rezolve urmatoarele cerințe:

1. În caz de contestație un student trebuie să poată fi găsit după cod, iar nota să fie
schimbată cu o nouă notă.
2. Sistemul va trebui să acceseze și să transmită simultan câte 8 studenți cu toate datele
aferente către un sistem al rectoratului.
3. Sistemul va trebui să determine, și să marcheze într-un câmp special studenții admiși.
Un student se consideră admis dacă are nota la examen peste 5, și dacă mai sunt locuri libere.
Numărul de locuri libere va fi definit într-o constantă/macro.

   Să se aleagă, să se exemplifice structura de date necesară pentru un număr de minim 5 
noduri, cu date la alegere și să se scrie cod pentru rezolvarea cerințelor sistemului.

Notă: Este obligatorie comentarea algoritmului și specificarea complexității acestuia în
termeni de O(f(n)), comentarea structurilor de date alese, precum și justificarea acestora.

Rezolvare:
Voi folosi un Arbore Binar de Căutare (BST)
Un arbore binar de căutare este util pentru operațiile de căutare, inserare și modificare, 
având complexitatea O(log n) pentru operațiile de bază, în condiții ideale (arbore echilibrat).
*/



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUMAR_LOCURI 5

//Structura Student stochează datele esențiale: codul studentului, nota și starea de admitere
typedef struct {
    int id;
    float nota;
    bool admis;
} Student;

// Structura pentru un nod al arborelui binar de cautare
typedef struct Nod {
    Student student;
    struct Nod* stanga;
    struct Nod* dreapta;
} Nod;

// Functie pentru a crea un nou nod
Nod* creeazaNod(int id, float nota) 
{
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->student.id = id;
    nodNou->student.nota = nota;
    nodNou->student.admis = false;
    nodNou->stanga = nodNou->dreapta = NULL;
    return nodNou;
}

// Functie pentru a insera un nod in BST
// Complexitate: O(log n) în medie, O(n) în cel mai rău caz
// Inserarea într-un arbore binar de căutare echilibrat (sau aproape echilibrat) are o complexitate logaritmică
// În cel mai rău caz (dacă arborele este degenerat și arată ca o listă), complexitatea devine liniară
Nod *insereazaNod(Nod *radacina, int id, float nota) 
{
    if (radacina == NULL)
    {
        return creeazaNod(id, nota);
    }
    if (id < radacina->student.id)
    {
        radacina->stanga = insereazaNod(radacina->stanga, id, nota);
    }
    else if (id > radacina->student.id)
    {
        radacina->stanga = insereazaNod(radacina->stanga, id, nota);
    }
    return radacina;
}

// Functie pentru a cauta un nod dupa id
// Complexitate: O(log n) în medie, O(n) în cel mai rău caz
Nod *gasesteNod(Nod *radacina, int id) 
{
    if (radacina == NULL || radacina->student.id == id)
    {
        return radacina;
    }
    if (id < radacina->student.id)
    {
        return gasesteNod(radacina->stanga, id);
    }
    return gasesteNod(radacina->dreapta, id);
}

// Functie pentru a modifica nota unui student
// Complexitate: O(log n) în medie, O(n) în cel mai rău caz
// Această funcție se bazează pe gasesteNod pentru a găsi nodul care trebuie modificat
// Prin urmare, complexitatea este aceeași ca și pentru gasesteNod
void modificaNota(Nod *radacina, int id, float notaNoua) 
{
    Nod *nod = gasesteNod(radacina, id);
    if (nod != NULL) 
    {
        nod->student.nota = notaNoua;
    }
}

// Functie pentru a transmite simultan 8 studenti catre rectorat
// Complexitate: O(n)
void transmiteStudentiRectorat(Nod *radacina, int *counter) 
{
    if (radacina == NULL || *counter >= 8)
    {
        return;
    }
    transmiteStudentiRectorat(radacina->stanga, counter);
    if (*counter < 8) 
    {
        printf("ID: %d, Nota: %.2f, Admis: %s\n", radacina->student.id, radacina->student.nota, radacina->student.admis ? "Da" : "Nu");
        (*counter)++;
    }
    transmiteStudentiRectorat(radacina->dreapta, counter);
}

// Functie pentru a marca studentii admisi
// Complexitate: O(n)
void marcheazaStudentiAdmisi(Nod* radacina, int* locuriOcupate, int numarLocuri) 
{
    if (radacina == NULL)
    {
        return;
    }
    marcheazaStudentiAdmisi(radacina->stanga, locuriOcupate, numarLocuri);
    if (radacina->student.nota > 5 && *locuriOcupate < numarLocuri) 
    {
        radacina->student.admis = true;
        (*locuriOcupate)++;
    } 
    else 
    {
        radacina->student.admis = false;
    }
    marcheazaStudentiAdmisi(radacina->dreapta, locuriOcupate, numarLocuri);
}

// Functie pentru a traversa arborele si a afisa studentii
//  Complexitate: O(n)
// Funcția traversează arborele în ordinea in-order și afișează toți studenții 
// Traversarea arborelui are complexitate liniară
void afiseazaStudenti(Nod* radacina) 
{
    if (radacina == NULL)
    {
        return;
    }
    afiseazaStudenti(radacina->stanga);
    printf("ID: %d, Nota: %.2f, Admis: %s\n", radacina->student.id, radacina->student.nota, radacina->student.admis ? "Da" : "Nu");
    afiseazaStudenti(radacina->dreapta);
}

int main(void) 
{
    Nod* radacina = NULL;
    radacina = insereazaNod(radacina, 1, 4.5);
    radacina = insereazaNod(radacina, 2, 6.0);
    radacina = insereazaNod(radacina, 3, 7.5);
    radacina = insereazaNod(radacina, 4, 5.5);
    radacina = insereazaNod(radacina, 5, 8.0);
    radacina = insereazaNod(radacina, 6, 4.0);
    radacina = insereazaNod(radacina, 7, 6.5);
    radacina = insereazaNod(radacina, 8, 7.0);

    // Exemplu de modificare a unei note
    modificaNota(radacina, 2, 9.0);

    // Exemplu de transmitere a studentilor catre rectorat
    int counter = 0;
    transmiteStudentiRectorat(radacina, &counter);

    // Exemplu de marcare a studentilor admisi
    int locuriOcupate = 0;
    marcheazaStudentiAdmisi(radacina, &locuriOcupate, NUMAR_LOCURI);

    // Afisare studenti pentru verificare
    afiseazaStudenti(radacina);

    return 0;
}


/*
Complexitatea totală a codului este:

În medie: O(n log n)
În cel mai rău caz: O(n^2)

*/
