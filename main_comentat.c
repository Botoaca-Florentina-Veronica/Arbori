#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// Definirea constantelor pentru dimensiunea buffer-ului și numărul maxim de cuvinte cheie
#define MAX_KEYWORDS 6
#define MAX_BUFFER_SIZE 1024

// Structura pentru metadatele unui fișier sau director
typedef struct {
    char name[256];         // Numele fișierului sau directorului
    char type;              // 'F' pentru fișier, 'D' pentru director
    time_t last_modified;   // Data ultimei modificări
    mode_t permissions;     // Permisiunile fișierului
    ino_t inode_number;     // Numărul inode al fișierului
    off_t size;             // Dimensiunea fișierului
    bool dangerous;         // Marcaj pentru fișierele periculoase
} Metadata;

// Prototipuri de funcții
void update_subdir_snapshots(const char *dir_path, const char *isolated_dir);
void save_global_snapshot(const char *dir_path, Metadata *metadata, int count);

// Funcția pentru crearea metadatelor unui fișier sau director
Metadata create_metadata(const char *path) 
{
    Metadata metadata;
    struct stat st;
    stat(path, &st);
    // Funcția stat() este apelată pentru a obține informații despre fișierul sau directorul specificat în calea dată
    // Informațiile sunt stocate în structura st
    strcpy(metadata.name, path);
    // Numele fișierului/directorului este copiat în câmpul name al obiectului metadata

    // Verificăm dacă fișierul este director
    DIR *dir = opendir(path);
    if (dir != NULL) 
    {
        // dacă deschiderea directorului a reușit, atunci calea specificată indică către un director
        closedir(dir);
        metadata.type = 'D';
        metadata.size = 0;  // Pentru directoare, dimensiunea va fi 0
        //un director conține doar referințe către fișierele și subdirectoarele conținute, și nu datele efective
        //astfel dimensiunea sa este întotdeauna 0
    } 
    else 
    {
        // daca a eșuat, atunci calea specificată indică către un fișier
        metadata.type = 'F';
        metadata.size = st.st_size; //este setat la dimensiunea fișierului, care este obținută din câmpul st.st_size al structurii st
    }

    //st_mtime, st_node, st_ino apartin structurii st

    metadata.last_modified = st.st_mtime;
    //Se copiază data ultimei modificări a fișierului/ directorului în câmpul last_modified al obiectului metadata
    metadata.permissions = st.st_mode;
    //Se copiază permisiunile fișierului/directorului în câmpul permissions
    metadata.inode_number = st.st_ino;
    //Se copiază numărul inode-ului fișierului/directorului în câmpul inode_number

    return metadata;
}

// Funcția pentru analiza sintactică a conținutului fișierului
bool analyze_file_syntax(const char *file_path) 
{
    int i;
    // Deschide fișierul în modul de citire
    FILE *file = fopen(file_path, "r");
    if (file == NULL) 
    {
        perror("Eroare la deschiderea fișierului");
        return false;
    }

    // Inițializăm variabile pentru numărul de linii, cuvinte și caractere
    int lines = 0, words = 0, characters = 0;
    char ch;
    bool has_non_ascii = false;

    // Parcurge fișierul caracter cu caracter
    while ((ch = fgetc(file)) != EOF) 
    {
        characters++;
        if (ch == '\n') 
        {
            lines++;
            words++;
        } 
        else if (isspace(ch)) 
        {
            words++;
        }
        if (!isascii(ch)) 
        {
            has_non_ascii = true;
            break;
        }
    }
    fclose(file);

    // Verifică cuvintele cheie și caracterele non-ASCII
    char *keywords[MAX_KEYWORDS] = {"corrupted", "dangerous", "risk", "attack", "malware", "malicious"};
    bool has_keyword = false;
    file = fopen(file_path, "r");
    if (file == NULL) 
    {
        perror("Eroare la deschiderea fișierului");
        return false;
    }

    char buffer[MAX_BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), file)) 
    {
        for (i = 0; i < MAX_KEYWORDS; i++) 
        {
            if (strstr(buffer, keywords[i]) != NULL) 
            {
                has_keyword = true;
                break;
            }
        }
        if (has_keyword) 
        {
            break;
        }
    }
    fclose(file);

    // Returnează true dacă fișierul are caracteristici periculoase
    return lines > 1000 || words > 10000 || characters > 100000 || has_keyword || has_non_ascii;
    // Returnează true dacă fișierul are mai mult de 1000 de linii, 10000 de cuvinte, 100000 de caractere, 
    //conține una dintre cuvintele cheie sau conține caractere non-ASCII, altfel returnează false.
}

