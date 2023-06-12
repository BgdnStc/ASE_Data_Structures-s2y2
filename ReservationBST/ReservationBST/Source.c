#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 1024

struct Rezervare
{
	unsigned int id;
	char* denumire;
	unsigned char nrCamere;
	char* numeClient;
	float suma;
};
typedef struct Rezervare Rezervare;

struct BinarySearchTree
{
	Rezervare* data;
	struct BinarySearchTree* left;
	struct BinarySearchTree* right;
};
typedef struct BinarySearchTree BST;

void inOrderBST(BST* bst);
void deleteFullNode(BST** bst, BST** leftSubTree);
int* getIds(BST* bst, int id);
void freeRez(Rezervare* rezervare);
void deleteRoot(BST** bst);
int getHeight(BST* bst);
void totalCam(BST* bst, int* cnt);
void addBST(BST** bst, Rezervare* rezervare);
BST* createNode(Rezervare* rezervare);
Rezervare* creareRezervare(unsigned int id, char* denumire, unsigned char nrCamere, char* numeClient, float suma);
void printRezervare(Rezervare* rezervare);

int main()
{
	FILE* f = fopen("data.txt", "r");
	if (f == NULL) {
		return -1;
	}
	char buffer[ARRAY_SIZE];
	char* token = NULL;
	char delimitator[] = ",\n";

	BST* bst = NULL;

	unsigned int id;
	char denumire[ARRAY_SIZE];
	unsigned char nrCamere;
	char numeClient[ARRAY_SIZE];
	float suma;
	while (fgets(buffer, ARRAY_SIZE, f))
	{
		token = strtok(buffer, delimitator);
		strcpy(denumire, token);
		token = strtok(NULL, delimitator);
		strcpy(numeClient, token);
		memmove(numeClient, numeClient + 1, strlen(numeClient));
		token = strtok(NULL, delimitator);
		id = atoi(token);
		token = strtok(NULL, delimitator);
		nrCamere = atoi(token);
		token = strtok(NULL, delimitator);
		suma = atof(token);

		Rezervare* rezervare = creareRezervare(id, denumire, nrCamere, numeClient, suma);
		addBST(&bst, rezervare);
	}

	inOrderBST(bst);

	int totalCamere = 0;
	totalCam(bst, &totalCamere);
	printf("\nTotal camere: %d", totalCamere);

	int height = getHeight(bst);
	printf("\nHeight: %d", height);

	deleteRoot(&bst);
	printf("\nAfisare arbore dupa sergere radacina: ");
	inOrderBST(bst);

	int idx = 4;
	unsigned int* ids = getIds(bst, idx);
	printf("\nId-urile de la radacina pana la nodul %d:", idx);
	for (int i = 0; i < ARRAY_SIZE && ids[i] != 0; i++)
	{
		printf(" %d ", ids[i]);
	}
	printf("\n\n");
	
	free(ids);
	free(token);
	fclose(f);
}

void inOrderBST(BST* bst)
{
	if (bst)
	{
		inOrderBST(bst->left);
		printRezervare(bst->data);
		inOrderBST(bst->right);
	}
}

void deleteFullNode(BST** bst, BST** leftSubTree)
{
	if ((*leftSubTree)->right)
	{
		deleteFullNode(bst, &(*leftSubTree)->right);
	}
	else
	{
		freeRez((*bst)->data);
		(*bst)->data = (*leftSubTree)->data;
		BST* freeMe = *leftSubTree;
		*leftSubTree = (*leftSubTree)->left;
		free(freeMe);
	}
}

int* getIds(BST* bst, int id)
{
	int* ids = (int*)malloc(ARRAY_SIZE * sizeof(int));
	memset(ids, 0, ARRAY_SIZE * sizeof(int));
	int index = 0;

	while (bst)
	{
		ids[index++] = bst->data->id;
		if (id > bst->data->id)
		{
			bst = bst->right;
		}
		else if (id < bst->data->id)
		{
			bst = bst->left;
		}
		else if (id == bst->data->id)
		{
			break;
		}
	}
	return ids;
}

void freeRez(Rezervare* rezervare)
{
	free(rezervare->denumire);
	free(rezervare->numeClient);
}

void printRezervare(Rezervare* rezervare)
{
	printf("\nRezervare: #%d, denumire: %s, numeclient: %s, nrcamere: %d, suma: %.2f", rezervare->id, rezervare->denumire, rezervare->numeClient, rezervare->nrCamere, rezervare->suma);
}

void deleteRoot(BST** bst)
{
	if (bst)
	{
		if ((*bst)->left == NULL && (*bst)->right == NULL)
		{
			free((*bst)->data);
			free((*bst));
			*bst = NULL;
		}
		else if ((*bst)->left == NULL || (*bst)->right == NULL)
		{
			free((*bst)->data);
			free((*bst));
			*bst = (*bst)->right ? (*bst)->right : (*bst)->left;
		}
		else
		{
			deleteFullNode(bst, &(*bst)->left);
		}
	}
}

int getHeight(BST* bst)
{
	if (bst)
	{
		return 1 + max(getHeight(bst->left), getHeight(bst->right));
	}
	else
	{
		return 0;
	}
}

void totalCam(BST* bst, int* cnt)
{
	if (bst)
	{
		if (bst->left == NULL && bst->right == NULL)
		{
			*cnt += bst->data->nrCamere;
		}
		totalCam(bst->left, cnt);
		totalCam(bst->right, cnt);
	}
}

void addBST(BST** bst, Rezervare* rezervare)
{
	if (*bst == NULL)
	{
		*bst = createNode(rezervare);
	}
	else
	{
		if (rezervare->id > (*bst)->data->id)
		{
			addBST(&(*bst)->right, rezervare);
		}
		else if (rezervare->id < (*bst)->data->id)
		{
			addBST(&(*bst)->left, rezervare);
		}
		else
		{
			if ((*bst)->data)
			{
				free((*bst)->data);
			}
			(*bst)->data = rezervare;
		}
	}
}

BST* createNode(Rezervare* rezervare)
{
	BST* node = (BST*)malloc(sizeof(BST));
	node->left = NULL;
	node->right = NULL;
	node->data = rezervare;
	return node;
}

Rezervare* creareRezervare(unsigned int id, char* denumire, unsigned char nrCamere, char* numeClient, float suma)
{
	Rezervare* rezervare = (Rezervare*)malloc(sizeof(Rezervare));
	rezervare->id = id;
	rezervare->nrCamere = nrCamere;
	rezervare->suma = suma;
	rezervare->denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(rezervare->denumire, denumire);
	rezervare->numeClient = (char*)malloc(strlen(numeClient) + 1);
	strcpy(rezervare->numeClient, numeClient);
	return rezervare;
}