/*
   Scrieti o biblioteca in care sa implementati operatiile generice pentru un arbore binar ordonat (ABO):
Initializare, Adaugare nod, Suprimare Nod, Cautare Nod, Parcurgeri arbore.
Tipul nodului considerati-l de tip struct. Se consideră că se citesc dintr-un fisier binar informatii cu privire la
studenții de la facultate, fiecare având informații legate de nume(string), vârstă(int) și note(float). Acestea vor fi
organizate sub forma unui arbore binar ordonat, care se foloseste si de functiile mentionate mai sus.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"


Arbore initializareArbore() 
{
    return NULL;
}

// Funcție pentru alocarea memoriei pentru un nou nod Student
node *createNode(Student student) 
{
    node *newNode;
    newNode = (node *)malloc(sizeof(node));
    if (newNode == NULL) 
    {
        printf("Eroare la alocarea memoriei pentru un nou nod.\n");
        exit(1);
    }
    newNode->student = student;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Funcție pentru adăugarea unui nod în arbore(vom insera alfabetic)
void addNode(Arbore *radacina, Student student) 
{
    if (*radacina == NULL) 
    {
        //daca rădăcina nu există, atunci putem adăuga noul nod, el devenind noua rădăcină
        *radacina = createNode(student);
    }
    else 
    {
        if (strcmp(student.nume, (*radacina)->student.nume) < 0) 
        {
            addNode(&(*radacina)->left, student);
        } 
        else 
        {
            addNode(&(*radacina)->right, student);
        }
    }
}


node *findMin(node *nod) 
{
    while (nod->left != NULL) 
    {
        nod = nod->left;
    }
    return nod;
}

node *deleteNod(node *radacina, char *nume) 
{
    if (radacina == NULL) 
    {
        return radacina;
    }

    // Găsește nodul de șters în subarborele stâng sau drept
    if (strcmp(nume, radacina->student.nume) < 0) 
    {
        radacina->left = deleteNod(radacina->left, nume);
    } 
    else if (strcmp(nume, radacina->student.nume) > 0) 
    {
        radacina->right = deleteNod(radacina->right, nume);
    } 
    else 
    {
        // Nodul de șters are 0 sau 1 copil
        if (radacina->left == NULL) 
        {
            node *temp = radacina->right;
            free(radacina);
            return temp;
        }
        else if (radacina->right == NULL) 
        {
            node *temp = radacina->left;
            free(radacina);
            return temp;
        }

        // Nodul de șters are 2 copii
        node *temp = findMin(radacina->right);
        radacina->student = temp->student;
        radacina->right = deleteNod(radacina->right, temp->student.nume);
    }
    return radacina;
}


// Funcție pentru afișarea arborelui în inordine (stanga, radacina, dreapta)
void inOrderTraversal(Arbore radacina) 
{
    if (radacina != NULL) 
    {
        inOrderTraversal(radacina->left);
        printf("Nume: %s, Varsta: %d, Nota: %.2f\n", radacina->student.nume, radacina->student.varsta, radacina->student.nota);
        inOrderTraversal(radacina->right);
    }
}


// Funcție pentru afișarea arborelui în preordine (radacina, stanga, dreapta)
void preOrderTraversal(Arbore radacina)
{
    if (radacina != NULL) 
    {
        printf("Nume: %s, Nota: %d, Varsta: %d\n", radacina->student.nume, radacina->student.nota, radacina->student.varsta);
        preOrderTraversal(radacina->left);
        preOrderTraversal(radacina->right);
    }
}


// Funcție pentru afișarea arborelui în postordine (stanga, dreapta, radacina)
void postOrderTraversal(Arbore radacina) 
{
    if (radacina != NULL) 
    {
        postOrderTraversal(radacina->left);
        postOrderTraversal(radacina->right);
        printf("Nume: %s, Nota: %d, Varsta: %d\n", radacina->student.nume, radacina->student.nota, radacina->student.varsta);
    }
}

// Funcție pentru citirea datelor dintr-un fișier binar într-un tablou de structuri
void readingFromBinaryFile(const char *fileName, Student *studenti, int nrInregistrari) 
{
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) 
    {
        printf("Eroare la deschiderea fisierului %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    // Citirea datelor pentru fiecare înregistrare
    fread(studenti, sizeof(Student), nrInregistrari, file);
    fclose(file);
}

// Funcție pentru citirea unei anumite înregistrări identificate prin numărul său din fișier
void readRegistration(const char *fileName, Student *student, int nrInregistrare) 
{
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) 
    {
        printf("Eroare la deschiderea fisierului %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    // Folosirea fseek pentru a poziționa cursorul la începutul înregistrării dorite
    fseek(file, (nrInregistrare - 1) * sizeof(Student), SEEK_SET);

    // Citirea înregistrării
    fread(student, sizeof(Student), 1, file);
    fclose(file);
}

// Funcție pentru scrierea datelor dintr-un arbore într-un fișier text
void writeTreeInTextFile(Arbore arbore, const char *fileName) 
{
    FILE *file = fopen(fileName, "w");
    if (file == NULL) 
    {
        printf("Eroare la deschiderea fisierului %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    // Scrierea datelor din arbore în fișierul text
    writeNodeInTextFile(arbore, file);

    fclose(file);
}

// Funcție auxiliară pentru scrierea nodurilor din arbore într-un fișier text
void writeNodeInTextFile(Arbore arbore, FILE *file) 
{
    if (arbore != NULL) 
    {
        // Scrierea informațiilor despre student în fișierul text
        fprintf(file, "Nume: %s, Varsta: %d, Nota: %.2f\n", arbore->student.nume, arbore->student.varsta, arbore->student.nota);
        
        // Parcurgerea subarborilor
        writeNodeInTextFile(arbore->left, file);
        writeNodeInTextFile(arbore->right, file);
    }
}


// Funcție pentru eliberarea memoriei alocate pentru arbore (stergerea completa)
void freeTree(Arbore radacina) 
{
    if (radacina != NULL) 
    {
        freeTree(radacina->left);
        freeTree(radacina->right);
        free(radacina);
    }
}
