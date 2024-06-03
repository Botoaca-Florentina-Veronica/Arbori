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

Structura de Date: Arbore AVL
  Un arbore AVL este un tip de arbore binar de căutare în care diferența de înălțime dintre subarborii 
stânga și dreapta pentru orice nod este cel mult 1. 
  Aceasta asigură că arborele rămâne echilibrat, permițând operații eficiente.



*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_LOCURI 10 // Constanta pentru numărul de locuri disponibile

typedef struct NodAVL {
    int cod_student;
    float nota;
    bool admis;
    int inaltime;
    struct NodAVL* stanga;
    struct NodAVL* dreapta;
} NodAVL;

// Funcție pentru a obține înălțimea unui nod
int inaltime(NodAVL* N) 
{
    if (N == NULL) 
    {
        return 0;
    } 
    else 
    {
        return N->inaltime;
    }
}

// Funcție pentru a crea un nod nou
NodAVL* creareNod(int cod_student, float nota) 
{
    NodAVL* nod = (NodAVL*)malloc(sizeof(NodAVL));
    nod->cod_student = cod_student;
    nod->nota = nota;
    nod->admis = false;
    nod->inaltime = 1; // Noul nod este inițializat cu înălțimea 1
    nod->stanga = NULL;
    nod->dreapta = NULL;
    return nod;
}

// Funcție pentru a roti dreapta subarborele cu rădăcina y
NodAVL* rotireDreapta(NodAVL* y) 
{
    NodAVL* x = y->stanga;
    NodAVL* T2 = x->dreapta;

    x->dreapta = y;
    y->stanga = T2;

    if (inaltime(y->stanga) > inaltime(y->dreapta)) 
    {
        y->inaltime = 1 + inaltime(y->stanga);
    } 
    else 
    {
        y->inaltime = 1 + inaltime(y->dreapta);
    }

    if (inaltime(x->stanga) > inaltime(x->dreapta)) 
    {
        x->inaltime = 1 + inaltime(x->stanga);
    } 
    else 
    {
        x->inaltime = 1 + inaltime(x->dreapta);
    }

    return x;
}

// Funcție pentru a roti stânga subarborele cu rădăcina x
NodAVL* rotireStanga(NodAVL* x) 
{
    NodAVL* y = x->dreapta;
    NodAVL* T2 = y->stanga;

    y->stanga = x;
    x->dreapta = T2;

    if (inaltime(x->stanga) > inaltime(x->dreapta)) 
    {
        x->inaltime = 1 + inaltime(x->stanga);
    } 
    else 
    {
        x->inaltime = 1 + inaltime(x->dreapta);
    }

    if (inaltime(y->stanga) > inaltime(y->dreapta)) 
    {
        y->inaltime = 1 + inaltime(y->stanga);
    } 
    else 
    {
        y->inaltime = 1 + inaltime(y->dreapta);
    }

    return y;
}

// Funcție pentru a obține factorul de echilibru al unui nod
int factorEchilibru(NodAVL* N) 
{
    if (N == NULL) 
    {
        return 0;
    } 
    else 
    {
        return inaltime(N->stanga) - inaltime(N->dreapta);
    }
}

// Funcție pentru a insera un nod nou și a echilibra arborele
NodAVL* inserareNod(NodAVL* nod, int cod_student, float nota) 
{
    if (nod == NULL)
    {
        return creareNod(cod_student, nota);
    }

    if (cod_student < nod->cod_student)
    {
        nod->stanga = inserareNod(nod->stanga, cod_student, nota);
    }
    else if (cod_student > nod->cod_student)
    {
        nod->dreapta = inserareNod(nod->dreapta, cod_student, nota);
    }
    else
    {
        return nod; // Codul studentului este deja prezent
    }

    if (inaltime(nod->stanga) > inaltime(nod->dreapta)) 
    {
        nod->inaltime = 1 + inaltime(nod->stanga);
    } 
    else 
    {
        nod->inaltime = 1 + inaltime(nod->dreapta);
    }

    int balans = factorEchilibru(nod);

    if (balans > 1 && cod_student < nod->stanga->cod_student)
    {
        return rotireDreapta(nod);
    }

    if (balans < -1 && cod_student > nod->dreapta->cod_student)
    {
        return rotireStanga(nod);
    }

    if (balans > 1 && cod_student > nod->stanga->cod_student) 
    {
        nod->stanga = rotireStanga(nod->stanga);
        return rotireDreapta(nod);
    }

    if (balans < -1 && cod_student < nod->dreapta->cod_student) 
    {
        nod->dreapta = rotireDreapta(nod->dreapta);
        return rotireStanga(nod);
    }

    return nod;
}

