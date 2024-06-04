/*
  Se dau un arbore AVL si unul B de ordinul n=1, cu aceeasi secventa de chei.
  Sa se compare inaltimile celor 2 arbori si sa se afiseze diferenta lor raportata la numarul de chei, 
precum si raportul celor doua inaltimi in mod cat mai eficient.
*/

#include <stdio.h>
#include <stdlib.h>

// Implementarea arborelui AVL
typedef int TCheie;
typedef struct TNodAVL {
    TCheie cheie;
    int ech;
    struct TNodAVL *stg, *dr;
} TNodAVL;

TNodAVL* adauga(TNodAVL *r, TCheie k);
TNodAVL* insertEchilibrat(TCheie x, TNodAVL *p, int *h);
int inaltimeAVL(TNodAVL *r);
void distrugeAVL(TNodAVL *r);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define TRUE 1
#define FALSE 0

TNodAVL* insertEchilibrat(TCheie x, TNodAVL *p, int *h)
{
    TNodAVL *p1, *p2;

    if(!p) //cheia nu e in arbore; se insereaza
    {
        p = malloc(sizeof(TNodAVL));
        p->cheie = x;
        p->ech = 0; //nodul nou inserat creaza un subarbore unitar, care este mereu echilibrat
        p->stg = p->dr = NULL; //noul nod este frunza
        *h = TRUE;
        return p;
    }

    if (x < p->cheie)
    {
        //daca cheia de inserat < cheia curenta,
        //se insereaza in subarborele stang
        p->stg = insertEchilibrat(x, p->stg, h);
        if (*h == TRUE) //ramura stinga a crescut in inaltime
        {
            switch (p->ech)
            {
                case 1:          //subarborele era dezechilibrat in dreapta
                    p->ech = 0;  //adaugare in subarborele stang => echilibru
                    *h = FALSE;
                    break;
                case 0:          //subarborele era in echilibru
                    p->ech = -1; //dezechilibrat cu un nivel in stanga
                    break;
                case -1: //subarborele era dezechilibrat in stanga cu un nivel,
                         //acum e cu doua nivele => reechilibrare
                    p1 = p->stg;
                    if (p1->ech == -1) //cazul 1 stanga, dezechilibru la fiu in stanga
                    {
                        /*Se efectueaza o rotatie la dreapta a subarborelui p1, astfel:
                           - fiul drept al nodului p1 este "decuplat" de arbore;
                           - nodul p devine fiul drept al nodului p1;
                           - nodul "decuplat" devine fiul stang al nodului p.
                        */
                        p->stg = p1->dr;
                        p1->dr = p;
                        p->ech = 0;
                        p = p1;
                    }
                    else // cazul 2 stanga, dezechilibru la fiu in dreapta
                    {
                        /*Se efectueaza o rotatie la stanga a subarborelui p2, astfel:
                           - fiul stanga al nodului p2 este "decuplat" de arbore;
                           - nodul p2->stg devine fiul drept al nodului p1;
                           - nodul p1 devine fiul stang al nodului "decuplat";
                           - nodul "decuplat" p2 devine fiul stang al nodului p.

                           Se efectueaza o rotatie la dreapta a subarborelui p2, astfel:
                           - fiul stanga al nodului p (p2) este "decuplat" de arbore;
                           - nodul p2->dr devine fiul stang al nodului p;
                           - nodul p devine fiul drept al nodului "decuplat" p2.
                        */
                        p2 = p1->dr;
                        p1->dr = p2->stg;
                        p2->stg = p1;
                        p->stg = p2->dr;
                        p2->dr = p;

                        if (p2->ech == -1)
                            p->ech = +1;
                        else
                            p->ech = 0;

                        if (p2->ech == +1)
                            p1->ech = -1;
                        else
                            p1->ech = 0;

                        p = p2;
                    } // caz 2 stinga
                    p->ech = 0;
                    *h = FALSE; //nu s-a schimbat diferenta de nivel
                    break;
            }// switch
        }
    }
    else
        if (x > p->cheie)
        {
            p->dr = insertEchilibrat(x, (TNodAVL *)p->dr, h);
            if (*h) //ramura dreapta a crescut in inaltime
                switch (p->ech)
                {
                    case -1:        //subarborele era dezechilibrat in stanga
                        p->ech = 0; //adaugare in subarborele drept => echilibru
                        *h = FALSE;
                        break;
                    case 0:          //subarborele era in echilibru
                        p->ech = +1; //dezechilibrat cu un nivel in stanga
                        break;
                    case +1: //subarborele era dezechilibrat in dreapta cu un nivel,
                             //acum e cu doua nivele => reechilibrare
                        p1 = p->dr;
                        if (p1->ech == 1) // cazul 1 dreapta
                        {
                            /*Se efectueaza o rotatie la stanga a subarborelui p1, astfel:
                               - nodul p1->stg este "decuplat" de arbore;
                               - nodul p devine fiul stang al nodului p1;
                               - nodul "decuplat" devine fiul drept al nodului p.
                            */
                            p->dr = p1->stg;
                            p1->stg = p;
                            p->ech = 0;
                            p = p1;
                        }
                        else // cazul 2 dreapta
                        {
                            /*Se efectueaza o rotatie la dreapta a subarborelui p2, astfel:
                               - nodul p2->dr este "decuplat" de arbore;
                               - nodul p1 devine fiul drept al nodului p2;
                               - nodul p2 devine fiul drept al nodului p
                               - nodul "decuplat" devine fiul stang al nodului p1.

                              Se efectueaza o rotatie la stanga a subarborelui p2, astfel:
                               - nodul p2->stg este "decuplat" de arbore;
                               - nodul p devine fiul stang al nodului p2;
                               - nodul "decuplat" devine fiul drept al nodului p.
                            */
                            p2 = p1->stg;
                            p1->stg = p2->dr;
                            p2->dr = p1;
                            p->dr = p2->stg;
                            p2->stg = p;

                            if (p2->ech == 1)
                                p->ech = -1;
                            else
                                p->ech = 0;

                            if (p2->ech == -1)
                                p1->ech = +1;
                            else
                                p1->ech = 0;

                            p = p2;
                        } // caz 2 dreapta
                        p->ech = 0;
                        *h = FALSE;
                } // switch
        }
    return p;
}

