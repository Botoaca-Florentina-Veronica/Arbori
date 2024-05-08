#ifndef _GRAF_
#define _GRAF_

#define MAX_NR_NODURI 100

typedef struct TipNod {
  //Ambele sunt doar niste atribute ale nodului
  //Nu influenteaza implementarea functiilor
  int cheie;
  int info;
} Nod;

typedef struct TipArc {
    int idx_nod_1, idx_nod_2;
} Arc;

typedef struct Graf {
  int nr_noduri;
  Nod Noduri[MAX_NR_NODURI];
  int Arce[MAX_NR_NODURI][MAX_NR_NODURI];//matricea de adiacenta
} Graf;

void Initializare(Graf *g);
void InsertNod(Graf *g, Nod n);
void InsertArc(Graf *g, Arc a);
void SuprimaNod(Graf *g, int index_nod);
void SuprimaArc(Graf *g, Arc a);
void Afiseaza_Arce(Graf *g);

#endif 
