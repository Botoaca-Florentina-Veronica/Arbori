#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 4 // Maximum degree of the B-tree

typedef struct BTreeNode
{
    int num_keys;           // Number of keys currently in the node
    int keys[M - 1];        // Array of keys
    BTreeNode *children[M]; // Array of child pointers
    bool is_leaf;           // True if node is a leaf
} BTreeNode;

// Function to create a new node
BTreeNode *createNode(bool is_leaf)
{
    int i;
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (newNode == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->num_keys = 0;
    newNode->is_leaf = is_leaf;
    for (i = 0; i < M; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

// Function to split a full child node
void splitChild(BTreeNode *parent, int index)
{
    BTreeNode *child = parent->children[index];
    BTreeNode *newNode = createNode(child->is_leaf);

    newNode->num_keys = M / 2 - 1;

    int i;
    // Move keys and children to the new node
    for (i = 0; i < M / 2 - 1; i++)
    {
        newNode->keys[i] = child->keys[i + M / 2];
    }

    if (!child->is_leaf)
    {
        for (i = 0; i < M / 2; i++)
        {
            newNode->children[i] = child->children[i + M / 2];
        }
    }

    child->num_keys = M / 2 - 1;

    // Shift parent's children to make space for the new node
    for (i = parent->num_keys; i > index; i--)
    {
        parent->children[i + 1] = parent->children[i];
    }

    parent->children[index + 1] = newNode;

    // Shift parent's keys to insert the middle key from the child
    for (i = parent->num_keys - 1; i >= index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    parent->keys[index] = child->keys[M / 2 - 1];
    parent->num_keys++;
}

// Function to insert a key into a non-full node
void insertNonFull(BTreeNode *node, int key)
{
    int i;
    i = node->num_keys - 1; // asignăm lui i, indexul ultimei chei din nod, și o vom folosi pentru a itera înapoi prin chei
    // De ce iterăm înapoi? Păi, vreau să trec doar prin valorile mai mari decât cheia mea, pentru ca mai apoi să
    // le shiftez, și să îmi introduc elementul în pagina. Nu are rost sa parcurg crescător ca să trec prin toate elementele
    // mai mici decât ce am eu nevoie

    if (node->is_leaf) // testăm dacă ne aflăm într-o pagină terminală
    {
        // Insert key into the sorted order
        while (i >= 0 && node->keys[i] > key)
        {
            // aici parcurgem cheile din nod până ajungem la cea mai mare
            // cand o găsim, vom shifta interativ spre dreapta(cu un index/unitate) acest element si cele ce se afla dupa el
            // fiindca știm că daca pagina nu e plină, mai există cel puțin un loc gol în ea
            node->keys[i + 1] = node->keys[i];
            // elementul din față va deveni cel curent
            i--; // Decrementăm i pentru a continua compararea cu cheia următoare
        }
        node->keys[i + 1] = key; // reținem în locul pe care doar ce l-am eliberat, valoarea noastră
        node->num_keys++;        // incrementăm numărul de chei din pagină
    }
    else
    {
        // Găsim copilul în care să inserăm cheia
        while (i >= 0 && node->keys[i] > key)
        {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == M - 1)
        {
            // Split child if it's full
            splitChild(node, i);

            // Determine which of the two children is the new one
            if (node->keys[i] < key)
            {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

// Function to insert a key into the B-tree
void insert(BTreeNode **root, int key)
{
    BTreeNode *node = *root;

    if (node == NULL)
    {
        // Create a new root node
        *root = createNode(true);
        (*root)->keys[0] = key;
        (*root)->num_keys = 1;
    }
    else
    {
        if (node->num_keys == M - 1)
        {
            // Split the root if it's full
            BTreeNode *new_root = createNode(false);
            new_root->children[0] = node;
            splitChild(new_root, 0);
            *root = new_root;
        }
        insertNonFull(*root, key);
    }
}

// Function to traverse and print the B-tree in-order
void traverse(BTreeNode *root)
{
    int i;
    if (root != NULL)
    {
        for (i = 0; i < root->num_keys; i++)
        {
            traverse(root->children[i]);
            printf("%d ", root->keys[i]);
        }
        traverse(root->children[i]);
    }
}

// Main function to test B-tree implementation
int main(void)
{
    BTreeNode *root = NULL;

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
