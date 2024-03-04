#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

typedef struct Student{
    char nume[50];
    int varsta;
    float nota;
} Student;

typedef struct node {
    Student student;
    struct node *left;
    struct node *right;
}node;

typedef node* Arbore;

// Prototipurile funcțiilor din bibliotecă
Arbore initializareArbore();
node *createNode(Student student);
void addNode(Arbore *radacina, Student student);
node *findMin(node *nod);
node *deleteNod(node *radacina, char *nume);
void preOrderTraversal(Arbore radacina);
void inOrderTraversal(Arbore radacina);
void postOrderTraversal(Arbore radacina);
void readingFromBinaryFile(const char *fileName, Student *studenti, int nrInregistrari);
void readRegistration(const char *fileName, Student *student, int nrInregistrare);
void writeTreeInTextFile(Arbore arbore, const char *fileName);
void writeNodeInTextFile(Arbore arbore, FILE *file); 
void freeTree(Arbore radacina);

#endif
