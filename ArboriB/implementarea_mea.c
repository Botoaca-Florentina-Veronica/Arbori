// arbore AVL

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define N 4 // gradul arborelui B

/*  TEORIE ARBORI B
 nr max chei = 2N
 nr min chei = N
 nr max fii = 2N+1
 nr min fii = N+1
*/

// cand spun fiu sau copil ma refer la același lucru!!

typedef struct pagina
{
    int nr_chei;     // numarul curent de chei aflate in pagina
    int chei[N - 1]; // vector de chei(aici îmi rețin cheile)
    bool frunza;     // adică pagina terminală
    pagina *fii[N];  // vector de pointeri ai fiilor
} pagina;

// functie pentru creearea unei pagini noi
pagina *creearePagina(bool frunza)
{
    int i;
    pagina *paginaNoua;
    paginaNoua = (pagina *)malloc(sizeof(pagina));
    if (paginaNoua == NULL)
    {
        perror("Eroare la alocarea dinamica!");
        exit(EXIT_FAILURE);
    }

    paginaNoua->nr_chei = 0; // la creearea noii pagini nu am elemente
    paginaNoua->frunza = frunza;

    for (i = 0; i < N; i++)
    {
        // fiind un array de pointeri, vom initializa elementele cu NULL
        paginaNoua->fii[i] = NULL;
    }

    return paginaNoua;
}

// Function to split a full child node
void scindarePagina(pagina *parent, int index)
{
    //aici eu îi dau ca parametru funcției pagina părinte și indexul paginii fiu a paginii părinte, pe care vreau să o împart
    //fiindcă am nevoie să nu pierd legătura cu pagina părinte, acolo va trebui introdusă ulterior cheia mediană
    pagina *fiu = parent->fii[index];                // Aici, fiu este pagina copil plină care va fi împărțită
    pagina *paginaNoua = creearePagina(fiu->frunza); // paginaNoua este noua pagină creată pentru a primi a doua jumătate din cheile și copiii paginii fiu

    // De ce creez doar o variabilă care să conțină a doua jumătate din cheile din pagina parinte? Ce fac cu cele aflate în prima jumătate? ei bine, nu pot arunca fosta
    //  pagina părinte care conținea ambele pagini de dinaintea scindării, ar fi o risipă
    // Așa că o voi 'recicla', și deci noul ei scop va fi acum să conțină valorile mai mici decât cheia, aflate în prima jumătate

    paginaNoua->nr_chei = N / 2 - 1;

    int i;
    // Mutarea cheilor și fiilor în noua pagină:
    for (i = 0; i < N / 2 - 1; i++)
    {
        paginaNoua->chei[i] = fiu->chei[i + N / 2];
    }

    //cat timp pagina nu e frunză, înseamnă că am și fii pe care trebuie sa îi mut în pagina nouă
    if (!fiu->frunza)
    {
        for (i = 0; i < N / 2; i++)
        {
            paginaNoua->fii[i] = fiu->fii[i + N / 2];  //aici se află deocamdată și cheia mediană
        }
    }

    fiu->nr_chei = N / 2 - 1; // aici actualizăm numarul de chei
    // au rămas doar cheile mai mici decât mediana care urcă la părinte

    //Deplasăm fii paginii părinte pentru a face loc noii pagini
    //Vom deplasa doar paginile ce contin cheile mai mici decât cheia mediană(adica vectorul de pagini fii în care au ramas acestea)
    //cu un pas spre stanga, rămânând astfel loc pentru încă o pagină. Acolo voi introduce paginaNoua, creeată
    //de mine la începutul funcției. ATENȚIE, nu parcurg întreg vectorul de fii, merg de la index spre stânga,
    //nu are rost să parcurg si să shiftez, toate paginile

    for (int i = parent->nr_chei; i > index; i--)
    {
        parent->fii[i + 1] = parent->fii[i];
    }

    parent->fii[index + 1] = paginaNoua; // inserăm noua pagină ce conține valoarea/valorile mai mari decât
    //pagina cu cheia mediana, în locul corespunzător
 
    //ATENȚIE, valorile mai mari sunt raportate la cheia mediana, nu e neapărat nevoie ca valorile mai mari sa se afle la final
    //de aceea ma raportez la index, acolo știu ca are loc scindarea, deci in locul cel mai din dreapta al indexului îmi voi insera noua pagină
    //cu valori mai mari decât cheia mediană

    // Deplasăm cheile paginii părinte pentru a insera cheia mediană din fiu
    for (i = parent->nr_chei - 1; i >= index; i--)
    {
        parent->chei[i + 1] = parent->chei[i];
    }

    //cheia mea mediană se afla în paginaNoua, adica in pagina cu valori cel putin mai mari decat cheia mediana
    //iar in urma inserarii acestei pagini la locul potrivit scindării, adică la index, de aici îmi voi prelua valoarea
    // fiu->chei[N/2-1] e acelasi lucru cu paginaNoua->chei[N/2-1]
    parent->chei[index] = fiu->chei[N / 2 - 1];
    parent->nr_chei++;
}