TNodAVL* adauga(TNodAVL *r, TCheie k) {
    int h = FALSE;
    return insertEchilibrat(k, r, &h);
}

int inaltimeAVL(TNodAVL *r) {
    if (!r) return 0;
    return MAX(inaltimeAVL(r->stg), inaltimeAVL(r->dr)) + 1;
}

void distrugeAVL(TNodAVL *r) {
    if (!r) return;
    distrugeAVL(r->stg);
    distrugeAVL(r->dr);
    free(r);
}

// Implementarea arborelui B
#define N 1
#define NN (2 * N)

typedef int TipCheie;
typedef struct Nod {
    TipCheie cheie;
    int contor;
    struct Pagina *p;
} Nod;

typedef struct Pagina {
    int m;
    Nod e[NN + 2];
    struct Pagina *p0;
} Pagina;

Pagina* insereaza(Pagina *pag, TipCheie x, Nod *nod);
int inaltimeB(Pagina *pag);
void distrugeB(Pagina *pag);
Nod v;

Pagina* insereaza(Pagina *pag, TipCheie x, Nod *nod)
{
	int i, s, d, mij;
	Pagina *q, *r;

	if (!nod)
	{
		v.cheie = x;
		v.contor = 1;
		v.p = NULL;
	}
	else
		v = *nod;

	if (pag == NULL) //daca nu exista radacina
	{
		pag = (Pagina*)calloc(sizeof(Pagina), 1);
		pag->e[++pag->m] = v;

		return pag;
	}

	//se cauta binar pozitia cheii x in pagina curenta
	s = 1;
	d = pag->m;
	while (s <= d)
	{
		mij = (s + d) / 2;
		if (pag->e[mij].cheie == x) //gasit
		{
			pag->e[mij].contor++;
			return pag;
		}
		if (x < pag->e[mij].cheie)
			d = mij - 1;
		else
			s = mij + 1;
	}

	//daca este pagina terminala sau daca se propaga insertia
	if (pag->p0 == NULL || nod)
	{
		if (pag->m < NN) //se poate adauga un nod in pagina curenta
		{
			++pag->m;
			for (i = pag->m; i > d + 1; i--)
				pag->e[i] = pag->e[i - 1];
			pag->e[i] = v;
		}
		//pagina curenta pag este plina => supradepasire => pagina trebuie scindata
		else
		{
			Pagina *a = pag;
			Pagina *b = (Pagina*)calloc(sizeof(Pagina), 1);
			pag = (Pagina*)calloc(sizeof(Pagina), 1);

			//scindarea paginii curente
			for (i = 1; i <= N; i++)
				b->e[i] = a->e[i + N];
			a->m = b->m = N;

			//cheia noudului v e cheia de scindare
			if (v.cheie > a->e[N].cheie && v.cheie < b->e[1].cheie)
				pag->e[++pag->m] = v;
			else
			{
				//ultima cheie a nodului a este folosita la scindare
				if (v.cheie < a->e[N].cheie)
				{
					pag->e[++pag->m] = a->e[N];
					for (i = a->m; i > 1 && a->e[i - 1].cheie > v.cheie; i--)
						a->e[i] = a->e[i - 1];
					a->e[i] = v;
				}
				//prima cheie a nodului a este folosita la scindare
				else
				{
					pag->e[++pag->m] = b->e[1];
					for (i = 1; i < b->m && b->e[i + 1].cheie < v.cheie; i++)
						b->e[i] = b->e[i + 1];
					b->e[i] = v;
				}
			}

			//se refac legaturile intre pagini
			b->p0 = pag->e[1].p; //prima legatura a paginii b devine legatura nodului parinte
			pag->p0 = a;         //prima legatura a nodului parinte devine fostul vecin stanga
			pag->e[1].p = b;     //a doua legatura a nodului parinte devine fostul vecin dreapta
		}
	}
	else
	{
		if (d == 0) //s-a ajuns la cel mai din stanga element => prima legatura
			q = pag->p0;
		else
			q = pag->e[d].p;
		r = insereaza(q, x, NULL);
		if (r != q) //daca pagina in care s-a inserat s-a scindat <=> arborele crescut cu un nivel
		{
			/*se incearca inserarea nodului din pagina scindata in pagina curenta
			in caz de succes, arborele nu va creste in inaltime*/
			pag = insereaza(pag, r->e[1].cheie, &r->e[1]);
			free(r); //se sterge pagina scindata, intrucat nodul a fost inserat intr-o alta pagina
		}
	}

	return pag;
}

