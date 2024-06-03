/*
   Intr-o baza de date sunt memorate intrari de tipul nume(string), data nasterii in Unix epoch time(int)
(ex. 1590384730). Baza de date este implementata sub forma unui arbore B. Sa se implementeze o functie care 
afiseaza numele tuturor persoanelor nascute dupa o data primita ca parametru(Unix epoch time)

Nota: Este obligatorie comentarea algoritmului si specificarea complexitatii acestuia in termeni de O(f(n)), 
comentarea structurilor de date alese, precum si justificarea alegerii acestora.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

// Structura pentru a reprezenta o intrare în baza de date
typedef struct {
    char name[MAX_NAME_LENGTH];
    int birth_date;
} DatabaseEntry;

// Structura pentru un nod din arborele B
typedef struct BTreeNode {
    DatabaseEntry entry;
    struct BTreeNode *left;
    struct BTreeNode *right;
} BTreeNode;

// Funcție pentru a crea un nou nod în arborele B
BTreeNode *createNode(DatabaseEntry entry) 
{
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (newNode == NULL) 
    {
        printf("Eroare la alocarea memoriei!\n");
        exit(EXIT_FAILURE);
    }
    newNode->entry = entry;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Funcție pentru a insera un nou nod în arborele B
void insertNode(BTreeNode **root, DatabaseEntry entry) {
    if (*root == NULL) 
    {
        *root = createNode(entry);
    } 
    else 
    {
        if (entry.birth_date < (*root)->entry.birth_date) 
        {
            insertNode(&(*root)->left, entry);
        } 
        else 
        {
            insertNode(&(*root)->right, entry);
        }
    }
}

// Funcție pentru a afișa numele persoanelor născute după o anumită dată în arborele B
void printNamesAfterDate(BTreeNode *root, int targetDate) 
{
    if (root == NULL) 
    {
        return;
    }
    // Verificăm dacă data de naștere a persoanei este mai mare decât data țintă
    if (root->entry.birth_date > targetDate) 
    {
        printf("%s\n", root->entry.name);
    }
    // Parcurgem subarborii în ordine
    printNamesAfterDate(root->left, targetDate);
    printNamesAfterDate(root->right, targetDate);
}

int main(void) 
{
    // Exemplu de utilizare
    BTreeNode *root = NULL;
    DatabaseEntry entry1 = {"John", 1610384730}; // Exemplu de date
    DatabaseEntry entry2 = {"Alice", 1600384730}; // Exemplu de date
    DatabaseEntry entry3 = {"Bob", 1580384730}; // Exemplu de date

    insertNode(&root, entry1);
    insertNode(&root, entry2);
    insertNode(&root, entry3);

    int targetDate = 1590384730; // Exemplu de data țintă
    printf("Persoane nascute dupa %d:\n", targetDate);
    printNamesAfterDate(root, targetDate);

    return 0;
}
