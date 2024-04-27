/*
Project SO
   Subiectul proiectului este monitorizarea modificarilor aparute in directoare de-a lungul timpului, prin realizarea de capturi (snapshots) la
cererea utilizatorului.
Cerintele programului:
   Se va dezvolta un program în limbajul C, prin care utilizatorul va putea specifica directorul de monitorizat ca argument în linia de
comandă, iar programul va urmări schimbările care apar în acesta și în subdirectoarele sale.
   La fiecare rulare a programului, se va actualiza captura (snapshot-ul) directorului, stocând metadatele fiecărei intrări.
Pentru a reprezenta grafic un director și schimbările survenite înainte și după rularea programului de monitorizare a modificărilor, putem utiliza un arbore de fișiere simplu.

Directorul înainte de rularea programului:

Director Principal
|_ Subdirector1
| |_ Fișier1.txt
| |_ Fișier2.txt
|_ Subdirector2
| |_ Fișier3.txt
|_ Fișier4.txt


Directorul după rularea programului:
// Versiunea 1
Director Principal
|_ Subdirector1
| |_ Fișier1.txt
| |_ Fișier2.txt
|_ Subdirector2
| |_ Fișier3.txt
|_ Fișier4.txt
|_ Snapshot.txt(sau alt format specificat)


// Versiunea 2
Director Principal
|_ Subdirector1
| |_ Fișier1.txt
| |_ Fișier1_snapshot.txt
| |_ Fișier2.txt
| |_ Fișier2_snapshot.txt
|_ Subdirector2
| |_ Fișier3.txt
| |_ Fișier3_snapshot.txt
|_ Fișier4.txt
|_ Fișier4_snapshot.txt

  Nota: S-au prezentat două variante ca exemplu, însă organizarea directorului și a fișierelor de snapshot poate fi realizată în orice fel dorit,
fără a exista restricții impuse.
Explicatie versiunea 1: După rularea programului, în directorul principal este creat un nou fișier, denumit "Snapshot.txt" (sau alt format
specificat), care conține metadatele fiecărei intrări (fișiere și subdirectoare) din directorul principal și subdirectoarele sale. Acest snapshot
este actualizat la fiecare rulare a programului pentru a reflecta modificările survenite.
  Nota: Selectarea metadatelor este complet la latitudinea utilizatorului. Este important să includem informații care să diferențieze fiecare
intrare din director într-un mod unic și să evidențieze modificările efectuate.


      Setup inotify notifications (IN) mask. All these defined in inotify.h. 

static int event_mask =
  (IN_ACCESS |        // File accessed
   IN_ATTRIB |        // File attributes changed
   IN_OPEN   |        // File was opened
   IN_CLOSE_WRITE |   // Writtable File closed
   IN_CLOSE_NOWRITE | // Unwrittable File closed
   IN_CREATE |        // File created in directory
   IN_DELETE |        // File deleted in directory
   IN_DELETE_SELF |   // Directory deleted
   IN_MODIFY |        // File modified
   IN_MOVE_SELF |     // Directory moved
   IN_MOVED_FROM |    // File moved away from the directory
   IN_MOVED_TO);      // File moved into the directory
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

// Structura pentru metadatele unui fișier sau director
typedef struct {
    char name[256]; // Numele fișierului sau directorului
    char type;      // 'F' pentru fișier, 'D' pentru director
    time_t last_modified; // Data ultimei modificări
} Metadata;

// Funcția pentru crearea metadatelor unui fișier sau director
Metadata create_metadata(const char *path) 
{
    Metadata metadata;
    struct stat st;
    stat(path, &st);
    strcpy(metadata.name, path);
    if (S_ISDIR(st.st_mode))
    {
        metadata.type = 'D';
    }
    else
    {
        metadata.type = 'F';
    }
    metadata.last_modified = st.st_mtime;
    return metadata;
}

// Funcția pentru salvarea metadatelor într-un fișier de snapshot
void save_snapshot(const char *dir_path, Metadata *metadata, int count) 
{
    char snapshot_path[256];
    sprintf(snapshot_path, "%s/Snapshot.txt", dir_path);
    FILE *f = fopen(snapshot_path, "w");
    if (f == NULL) 
    {
        perror("Eroare la deschiderea fișierului de snapshot");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "Snapshot pentru directorul: %s\n\n", dir_path);
    for (int i = 0; i < count; i++) 
    {
        fprintf(f, "%s\t%c\t%s", metadata[i].name, metadata[i].type, ctime(&metadata[i].last_modified));
    }
    fclose(f);
}

// Funcția pentru parcurgerea recursivă a directorului și crearea snapshot-ului
void create_snapshot(const char *dir_path) 
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL) 
    {
        perror("Eroare la deschiderea directorului");
        exit(EXIT_FAILURE);
    }
    
    Metadata metadata[100]; // Presupunem că directorul nu conține mai mult de 100 de fișiere sau subdirectoare
    int count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
        {
            char path[512]; // Am mărit dimensiunea bufferului pentru a încorpora calea completă
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name); // Am înlocuit sprintf cu snprintf pentru a evita posibilele probleme de buffer overflow
            metadata[count++] = create_metadata(path);
            if (metadata[count-1].type == 'D') {
                create_snapshot(path); // Apelăm recursiv funcția pentru subdirector
            }
        }
    }
    closedir(dir);

    save_snapshot(dir_path, metadata, count);
}



int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Utilizare: %s <director>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *dir_path = argv[1];
    struct stat st;
    if (stat(dir_path, &st) == -1) {
        perror("Eroare la verificarea directorului");
        exit(EXIT_FAILURE);
    }

    if (!S_ISDIR(st.st_mode)) {
        printf("%s nu este un director.\n", dir_path);
        exit(EXIT_FAILURE);
    }

    create_snapshot(dir_path);

    printf("Snapshot-ul a fost creat cu succes pentru directorul: %s\n", dir_path);

    return 0;
}