int inaltimeB(Pagina *pag) {
    if (!pag) return 0;
    int maxInaltime = 0;
    for (int i = 0; i <= pag->m; i++) {
        int h = (i == 0) ? inaltimeB(pag->p0) : inaltimeB(pag->e[i].p);
        if (h > maxInaltime) maxInaltime = h;
    }
    return maxInaltime + 1;
}

void distrugeB(Pagina *pag) {
    if (!pag) return;
    distrugeB(pag->p0);
    for (int i = 1; i <= pag->m; i++)
        distrugeB(pag->e[i].p);
    free(pag);
}

// Funcția principală pentru a compara înălțimile celor doi arbori
int main(void) 
{
    int i;
    TNodAVL *radacinaAVL = NULL;
    Pagina *radacinaB = NULL;
    
    int chei[] = {10, 20, 5, 6, 15, 30, 25, 50};
    int n = sizeof(chei) / sizeof(chei[0]);
    
    for (i = 0; i < n; i++) {
        radacinaAVL = adauga(radacinaAVL, chei[i]);
        radacinaB = insereaza(radacinaB, chei[i], NULL);
    }
    
    int hAVL = inaltimeAVL(radacinaAVL);
    int hB = inaltimeB(radacinaB);
    
    printf("Inaltime AVL: %d\n", hAVL);
    printf("Inaltime B: %d\n", hB);
    printf("Diferenta raportata la numarul de chei: %f\n", (float)(hAVL - hB) / n);
    printf("Raportul celor doua inaltimi: %f\n", (float)hB / hAVL);
    
    distrugeAVL(radacinaAVL);
    distrugeB(radacinaB);
    
    return 0;
}
