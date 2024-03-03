#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

typedef struct Student {
    char nume[50];
    int nota;
    int varsta;
    struct Student *left;
    struct Student *right;
} Student;

// Prototipurile funcțiilor din bibliotecă
Student *createNode(char nume[], int nota, int varsta);
Student *insertNode(Student *radacina, char nume[], int nota, int varsta);
Student *findMinNode(Student *node);
Student *deleteNode(Student *radacina, char nume[]);
Student *searchNode(Student *radacina, char nume[]);
void preOrderTraversal(Student *radacina);
void inOrderTraversal(Student *radacina);
void postOrderTraversal(Student *radacina);
void freeTree(Student *radacina);

#endif
