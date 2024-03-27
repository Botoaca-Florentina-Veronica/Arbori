#include<time.h>
#define N   2
#define NN (N*2)

typedef int TipCheie;

struct Nod
{
	TipCheie cheie;
	struct Pagina* p;
	int contor;
};
typedef struct Nod Nod;

struct Pagina
{
	int m;
	struct Pagina* p0;
	Nod e[NN + 1];
};
typedef struct Pagina Pagina;
void citesteFisierBinar(const char *numeFisier, Nod *inregistrari, int numarInregistrari);
int numarAleatoriu(int min, int max);
void populeazaFisierBinar(const char *numeFisier, int numarInregistrari);
void cautareFisierBinar(const char *numeFisier, TipCheie cheieCautata, int *numarComparatii);
Pagina* initializareRadacina();
Pagina* insereaza(Pagina *pag, TipCheie x, Nod *nod);
void afisare(Pagina *arbore, int nivel);
int cautare(Pagina *pag, TipCheie x, int *numarComparatii);
Pagina* suprima(Pagina *pag, TipCheie x, Nod *nod);
void inordine(Pagina *pag);
void preordine(Pagina *pag);
void postordine(Pagina *pag);
