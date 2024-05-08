#include<stdio.h>
#include<stdlib.h>
#include"graf.h"

void Initializare(Graf *g)
{
  g->nr_noduri = 0;
}

void InsertNod(Graf *g, Nod n)
{
  //se adauga nodul n in graf(primul nod e pe pozitia 0)
  int index_nod = g->nr_noduri;
  g->Noduri[index_nod] = n; 
  g->nr_noduri++;
  //se initializeaza matricea de adiacenta pentru noul nod
  int i;
  for(i = 0; i < g->nr_noduri; i++)
  {
    g->Arce[index_nod][i] = g->Arce[i][index_nod] = 0;
  }  
}

void InsertArc(Graf *g, Arc a)
{
  g->Arce[a.idx_nod_1][a.idx_nod_2] = g->Arce[a.idx_nod_2][a.idx_nod_1] = 1;
}

void SuprimaNod(Graf *g, int index_nod)
{
  //nodul indicat este inlocuit cu ultimul nod
  int index_ultim = g->nr_noduri - 1;
  g->Noduri[index_nod] = g->Noduri[index_ultim];
  int i;
  for(i = 0; i < g->nr_noduri; i++)
  {
    g->Arce[i][index_nod] = g->Arce[i][index_ultim];
    g->Arce[index_nod][i] = g->Arce[index_ultim][i];
  }
    g->nr_noduri--;
}

void SuprimaArc(Graf *g, Arc a)
{
  g->Arce[a.idx_nod_1][a.idx_nod_2] = g->Arce[a.idx_nod_2][a.idx_nod_1] = 0;
}

void Afiseaza_Arce(Graf *g)
{
  int i,j;
  for(i = 0; i < g->nr_noduri; i++)
  {
    for(j = 0; j < g->nr_noduri; j++)
    {
        printf("%d ", g->Arce[i][j]);
    }
    printf("\n");
  }
}