// Funcție pentru a găsi un student după cod și a-i actualiza nota
NodAVL* actualizareNota(NodAVL* radacina, int cod_student, float noua_nota) 
{
    if (radacina == NULL)
    {
        return NULL;
    }

    if (cod_student < radacina->cod_student)
    {
        radacina->stanga = actualizareNota(radacina->stanga, cod_student, noua_nota);
    }
    else if (cod_student > radacina->cod_student)
    {
        radacina->dreapta = actualizareNota(radacina->dreapta, cod_student, noua_nota);
    }
    else
    {
        radacina->nota = noua_nota; // Găsit studentul, actualizează nota
    }

    return radacina;
}

// Funcție pentru a marca studenții admiși
void marcheazaAdmisi(NodAVL* radacina, int* locuri_ramase) 
{
    if (radacina == NULL)
    {
        return;
    }

    marcheazaAdmisi(radacina->stanga, locuri_ramase);

    if (*locuri_ramase > 0 && radacina->nota > 5) 
    {
        radacina->admis = true;
        (*locuri_ramase)--;
    }

    marcheazaAdmisi(radacina->dreapta, locuri_ramase);
}

// Funcție pentru a accesa și transmite simultan 8 studenți
void transmiteStudenti(NodAVL* radacina, NodAVL** buffer, int* index) 
{
    if (radacina == NULL || *index >= 8)
    {
        return;
    }

    transmiteStudenti(radacina->stanga, buffer, index);

    if (*index < 8) 
    {
        buffer[*index] = radacina;
        (*index)++;
    }

    transmiteStudenti(radacina->dreapta, buffer, index);
}

int main(void) 
{
    NodAVL* radacina = NULL;
    radacina = inserareNod(radacina, 1, 6.5);
    radacina = inserareNod(radacina, 2, 8.0);
    radacina = inserareNod(radacina, 3, 4.5);
    radacina = inserareNod(radacina, 4, 9.0);
    radacina = inserareNod(radacina, 5, 7.0);

    // Actualizare nota
    actualizareNota(radacina, 3, 5.5);

    // Marcare studenți admiși
    int locuri_ramase = NUM_LOCURI;
    marcheazaAdmisi(radacina, &locuri_ramase);

    // Transmitere 8 studenți
    NodAVL* buffer[8];
    int index = 0;
    transmiteStudenti(radacina, buffer, &index);

    for (int i = 0; i < index; i++) 
    {
        printf("Student Cod: %d, Nota: %.2f, Admis: %s\n", buffer[i]->cod_student, buffer[i]->nota, buffer[i]->admis ? "Da" : "Nu");
    }

    return 0;
}


/*
Complexitatea totală a codului depinde de combinația operațiunilor efectuate asupra arborelui AVL. 
Să analizăm fiecare funcție și apoi să vedem cum se combină acestea.

Inserarea și Echilibrarea Nodurilor (inserareNod)

Complexitate: O(log n) pentru fiecare inserare.
Inserarea nodurilor în exemplul din main se face de 5 ori, fiecare având complexitate O(log n), 
deci total pentru inserări: O(5 * log n) = O(log n) (deoarece log n este dominant).
Actualizarea Notei (actualizareNota)

Complexitate: O(log n) pentru găsirea nodului și actualizarea notei.
Marcarea Studenților Admiși (marcheazaAdmisi)

Complexitate: O(n) pentru parcurgerea întregului arbore și marcarea studenților.
Transmiterea Simultană a 8 Studenți (transmiteStudenti)

Complexitate: O(1) pentru fiecare nod până la un maxim de 8 noduri, deci O(1).
Calcularea Complexității Totale
Considerând toate operațiile executate în funcția main, complexitatea totală este suma complexităților individuale:

Inserări: O(log n) (dominantă datorită inserărilor multiple)
Actualizare notă: O(log n)
Marcare studenți admiși: O(n)
Transmitere simultană a 8 studenți: O(1)
Complexitatea totală combinată pentru operațiile din main este:

O(logn)+O(logn)+O(n)+O(1)=O(n)

Complexitatea totală a unui algoritm compus din mai multe operații se determină de termenul cel 
mai mare (dominant) din suma complexităților individuale ale acestor operații. Aceasta se întâmplă 
deoarece complexitatea algoritmului este dată de partea care necesită cel mai mult timp de execuție 
pentru a se finaliza, în termeni de creștere asimptotică.

Astfel, complexitatea totală a codului este dominată de operația de parcurgere pentru marcarea 
studenților admiși, rezultând într-o complexitate totală de O(n).
*/
