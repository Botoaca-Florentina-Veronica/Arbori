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
#include <string.h>

#define NUM_LOCURI_LIBERE 5

typedef struct {
    int id;
    float nota;
    bool admis;
} Student;

// Structura pentru un nod al arborelui binar de căutare
typedef struct Nod {
    Student student;
    struct Nod *stanga;
    struct Nod *dreapta;
} Nod;

// Funcție pentru crearea unui nod nou
Nod *creareNod(int id, float nota) 
{
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->student.id = id;
    nodNou->student.nota = nota;
    nodNou->student.admis = false;
    nodNou->stanga = nodNou->dreapta = NULL;
    return nodNou;
}

// Funcție pentru inserarea unui nod în arbore
Nod *inserareNod(Nod *radacina, int id, float nota) 
{
    if (radacina == NULL) 
    {
        return creareNod(id, nota);
    }
    if (id < radacina->student.id) 
    {
        radacina->stanga = inserareNod(radacina->stanga, id, nota);
    } 
    else if (id > radacina->student.id) 
    {
        radacina->dreapta = inserareNod(radacina->dreapta, id, nota);
    }
    return radacina;
}

// Funcție pentru căutarea unui nod după id
Nod *cautareNod(Nod *radacina, int id) 
{
    if (radacina == NULL || radacina->student.id == id) 
    {
        return radacina;
    }
    if (id < radacina->student.id) 
    {
        return cautareNod(radacina->stanga, id);
    } 
    else 
    {
        return cautareNod(radacina->dreapta, id);
    }
}

// Funcție pentru actualizarea notei unui student
void actualizareNota(Nod *radacina, int id, float notaNoua) 
{
    Nod *nod = cautareNod(radacina, id);
    if (nod != NULL) 
    {
        nod->student.nota = notaNoua;
    }
}

// Funcție pentru determinarea și marcarea studenților admiși
void marcheazaAdmisi(Nod *radacina, int *locuriLibere) 
{
    if (radacina == NULL || *locuriLibere <= 0) 
    {
        return;
    }
    marcheazaAdmisi(radacina->stanga, locuriLibere);
    if (radacina->student.nota > 5.0 && *locuriLibere > 0) 
    {
        radacina->student.admis = true;
        (*locuriLibere)--;
    }
    marcheazaAdmisi(radacina->dreapta, locuriLibere);
}

// Funcție pentru afișarea informațiilor despre un student
void afiseazaStudent(Nod *nod) 
{
    if (nod != NULL) 
    {
        printf("ID: %d, Nota: %.2f, Admis: %s\n", nod->student.id, nod->student.nota, nod->student.admis ? "Da" : "Nu");
    }
}

// Funcție pentru accesarea și transmiterea a câte 8 studenți
void transmiteStudenti(Nod *radacina, int *counter) 
{
    if (radacina == NULL || *counter >= 8) 
    {
        return;
    }
    transmiteStudenti(radacina->stanga, counter);
    if (*counter < 8) 
    {
        afiseazaStudent(radacina);
        (*counter)++;
    }
    transmiteStudenti(radacina->dreapta, counter);
}

// Funcție pentru eliberarea memoriei arborelui
void elibereazaArbore(Nod* radacina) 
{
    if (radacina == NULL) 
    {
        return;
    }
    elibereazaArbore(radacina->stanga);
    elibereazaArbore(radacina->dreapta);
    free(radacina);
}

int main(void) 
{
    Nod* radacina = NULL;
    // Introducere studenti
    radacina = inserareNod(radacina, 10, 4.5);
    radacina = inserareNod(radacina, 20, 6.0);
    radacina = inserareNod(radacina, 30, 8.5);
    radacina = inserareNod(radacina, 40, 3.7);
    radacina = inserareNod(radacina, 50, 7.2);
    radacina = inserareNod(radacina, 60, 9.1);
    radacina = inserareNod(radacina, 70, 5.8);
    radacina = inserareNod(radacina, 80, 4.2);
    radacina = inserareNod(radacina, 90, 6.5);
    radacina = inserareNod(radacina, 100, 7.0);

    // Actualizare notă pentru un student
    actualizareNota(radacina, 10, 5.5);

    // Marchează studenții admiși
    int locuriLibere = NUM_LOCURI_LIBERE;
    marcheazaAdmisi(radacina, &locuriLibere);

    // Transmite primii 8 studenți
    int counter = 0;
    transmiteStudenti(radacina, &counter);

    // Eliberează memoria
    elibereazaArbore(radacina);
    return 0;
}

/*
   Algoritmul de marcare a studenților admiși parcurge arborele în ordine și marchează primii studenți eligibili 
în funcție de nota lor, până când locurile disponibile sunt epuizate. Dacă un student cu o notă mai mare este procesat 
după ce toate locurile au fost ocupate, acesta nu va fi marcat ca admis, chiar dacă nota lui este mai mare decât a unui 
student deja admis.

Complexitatea totală a codului este:

În medie: O(n log n)
În cel mai rău caz: O(n^2)
*/