// Funcția pentru salvarea metadatelor într-un fișier de snapshot
void save_snapshot(const char *dir_path, Metadata *metadata, int count) 
{
    int i;
    char snapshot_path[512];
    sprintf(snapshot_path, "%s/Snapshot.txt", dir_path);
    // După ce această linie de cod este executată, snapshot_path va conține concatenarea șirului dir_path
    //și a șirului "/Snapshot.txt". Astfel, dacă dir_path este "/path/to/directory", atunci snapshot_path va 
    //fi "/path/to/directory/Snapshot.txt".
    FILE *f = fopen(snapshot_path, "w");
    //deschidem fisierul in modul de scriere(w-write)
    if (f == NULL) 
    {
        perror("Eroare la deschiderea fișierului de snapshot");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "Snapshot pentru directorul: %s\n\n", dir_path);
    //fprintf face acelasi lucru ca printf, doar ca in loc sa afiseze textul formatat pe ecran
    //acesta il afiseaza in fisierul dat ca parametru in functie
    for (i = 0; i < count; i++) 
    {
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
        if (metadata[i].type == 'F') 
        {
            fprintf(f, "Dimensiune: %ld octeți\n", (long)metadata[i].size);
        }
        if (metadata[i].dangerous)
        {
            fprintf(f, "Fișier periculos!\n");
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
}

// Funcția pentru verificarea drepturilor lipsă ale fișierelor
bool has_missing_permissions(mode_t permissions) 
{
    // Verifică fiecare permisiune individual
    // Permisiunile necesare sunt: S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH
    
    // Verificăm dacă permisiunile necesare pentru utilizator lipsesc(user)
    if (!(permissions & S_IRUSR)) return true; // Permisiune de citire pentru proprietar
    if (!(permissions & S_IWUSR)) return true; // Permisiune de scriere pentru proprietar
    if (!(permissions & S_IXUSR)) return true; // Permisiune de executare pentru proprietar
    
    // Verificăm dacă permisiunile necesare pentru grup lipsesc(group)
    if (!(permissions & S_IRGRP)) return true; // Permisiune de citire pentru grup
    if (!(permissions & S_IWGRP)) return true; // Permisiune de scriere pentru grup
    if (!(permissions & S_IXGRP)) return true; // Permisiune de executare pentru grup
    
    // Verificăm dacă permisiunile necesare pentru alții lipsesc(others)
    if (!(permissions & S_IROTH)) return true; // Permisiune de citire pentru ceilalți
    if (!(permissions & S_IWOTH)) return true; // Permisiune de scriere pentru ceilalți
    if (!(permissions & S_IXOTH)) return true; // Permisiune de executare pentru ceilalți
    
    return false; // Toate permisiunile necesare sunt prezente
}

// Funcția pentru crearea snapshot-ului unui director
void create_snapshot(const char *dir_path, Metadata *metadata, int *count, const char *isolated_dir) 
{
    int i;
    DIR *dir = opendir(dir_path);
    if (dir == NULL) 
    {
        perror("Eroare la deschiderea directorului");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) 
    // Se parcurg fișierele și directoarele conținute în directorul curent
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
        // Se ignoră intrările speciale . și .., care reprezintă directorul curent și directorul părinte
        // si se procesează doar fișierele și directoarele reale conținute în directorul specificat
        {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            metadata[*count] = create_metadata(path); // Adăugăm metadatele la array
            (*count)++;

            // Apelăm recursiv funcția pentru subdirectoare
            if (entry->d_type == DT_DIR) 
            //Dacă elementul curent este un director:
            {
                create_snapshot(path, metadata, count, isolated_dir);
            }
            else if (entry->d_type == DT_REG) // DT_REG indică un fișier regulat
            {
                if (analyze_file_syntax(path)) 
                // Se verifică dacă fișierul este periculos
                {
                    metadata[*count - 1].dangerous = true; // Marcam fișierele periculoase
                }
            }
        }
    }
    closedir(dir);

    // Verificăm dacă avem permisiuni lipsă pentru fișiere
    for (i = 0; i < *count; i++) 
    {
        if (has_missing_permissions(metadata[i].permissions)) 
        {
            //retinem fisierele/directoarele cu permisiuni lipsa intr-un fisier dedicat pentru a tine o evidenta a acestora
            char log_path[512];
            sprintf(log_path, "%s/Log.txt", isolated_dir);
            FILE *f = fopen(log_path, "a");
            //"a" deschide fișierul pentru scriere. Dacă fișierul nu există, este creat. Dacă există, scrierea va începe de la sfârșitul fișierului, 
            //păstrând conținutul existent, și nu de la începutul său, cum ar fi în cazul modului "w" (care șterge conținutul existent).
            if (f == NULL) 
            {
                perror("Eroare la deschiderea fișierului de log");
                exit(EXIT_FAILURE);
            }
            fprintf(f, "Permisiuni lipsă pentru: %s\n", metadata[i].name);
            fclose(f);
        }
    }
    // Salvăm snapshot-ul directorului
    save_snapshot(dir_path, metadata, *count);
}

// Funcția pentru actualizarea snapshot-urilor subdirectoarelor
void update_subdir_snapshots(const char *dir_path, const char *isolated_dir) 
{
    int i;
    Metadata metadata[100];
    int count = 0;
    DIR *dir;
    struct dirent *entry;
    //se declară o variabilă(entry) pentru a stoca informațiile despre fișierele și subdirectoarele conținute în directorul curent.

    if ((dir = opendir(dir_path)) != NULL) 
    {
        while ((entry = readdir(dir)) != NULL) 
        // Se parcurg fișierele și directoarele conținute în directorul curent
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
            {
                continue; // Ignorăm directorul curent și directorul părinte
            }
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            Metadata file_data = create_metadata(path);

            // Dacă este un fișier, verificăm dacă este periculos
            if (file_data.type == 'F') 
            {
                if (strstr(file_data.name, ".c") != NULL || strstr(file_data.name, ".txt") != NULL) 
                {
                    FILE *f = fopen(path, "r");
                    if (f == NULL) 
                    {
                        perror("Eroare la deschiderea fișierului pentru analiză sintactică");
                        exit(EXIT_FAILURE);
                    }

                    int line_count = 0;
                    int word_count = 0;
                    int char_count = 0;
                    int non_ascii_count = 0;
                    char word[100];
                    char c;
                    bool dangerous = false;

                    while ((c = fgetc(f)) != EOF) 
                    {
                        char_count++;

                        // Verificăm dacă caracterul este non-ASCII
                        if ((unsigned char)c > 127) 
                        {
                            //caracterele ascii se opresc la 127
                            non_ascii_count++;
                        }

                        // Verificăm sfârșitul cuvântului sau liniei
                        if (isspace(c) || ispunct(c)) 
                        {
                            if (strlen(word) > 0) 
                            {
                                word_count++;
                                if (strcmp(word, "password") == 0 || strcmp(word, "virus") == 0 || strcmp(word, "malware") == 0) 
                                {
                                    dangerous = true;
                                }
                                strcpy(word, "");
                            }
                            if (c == '\n') 
                            {
                                line_count++;
                            }
                        } 
                        else 
                        {
                            strncat(word, &c, 1);
                        }
                    }

                    fclose(f);

                    printf("\n");
                    printf("Fișier analizat: %s\n", path);
                    printf("Număr de linii: %d\n", line_count);
                    printf("Număr de cuvinte: %d\n", word_count);
                    printf("Număr de caractere: %d\n", char_count);
                    printf("Număr de caractere non-ASCII: %d\n", non_ascii_count);
                    if (dangerous) 
                    {
                        printf("Fișier periculos găsit: %s\n", path);
                    }
                    file_data.dangerous = dangerous; // Adăugăm marcajul de fișier periculos
                }
            }
            metadata[count++] = file_data;
        }
        closedir(dir);
    } 
    else 
    {
        perror("Eroare la deschiderea directorului");
        exit(EXIT_FAILURE);
    }

    // Salvăm snapshot-ul pentru directorul curent
    save_snapshot(dir_path, metadata, count);

    // Apoi creăm snapshot-uri pentru subdirectoare
    for (i = 0; i < count; i++) 
    {
        if (metadata[i].type == 'D') // Verificăm dacă elementul curent din array este un director
        {
            update_subdir_snapshots(metadata[i].name, isolated_dir); // Apelăm recursiv funcția pentru a crea snapshot-uri pentru subdirectoare
        }
    }
}


