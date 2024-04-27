/*
Project SO

----S6----
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


----S7----


S7
 Functionalitatea programului va fi actualizată pentru a permite primirea unui număr nespecificat de argumente (directoare) în linia de comandă (dar nu mai mult de 10), iar fiecare argument este diferit. 
Logica pentru capturarea metadatelor va fi acum aplicată tuturor argumentelor primite, ceea ce înseamnă că programul va actualiza snapshot-urile pentru toate directoarele specificate de utilizator. 
De exemplu:

1./program_exe dir1 dir2 dir3 dir4
  
  Pentru fiecare intrare din directoarele furnizat ca argumente, utilizatorul va putea compara snapshot-ul anterior al directorului specificat cu cel actual.  
  Dacă există diferențe între cele două snapshot-uri, captura veche va fi actualizată cu noile informații din snapshot-ul curent.

 // Fișierul instantaneu vechi pentru File1.txt
 Marca temporală: 2023-03-15 10:30:00
 Intrare: File1.txt
 Dimensiune: 1024 octeți
 Ultima modificare: 2023-03-14 08:45:00
 Permisiuni: rw-r--r--
 Număr Inode: 12345


 // Fişierul instantaneu nou pentru File1.txt
 Marca temporală: 2023-03-16 11:00:00
 Intrare: File1.txt
 Dimensiune: 1024 octeți
 Ultima modificare: 2023-03-16 09:15:00
 Permisiuni: rw-r--r-x
 Număr Inode: 12345


În exemplele de mai sus, avem o reprezentare posibilă a unui snapshot vechi şi curent (nou) pentru File1.txt. Având în vedere că noul snapshot este diferit de cel anterior, cel vechi va fi suprascris.
• Funcţionalitatea codului va fi extinsă astfel încât programul să primească un argument suplimentar, reprezentând directorul de ieşire în care vor fi stocate toate snapshot-urile intrărilor din directoarele specificate în linia de comandă. 
Acest director de ieşire va fi specificat folosind opțiunea -o.

De exemplu, comanda pentru a rula programul va fi:

1 ./program_exe -o director_iesire dir1 dir2 dir3 dir4

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

// Structura pentru metadatele unui fișier sau director
typedef struct {
    char name[256];         // Numele fișierului sau directorului
    char type;              // 'F' pentru fișier, 'D' pentru director
    time_t last_modified;   // Data ultimei modificări
    mode_t permissions;     // Permisiunile fișierului
    ino_t inode_number;     // Numărul inode al fișierului
    off_t size;             // Dimensiunea fișierului
} Metadata;

// Funcția pentru crearea metadatelor unui fișier sau director
Metadata create_metadata(const char *path) {
    Metadata metadata;
    struct stat st;
    stat(path, &st);
    strcpy(metadata.name, path);

    // Verificăm dacă fișierul este director
    DIR *dir = opendir(path);
    if (dir != NULL) {
        closedir(dir);
        metadata.type = 'D';
    } else {
        metadata.type = 'F';
    }

    metadata.last_modified = st.st_mtime;
    metadata.permissions = st.st_mode;
    metadata.inode_number = st.st_ino;
    metadata.size = st.st_size;

    return metadata;
}

// Funcția pentru salvarea metadatelor într-un fișier de snapshot
void save_snapshot(const char *dir_path, Metadata *metadata, int count) {
    char snapshot_path[256];
    sprintf(snapshot_path, "%s/Snapshot.txt", dir_path);
    FILE *f = fopen(snapshot_path, "w");
    if (f == NULL) {
        perror("Eroare la deschiderea fișierului de snapshot");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "Snapshot pentru directorul: %s\n\n", dir_path);
    for (int i = 0; i < count; i++) {
        fprintf(f, "Metadate pentru: %s\n", metadata[i].name);
        fprintf(f, "Tip: %c\n", metadata[i].type);
        fprintf(f, "Ultima modificare: %s", ctime(&metadata[i].last_modified));
        fprintf(f, "Permisiuni: ");
        fprintf(f, (metadata[i].permissions & S_IRUSR) ? "r" : "-");
        fprintf(f, (metadata[i].permissions & S_IWUSR) ? "w" : "-");
        fprintf(f, (metadata[i].permissions & S_IXUSR) ? "x" : "-");
        fprintf(f, (metadata[i].permissions & S_IRGRP) ? "r" : "-");
        fprintf(f, (metadata[i].permissions & S_IWGRP) ? "w" : "-");
        fprintf(f, (metadata[i].permissions & S_IXGRP) ? "x" : "-");
        fprintf(f, (metadata[i].permissions & S_IROTH) ? "r" : "-");
        fprintf(f, (metadata[i].permissions & S_IWOTH) ? "w" : "-");
        fprintf(f, (metadata[i].permissions & S_IXOTH) ? "x" : "-");
        fprintf(f, "\n");
        fprintf(f, "Număr Inode: %ld\n", (long)metadata[i].inode_number);
        if (metadata[i].type == 'F') {
            fprintf(f, "Dimensiune: %ld octeți\n", (long)metadata[i].size);
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
}

// Funcția pentru parcurgerea recursivă a directorului și crearea snapshot-ului
void create_snapshot(const char *dir_path, Metadata *metadata, int *count) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Eroare la deschiderea directorului");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[512]; // Am mărit dimensiunea bufferului pentru a încorpora calea completă
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name); // Am înlocuit sprintf cu snprintf pentru a evita posibilele probleme de buffer overflow
            
            // Verificăm dacă este un director
            DIR *subdir = opendir(path);
            if (subdir != NULL) {
                closedir(subdir);
                metadata[*count] = create_metadata(path);
                (*count)++;
                create_snapshot(path, metadata, count); // Apelăm recursiv funcția pentru subdirector
            } else {
                metadata[*count] = create_metadata(path);
                (*count)++;
            }
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 5 || argc > 13) {
        printf("Utilizare: %s -o <director_iesire> <dir1> <dir2> ... <dir10>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *output_dir = NULL;
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            output_dir = argv[i + 1];
            break;
        }
    }

    if (output_dir == NULL) {
        printf("Opțiune invalidă pentru directorul de ieșire\n");
        printf("Utilizare: %s -o <director_iesire> <dir1> <dir2> ... <dir10>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mkdir(output_dir, 0777); // Creăm directorul de ieșire, dacă nu există deja
    
    Metadata *metadata = malloc(sizeof(Metadata) * 1000); // Alocare dinamică a memoriei
    if (metadata == NULL) {
        perror("Eroare la alocarea memoriei");
        exit(EXIT_FAILURE);
    }
    
    int count = 0;
    for (int i = 3; i < argc; i++) {
        char *dir_name = strrchr(argv[i], '/');
        if (dir_name == NULL) {
            dir_name = argv[i];
        } else {
            dir_name++; // Ignorăm caracterul '/'
        }
        create_snapshot(argv[i], metadata, &count);
        save_snapshot(argv[i], metadata, count);
        count = 0; // Resetăm numărul de metadate pentru următorul director
    }
    
    // Creăm fișierul de snapshot global
    char global_snapshot_path[256];
    sprintf(global_snapshot_path, "%s/Global_Snapshot.txt", output_dir);
    FILE *global_snapshot = fopen(global_snapshot_path, "w");
    if (global_snapshot == NULL) {
        perror("Eroare la deschiderea fișierului de snapshot global");
        exit(EXIT_FAILURE);
    }
    
    fprintf(global_snapshot, "Snapshot pentru toate directoarele:\n\n");
    for (int i = 3; i < argc; i++) {
        char *dir_name = strrchr(argv[i], '/');
        if (dir_name == NULL) {
            dir_name = argv[i];
        } else {
            dir_name++; // Ignorăm caracterul '/'
        }
        char snapshot_path[256];
        sprintf(snapshot_path, "%s/Snapshot.txt", argv[i]);
        FILE *snapshot = fopen(snapshot_path, "r");
        if (snapshot == NULL) {
            perror("Eroare la deschiderea fișierului de snapshot");
            exit(EXIT_FAILURE);
        }
        
        char line[256];
        fprintf(global_snapshot, "Snapshot pentru directorul: %s\n\n", dir_name);
        while (fgets(line, sizeof(line), snapshot) != NULL) {
            fprintf(global_snapshot, "%s", line);
        }
        fprintf(global_snapshot, "\n");
        fclose(snapshot);
    }
    fclose(global_snapshot);
    
    free(metadata); // Eliberăm memoria alocată dinamic

    printf("Snapshot-urile pentru toate directoarele au fost create cu succes în directorul de ieșire: %s\n", output_dir);
    return 0;
}

