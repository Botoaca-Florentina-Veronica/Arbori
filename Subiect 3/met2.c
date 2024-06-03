/*
   Intr-o baza de date sunt memorate intrari de tipul nume(string), data nasterii in Unix epoch time(int)
(ex. 1590384730). Baza de date este implementata sub forma unui arbore B. Sa se implementeze o functie care 
afiseaza numele tuturor persoanelor nascute dupa o data primita ca parametru(Unix epoch time)

Nota: Este obligatorie comentarea algoritmului si specificarea complexitatii acestuia in termeni de O(f(n)), 
comentarea structurilor de date alese, precum si justificarea alegerii acestora.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 2
#define NN (2 * N)

typedef struct {
    int cheie;
    char nume[50];
} Nod;

typedef struct Pagina {
    int m;
    Nod e[NN + 1];
    struct Pagina *copil[NN + 2]; // Referință la copii (nu mai folosim 'p0')
} Pagina;

Pagina *insereaza(Pagina *pag, int x, char *nume);
void afisare(Pagina *arbore, int nivel);
void afisareNumeDupaData(Pagina *arbore, int dataNasterii);


Pagina *insereaza(Pagina *pag, int x, char *nume) {
    if (!nume)
        return pag;

    if (pag == NULL) {
        pag = (Pagina *)malloc(sizeof(Pagina));
        pag->m = 1;
        pag->e[1].cheie = x;
        strcpy(pag->e[1].nume, nume);
        for (int i = 0; i < NN + 2; i++) {
            pag->copil[i] = NULL;
        }
        return pag;
    }

    int i;
    for (i = 1; i <= pag->m; i++) {
        if (x < pag->e[i].cheie)
            break;
    }

    if (pag->copil[i] == NULL) {
        if (pag->m < NN) {
            for (int j = pag->m; j >= i; j--) {
                pag->e[j + 1] = pag->e[j];
                pag->copil[j + 1] = pag->copil[j];
            }
            pag->e[i].cheie = x;
            strcpy(pag->e[i].nume, nume);
            pag->m++;
        } else {
            Pagina *temp = (Pagina *)malloc(sizeof(Pagina));
            temp->m = N;
            for (int j = 1; j <= N; j++) {
                temp->e[j] = pag->e[j + N];
                temp->copil[j] = pag->copil[j + N];
            }
            pag->m = N;
            if (i > N) {
                temp->copil[1] = pag->copil[N + 1];
                for (int j = N + 1; j < i; j++) {
                    pag->copil[j] = pag->copil[j + 1];
                }
                pag->copil[i] = NULL;
                temp->e[1] = pag->e[N + 1];
                for (int j = N + 1; j < i - 1; j++) {
                    pag->e[j] = pag->e[j + 1];
                }
                pag->e[i - 1].cheie = x;
                strcpy(pag->e[i - 1].nume, nume);
            } else {
                temp->copil[1] = pag->copil[N];
                for (int j = N; j >= i; j--) {
                    temp->e[j - N + 1] = pag->e[j];
                    temp->copil[j - N + 2] = pag->copil[j + 1];
                }
                pag->m--;
                temp->e[1].cheie = x;
                strcpy(temp->e[1].nume, nume);
            }
            temp->copil[0] = pag->copil[N + 1];
            pag->copil[N + 1] = NULL;
            return temp;
        }
    } else {
        pag->copil[i] = insereaza(pag->copil[i], x, nume);
    }
    return pag;
}


void afisare(Pagina *arbore, int nivel) {
    int i;

    if (!arbore)
        return;
    for (i = 1; i <= arbore->m + 1; i++)
        afisare(arbore->copil[i], nivel + 1);

    printf("Nivel %d: ", nivel);
    for (i = 1; i <= arbore->m; i++)
        printf("%s ", arbore->e[i].nume);
    printf("\n");
}

void afisareNumeDupaData(Pagina *arbore, int dataNasterii) {
    if (!arbore)
        return;

    for (int i = 1; i <= arbore->m; i++) {
        if (arbore->e[i].cheie > dataNasterii) {
            printf("%s\n", arbore->e[i].nume);
        }
        afisareNumeDupaData(arbore->copil[i + 1], dataNasterii);
    }
}

int main() {
    Pagina *arbore = NULL;

    arbore = insereaza(arbore, 1590384730, "John");
    arbore = insereaza(arbore, 1600384730, "Alice");
    arbore = insereaza(arbore, 1580384730, "Bob");

    int dataCautata = 1590000000;
    printf("Persoane nascute dupa data de %d:\n", dataCautata);
    afisareNumeDupaData(arbore, dataCautata);

    // Eliberăm memoria alocată pentru arbore (nu implementez această parte aici pentru simplificare)

    return 0;
}