// Funcția pentru crearea snapshot-ului global
void create_global_snapshot(const char *output_dir, int argc, char *argv[], const char *isolated_dir) 
{
    //output_dir - directorul în care să se salveze snapshot-ul global
    int i, j;
    Metadata *global_metadata = malloc(sizeof(Metadata) * 10000); // Creăm un spațiu mai mare pentru a încapacitate mai multe metadate
    int global_count = 0;
    for (i = 4; i < argc; i++) 
    {
        Metadata *dir_metadata = malloc(sizeof(Metadata) * 1000); // Creăm un spațiu pentru metadatele directorului curent
        int count = 0;
        create_snapshot(argv[i], dir_metadata, &count, isolated_dir); // Creăm snapshot-ul directorului curent
        for (j = 0; j < count; j++) 
        {
            global_metadata[global_count++] = dir_metadata[j]; // Adăugăm metadatele directorului curent la metadatele globale
        }
        free(dir_metadata);

        // Actualizăm snapshot-urile pentru subdirectoarele directorului curent și le adăugăm la snapshot-ul global
        update_subdir_snapshots(argv[i], isolated_dir);
        DIR *dir = opendir(argv[i]);
        if (dir != NULL) 
        {
            struct dirent *entry;
            while ((entry = readdir(dir)) != NULL) 
            {
                if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
                {
                    char sub_dir_path[512];
                    snprintf(sub_dir_path, sizeof(sub_dir_path), "%s/%s", argv[i], entry->d_name);
                    Metadata *subdir_metadata = malloc(sizeof(Metadata) * 1000);
                    int count = 0;
                    create_snapshot(sub_dir_path, subdir_metadata, &count, isolated_dir);
                    for (j = 0; j < count; j++) 
                    {
                        global_metadata[global_count++] = subdir_metadata[j];
                    }
                    free(subdir_metadata);
                }
            }
            closedir(dir);
        }
    }
    save_snapshot(output_dir, global_metadata, global_count); // Salvăm snapshot-ul global
    free(global_metadata);
}


