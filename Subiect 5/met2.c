#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ORDER 3 // Ordinul arborelui B

// Structura pentru nodul AVL
typedef struct AVLNode {
    int key;
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

// Structura pentru nodul B
typedef struct BNode {
    int n; // Numarul de chei
    int keys[ORDER - 1]; // Cheile
    struct BNode* children[ORDER]; // Copiii
    int leaf; // 1 daca este frunza, 0 altfel
} BNode;

// Functii pentru AVL
int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(AVLNode* node) {
    return (node == NULL) ? 0 : node->height;
}

AVLNode* createAVLNode(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(AVLNode* node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

AVLNode* insertAVL(AVLNode* node, int key) {
    if (node == NULL) {
        return createAVLNode(key);
    }

    if (key < node->key) {
        node->left = insertAVL(node->left, key);
    } else if (key > node->key) {
        node->right = insertAVL(node->right, key);
    } else {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key) {
        return rightRotate(node);
    }

    if (balance < -1 && key > node->right->key) {
        return leftRotate(node);
    }

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Functii pentru B
BNode* createBNode(int leaf) {
    BNode* node = (BNode*)malloc(sizeof(BNode));
    node->n = 0;
    node->leaf = leaf;
    for (int i = 0; i < ORDER; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void splitChild(BNode* x, int i, BNode* y) {
    BNode* z = createBNode(y->leaf);
    z->n = ORDER / 2;

    for (int j = 0; j < ORDER / 2; j++) {
        z->keys[j] = y->keys[j + ORDER / 2];
    }

    if (!y->leaf) {
        for (int j = 0; j <= ORDER / 2; j++) {
            z->children[j] = y->children[j + ORDER / 2];
        }
    }

    y->n = ORDER / 2 - 1;

    for (int j = x->n; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }

    x->children[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
    }

    x->keys[i] = y->keys[ORDER / 2 - 1];
    x->n++;
}

void insertNonFull(BNode* x, int k) {
    int i = x->n - 1;

    if (x->leaf) {
        while (i >= 0 && x->keys[i] > k) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->n++;
    } else {
        while (i >= 0 && x->keys[i] > k) {
            i--;
        }
        i++;
        if (x->children[i]->n == ORDER - 1) {
            splitChild(x, i, x->children[i]);
            if (x->keys[i] < k) {
                i++;
            }
        }
        insertNonFull(x->children[i], k);
    }
}

void insertB(BNode** root, int k) {
    if (*root == NULL) {
        *root = createBNode(1);
        (*root)->keys[0] = k;
        (*root)->n = 1;
    } else {
        if ((*root)->n == ORDER - 1) {
            BNode* s = createBNode(0);
            s->children[0] = *root;
            splitChild(s, 0, *root);
            int i = 0;
            if (s->keys[0] < k) {
                i++;
            }
            insertNonFull(s->children[i], k);
            *root = s;
        } else {
            insertNonFull(*root, k);
        }
    }
}

int BTreeHeight(BNode* node) {
    if (node == NULL) {
        return 0;
    } else if (node->leaf) {
        return 1;
    } else {
        return 1 + BTreeHeight(node->children[0]);
    }
}

// Funcții pentru eliberarea memoriei
void freeAVL(AVLNode* node) {
    if (node != NULL) {
        freeAVL(node->left);
        freeAVL(node->right);
        free(node);
    }
}

void freeB(BNode* node) {
    if (node != NULL) {
        for (int i = 0; i <= node->n; i++) {
            freeB(node->children[i]);
        }
        free(node);
    }
}

// Funcția principală pentru a compara înălțimile celor doi arbori
int main(void) {
    int i;
    AVLNode *radacinaAVL = NULL;
    BNode *radacinaB = NULL;

    int chei[] = {10, 20, 5, 6, 15, 30, 25, 50};
    int n = sizeof(chei) / sizeof(chei[0]);

    for (i = 0; i < n; i++) {
        radacinaAVL = insertAVL(radacinaAVL, chei[i]);
        insertB(&radacinaB, chei[i]);
    }

    int hAVL = height(radacinaAVL);
    int hB = BTreeHeight(radacinaB);

    printf("Inaltime AVL: %d\n", hAVL);
    printf("Inaltime B: %d\n", hB);
    printf("Diferenta raportata la numarul de chei: %f\n", (float)(hAVL - hB) / n);
    printf("Raportul celor doua inaltimi: %f\n", (float)hB / hAVL);

    freeAVL(radacinaAVL);
    freeB(radacinaB);

    return 0;
}


/*
   Arbore AVL:

        10
       /  \
      5    30
      \    / \
       6  20 50
          / \
         15  25
Inaltime:4
   

   Arbore B:
         20
      /     \
    10       30
    /  \     / \
[5,6]  15   25  50
Inaltime:3

*/
