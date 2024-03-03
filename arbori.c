/*
   Se cere sa se implementeze folosind tablouri de indicatori spre parinte un arbore generalizat continand chei intregi, impreuna cu urmatorii operatori:

void insereaza_radacina( int cheie_radacina ) - distruge arborele generalizat daca acesta exista si creeaza unul nou avand "cheie_radacina" pe post de radacina;
void insereaza_cheie( int cheie , int cheie_parinte ) - insereaza in arbore cheia "cheie" ca si cheie fiu a cheii "cheie_parinte";
int cauta_cheie( int cheie ) - cauta cheia "cheie" in arbore; returneaza 1 in caz ca a fost gasita cheia, sau 0 in caz contrar;
int sterge_cheie( int cheie ) - cauta cheia "cheie" in arbore si o sterge; returneaza 1 in caz ca a fost gasita si stearsa cheia, sau 0 in caz contrar;
void preordine() - parcurge arborele in preordine, afisand cheile intalnite in aceasta parcurgere;
void inordine() - parcurge arborele in inordine, afisand cheile intalnite in aceasta parcurgere;
void postordine() - parcurge arborele in postordine, afisand cheile intalnite in aceasta parcurgere;
int inaltime_arbore() - returneaza inaltimea arborelui generalizat;
int grad_arbore() - returneaza gradul arborelui generalizat;
int cel_mai_din_stanga_frate( int cheie ) - returneaza cheia celui mai din stanga frate al cheii "cheie" sau -1 daca acesta nu exista;
int cel_mai_din_dreapta_frate( int cheie ) - returneaza cheia celui mai din dreapta frate al cheii "cheie" sau -1 daca acesta nu exista;
int numar_frati( int cheie ) - returneaza numarul de frati ai cheii "cheie".
*/

#include<stdio.h>
#include<stdlib.h>

#define MAX 100

int parinte[MAX];
int frate_dreapta[MAX];
int frate_stanga[MAX];  //sau primul fiu


//functie pentru initializarea arborelui generalizat:
void initializare()
{
	int i;
	for(i=0; i<MAX; i++)
	{
		//initial toate nodurile sunt neatașate/goale
		parinte[i] = -1;
		frate_dreapta[i] = -1;
		frate_stanga[i] = -1;
	}
}

//inserare radacină nouă în arbore și stergerea arborelui vechi
void insereaza_radacina(int cheie_radacina)
{
	//pentru a sterge arborele precedent, nu fac decat să îl reinițializez
	//folosind funcția creata mai sus
	initializare();
	parinte[cheie_radacina] = -1;
	//noua radăcină nu are părinte, fiindca doar ce am șters tot arborele, de aia 
	//îi atașez valoarea -1
}

void insereaza_cheie(int cheie, int cheie_parinte)
{
	//setăm părintele cu parametrii dați
	//adică dacă am rădăcina 0, iar parametrii sunt (1, 0), eu voi seta
	//parintele lui 1(adica noul fiu pe care vreau sa il inserez la arbore) ca fiind 0
	parinte[cheie] = cheie_parinte;
	frate_dreapta[cheie] = -1;  //nu are frati la inceput

	//acum verific dacă părintele nu are încă niciun fiu
	if(frate_stanga[cheie_parinte] == -1)
	{
		//inseamna ca imi pot adauga linistita, noul fiu
		//pentru ca in partea stanga se adauga primul fiu al unui parinte
		frate_stanga[cheie_parinte] = cheie;
	}
	else
	{
		//daca are frati, atunci voi parcurge toti fii din dreapta ai parintelui
		//iar cand ajung la ultimul, il inserez si pe noul fiu
		int frate;
		frate = frate_stanga[cheie_parinte];
		while(frate_dreapta[frate] != -1)
		{
			frate = frate_dreapta[frate];
		}
		frate_dreapta[frate] = cheie;
	}
}

//caută o cheie în arbore
void cauta_cheie(int cheie)
{
	//daca are părinte, atunci cheia există deja în arbore
	if(parinte[cheie] != -1)
	{
		return parinte[cheie];
	}
}

//parcurgere arbore in preordine
void preordine_util(int node)
{
	if(node != -1)
	{
		printf("%d", node);
		preordine_util(frate_stanga[node]);
		preordine_util(frate_dreapta[node]);
	}
}

void preordine()
{
	preordine_util(frate_stanga[0]);
	printf("\n");
}

//parcurgere arbore în inordine
void inordine_util(int node)
{
	inordine_util(frate_stanga[node]);
	printf("%d", node);
	inordine_util(frate_dreapta[node]);
}

void inordine()
{
	inordine_util(frate_stanga[0]);
	printf("\n");
}

//parcurgere arbore în postordine
void postordine_util(int node)
{
	postordine_util(frate_stanga[node]);
	postordine_util(frate_dreapta[node]);
	printf("%d", node);
}

void postordine()
{
	postordine_util(frate_stanga[0]);
	printf("\n");
}

int inaltime_arbore()
{
	int max_height, i;
	max_height = 0;
	for(i=0; i<MAX; i++)
	{
		int height = 0;
		int curr = i;
		while(parinte[curr] != -1)
		{
			height++;
			curr = parinte[curr];
		}
		if(height > max_height)
		{
			max_height = height;
		}
	}
	return max_height;
}

//functia returnează numărul de frați al unei chei
int numar_frati(int cheie)
{
	int count, i, cheie_parinte, sibling;
	count = 0;
	cheie_parinte = parinte[cheie];
	sibling = frate_stanga[cheie_parinte]; 

	while(sibling != -1)
	{
		//cat timp primul frate exista, putem trece mai departe cu parcurgerea
		count++;
		sibling = frate_dreapta[sibling]; //aici actualizam ca primul frate sa devina al doilea
		//pentru a putea parcurge pe rand toti fratii
	}
	return count;
}

int main(void)
{
	return 0;
}