// Funcția pentru salvarea snapshot-ului global într-un fișier
void save_global_snapshot(const char *dir_path, Metadata *metadata, int count) 
{
    int i;
    char snapshot_path[512];
    sprintf(snapshot_path, "%s/Snapshot.txt", dir_path);
    FILE *f = fopen(snapshot_path, "w");
    if (f == NULL) 
    {
        perror("Eroare la deschiderea fișierului de snapshot");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "Snapshot pentru directorul: %s\n\n", dir_path);
    for (i = 0; i < count; i++) 
    {
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
        if (metadata[i].type == 'F') 
        {
            fprintf(f, "Dimensiune: %ld octeți\n", (long)metadata[i].size);
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
}


int main(int argc, char *argv[]) 
{
    int i;
    if (argc < 6 || argc > 14) 
    {
        printf("Utilizare: %s -o <director_iesire> -s <director_izolare> <dir1> <dir2> ... <dir10>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *output_dir = NULL;
    char *isolated_dir = NULL;
    //parcurgem argumentele primite și identificăm directoarele de ieșire și izolare specificate cu opțiunile -o și -s
    for (i = 1; i < argc - 1; i++) 
    {
        if (strcmp(argv[i], "-o") == 0) 
        {
            output_dir = argv[i + 1];
        }
        if (strcmp(argv[i], "-s") == 0) 
        {
            isolated_dir = argv[i + 1];
        }
    }

    //daca nu sunt scrise corect directoarele de iesire/izolare fisiere periculoase
    if (output_dir == NULL || isolated_dir == NULL) 
    {
        printf("Opțiuni invalide pentru directorul de ieșire și/sau directorul izolat\n");
        printf("Utilizare: %s -o <director_iesire> -s <director_izolare> <dir1> <dir2> ... <dir10>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    mkdir(output_dir, 0777); // Creăm directorul de ieșire, dacă nu există deja
    mkdir(isolated_dir, 0777); // Creăm directorul izolat, dacă nu există deja

    // 0777 înseamnă că atât proprietarul, grupul și restul utilizatorilor au permisiuni
    // complete(pentru citire, scriere și execuție) asupra fișierului sau directorului

    //0 de la început înseamnă că acestea sunt simple permisiuni de acces, nu sunt suplimentate 
    //cu alte atribute speciale, cum ar fi suid, guid, sau sticky bit

    pid_t pid;
    int num_dirs = argc - 5; // numărul de directoare
    int child_status[num_dirs];
    //Inițializăm un vector de stări ale copiilor pentru a urmări starea de ieșire a fiecărui proces copil

    for (i = 3; i < argc; i++) 
    {
        pid = fork();
        //Creează un proces copil folosind fork()
        if (pid < 0) 
        {
            perror("Eroare la crearea procesului copil");
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) 
        {
            update_subdir_snapshots(argv[i], isolated_dir);
            // Creăm snapshot-ul pentru directorul dat în linia de comandă
            Metadata *dir_metadata = malloc(sizeof(Metadata) * 1000);
            int count = 0;
            create_snapshot(argv[i], dir_metadata, &count, isolated_dir);
            save_snapshot(argv[i], dir_metadata, count);
            free(dir_metadata);
            printf("\n\nSnapshot for Directory %s created successfully.\n", argv[i]);
            exit(EXIT_SUCCESS);
        }
    }

    // Așteptăm terminarea tuturor proceselor copil și salvăm starea lor în vectorul child_status
    for (i = 0; i < num_dirs; i++) 
    {
        waitpid(-1, &child_status[i], 0);
    }

    // Verificăm starea fiecărui proces copil
    for (i = 0; i < num_dirs; i++) 
    {
        if (WIFEXITED(child_status[i])) 
        {
            printf("Child Process %d terminated with exit code %d.\n", i + 1, WEXITSTATUS(child_status[i]));
        } 
        else 
        {
            printf("Child Process %d terminated abnormally.\n", i + 1);
        }
    }
    printf("\n");

    create_global_snapshot(output_dir, argc, argv, isolated_dir);

    printf("\nGlobal Snapshot created successfully.\n");
    printf("\nSnapshot-urile pentru toate directoarele au fost create cu succes în directorul de ieșire: %s\n", output_dir);
    return 0;
}
