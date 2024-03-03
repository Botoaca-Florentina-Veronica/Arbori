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

// Funcție pentru alocarea memoriei pentru un nou nod Student
Student *createNode(char nume[], int nota, int varsta) 
{
    Student *newNode;
    newNode = (Student *)malloc(sizeof(Student));
    if (newNode == NULL) 
    {
        printf("Eroare la alocarea memoriei pentru un nou nod.\n");
        exit(1);
    }
    strcpy(newNode->nume, nume);
    newNode->nota = nota;
    newNode->varsta = varsta;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Funcție pentru adăugarea unui nod în arbore
Student *insertNode(Student *radacina, char nume[], int nota, int varsta) 
{
    if (radacina == NULL) 
    {
        //daca rădăcina nu există, atunci putem adăuga noul nod, el devenind noua rădăcină
        return createNode(nume, nota, varsta);
    }
    if (strcmp(nume, radacina->nume) < 0) 
    {
        radacina->left = insertNode(radacina->left, nume, nota, varsta);
    } 
    else if (strcmp(nume, radacina->nume) > 0) 
    {
        radacina->right = insertNode(radacina->right, nume, nota, varsta);
    }
    return radacina;
}

// Funcție pentru găsirea celui mai mic nod în arbore (utilizată pentru ștergerea unui nod)
Student *findMinNode(Student* node) 
{
    Student* current = node;
    while (current && current->left != NULL) 
    {
        current = current->left;
    }
    return current;
}

// Funcție pentru ștergerea unui nod din arbore
Student *deleteNode(Student *radacina, char nume[])
 {
    if (radacina == NULL) 
    {
        return radacina;
    }
    if (strcmp(nume, radacina->nume) < 0) 
    {
        radacina->left = deleteNode(radacina->left, nume);
    } 
    else if (strcmp(nume, radacina->nume) > 0) 
    {
        radacina->right = deleteNode(radacina->right, nume);
    } 
    else 
    {
        if (radacina->left == NULL) 
        {
            Student* temp = radacina->right;
            free(radacina);
            return temp;
        } 
        else if (radacina->right == NULL) 
        {
            Student* temp = radacina->left;
            free(radacina);
            return temp;
        }
        Student* temp = findMinNode(radacina->right);
        strcpy(radacina->nume, temp->nume);
        radacina->nota = temp->nota;
        radacina->varsta = temp->varsta;
        radacina->right = deleteNode(radacina->right, temp->nume);
    }
    return radacina;
}

// Funcție pentru căutarea unui nod în arbore
Student* searchNode(Student *radacina, char nume[]) 
{
    if (radacina == NULL || strcmp(nume, radacina->nume) == 0) 
    {
        return radacina;
    }
    if (strcmp(nume, radacina->nume) < 0) 
    {
        return searchNode(radacina->left, nume);
    }
    return searchNode(radacina->right, nume);
}

// Funcție pentru afișarea arborelui în preordine (radacina, stanga, dreapta)
void preOrderTraversal(Student* radacina)
{
    if (radacina != NULL) 
    {
        printf("Nume: %s, Nota: %d, Varsta: %d\n", radacina->nume, radacina->nota, radacina->varsta);
        preOrderTraversal(radacina->left);
        preOrderTraversal(radacina->right);
    }
}

// Funcție pentru afișarea arborelui în inordine (stanga, radacina, dreapta)
void inOrderTraversal(Student *radacina) 
{
    if (radacina != NULL) 
    {
        inOrderTraversal(radacina->left);
        printf("Nume: %s, Nota: %d, Varsta: %d\n", radacina->nume, radacina->nota, radacina->varsta);
        inOrderTraversal(radacina->right);
    }
}

// Funcție pentru afișarea arborelui în postordine (stanga, dreapta, radacina)
void postOrderTraversal(Student *radacina) 
{
    if (radacina != NULL) 
    {
        postOrderTraversal(radacina->left);
        postOrderTraversal(radacina->right);
        printf("Nume: %s, Nota: %d, Varsta: %d\n", radacina->nume, radacina->nota, radacina->varsta);
    }
}

// Funcție pentru eliberarea memoriei alocate pentru arbore (stergerea completa)
void freeTree(Student *radacina) 
{
    if (radacina != NULL) 
    {
        freeTree(radacina->left);
        freeTree(radacina->right);
        free(radacina);
    }
}