// Function to insert a key into a non-full node
void inserarePaginaNeplină(pagina *pagina, int key)
{
    int i;
    i = pagina->nr_chei - 1; // asignăm lui i indexul ultimei chei din pagina, și o vom folosi pentru a itera înapoi prin chei
    // De ce iterăm înapoi? Păi, vreau să trec doar prin valorile mai mari decât cheia mea, pentru ca mai apoi să
    // le shiftez, și să îmi introduc elementul în pagina. Nu are rost sa parcurg crescător ca să trec prin toate elementele
    // mai mici decât ce am eu nevoie

    if (pagina->frunza) // Dacă pagina curentă este o frunză, introducem direct cheia în aceasta
    {
        // Insert key into the sorted order
        while (i >= 0 && pagina->chei[i] > key)
        {
            // aici parcurgem de la (dreapta spre stânga) cheile din pagină până ajungem la cea mai mare
            // cand o găsim, vom shifta interativ spre dreapta(cu un index/unitate) acest element si cele ce se afla dupa el
            // fiindca știm că daca pagina nu e plină, mai există cel puțin un loc gol în ea
            pagina->chei[i + 1] = pagina->chei[i];
            // elementul din față va deveni cel curent
            i--; // Decrementăm i pentru a continua compararea cu cheia următoare
        }
        pagina->chei[i + 1] = key; // reținem în locul pe care doar ce l-am eliberat, valoarea noastră
        pagina->nr_chei++;         // incrementăm numărul de chei din pagină
    }
    else // dacă nu ne aflăm într-o pagină terminală, înseamnă că trebuie să parcurg întreg arborele B, de jos în sus, până aflu pagina unde trebuie inserată cheia
    {
        while (i >= 0 && pagina->chei[i] > key)
        {
            i--;
        }
        i++; // am gasit indexul paginii fiu la care vreau să cobor pentru a insera cheia(fiindcă știu sigur că pagina are fii, nefiind pagină terminală-frunză )

        // cu indexul găsit anterior, verific dacă pagina fiu e plină:
        if (pagina->fii[i]->nr_chei == N - 1)
        {
            // Dacă pagina copil este plină, trebuie să o împărțim înainte de a insera cheia
            scindarePagina(pagina, i);

            // După împărțire, cheia mediană se ridică în pagina părinte(adică pagina de la care am aflat indexul în while-ul de mai sus),
            // iar pagina copil se împarte în două pagini mai mici
            // Trebuie să verificăm dacă cheia pe care dorim să o inserăm se află în pagina din stânga sau din dreapta medianei
            // pentru a continua inserarea în nodul corespunzător
            if (pagina->chei[i] < key)
            {
                // dacă cheia din pagina la care ma aflu cu parcurgerea, e mai mică decât cea pe care vreau sa o inserez, trebuie să merg în pagina fiu din dreapta, deci îl incrementez pe i
                // și parcurg arborele până ajung la pagina corectă
                i++;
            }
        }
        inserarePaginaNeplină(pagina->fii[i], key);
        // În final, apelăm recursiv funcția pentru pagina copil corespunzătoare, pentru a continua procesul de inserare
    }
}

// Function to insert a key into the B-tree
void insert(pagina **root, int key)
{
    pagina *node = *root;

    if (node == NULL)
    {
        // Create a new root node
        *root = creearePagina(true);
        (*root)->chei[0] = key;
        (*root)->nr_chei = 1;
    }
    else
    {
        if (node->nr_chei == N - 1)
        {
            // scindăm pagina fiindca e plină
            pagina *new_root = creearePagina(false);
            new_root->fii[0] = node;
            scindarePagina(new_root, 0);
            *root = new_root;
        }
        inserarePaginaNeplină(*root, key);
    }
}

// Function to traverse and print the B-tree in-order
void traverse(pagina *root)
{
    int i;
    if (root != NULL)
    {
        for (i = 0; i < root->nr_chei; i++)
        {
            traverse(root->fii[i]);
            printf("%d ", root->chei[i]);
        }
        traverse(root->fii[i]);
    }
}

// Main function to test B-tree implementation
int main(void)
{
    pagina *root = NULL;

    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 5);
    insert(&root, 6);
    insert(&root, 12);
    insert(&root, 30);

    printf("In-order traversal of the B-tree: ");
    traverse(root);
    printf("\n");

    return 0;
}
