#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define ASIZE 1024

struct Project {
	unsigned int id;
	char* title;
	char* author;
	unsigned char noExecutants;
	float budget;
};
typedef struct Project Project;

struct DoubleLinkedList {
	Project* data;
	struct DoubleLinkedList* next;
	struct DoubleLinkedList* prev;
};
typedef struct DoubleLinkedList Node;

struct Heap {
	Project** projects;
	int currentIndex;
};
typedef struct Heap Heap;

Project* createProject(unsigned int, const char*, const char*, unsigned char, float);
void freeProject(Project*);
void printProject(Project*);
Node* createNode(Project*);
void addNode(Node**, Node*);
void printList(Node*);
int detProjects(Node*, int);
void updateList(Node*, const char*, float);
void sortList(Node*);
void freeList(Node**);
void convertToHeap(Heap*, Node*, int, const char*);
void reheapup(Heap*, int);
void printHeap(Heap*);
void freeHeap(Heap*);

void main()
{
	FILE* fp = fopen("projects.txt", "r");
	if (!fp) return -1;

	char buffer[ASIZE];
	char* tok;
	char del[] = ",\n";

	Node* list = NULL;
	Heap heap = { NULL, 0 };

	unsigned int id;
	char title[ASIZE];
	char author[ASIZE];
	unsigned char noExecutants;
	float budget;

	while (fgets(buffer, ASIZE, fp))
	{
		tok = strtok(buffer, del);
		id = atoi(tok);

		tok = strtok(NULL, del);
		strcpy(title, tok);
		memmove(title, title + 1, strlen(title) + 1);

		tok = strtok(NULL, del);
		strcpy(author, tok);
		memmove(author, author + 1, strlen(author) + 1);

		tok = strtok(NULL, del);
		noExecutants = atoi(tok);

		tok = strtok(NULL, del);
		budget = atof(tok);

		Project* project = createProject(id, title, author, noExecutants, budget);
		Node* node = createNode(project);
		addNode(&list, node);
	}

	// ex 1
	printList(list);

	// ex 2
	int minParticipants = 3;
	int noProjects = detProjects(list, minParticipants);
	printf("\n\nNumber of project with stricly more than %d participants: %d", minParticipants, noProjects);
	
	// ex 3
	float percentage = 0.50;
	char authorName[] = "Antonescu Robert";
	printf("\n\nUpdating the list for %s by increasing budget by %.0f%%:", authorName, percentage * 100);
	updateList(list, authorName, percentage);
	printList(list);

	// ex 4
	printf("\n\nSorting the List:");
	sortList(list);
	printList(list);

	// ex 5
	printf("\n\nConverting to heap:");
	int size = getSize(list);
	convertToHeap(&heap, list, size, authorName);
	printHeap(&heap);

	printf("\n\n\n");
	freeHeap(&heap);
	freeList(&list);
	fclose(fp);
}

void freeHeap(Heap* heap)
{
	for (int i = 0; i < heap->currentIndex; i++)
	{
		freeProject(heap->projects[i]);
	}
	free(heap->projects);
	heap->currentIndex = 0;
	heap = NULL;
}

void printHeap(Heap* heap)
{
	printf("\nPrinting heap:");
	for (int i = 0; i < heap->currentIndex; i++)
	{
		printProject(heap->projects[i]);
	}
}

void reheapup(Heap* heap, int idx)
{
	if (idx > 0)
	{
		int parentIdx = (idx - 1) / 2;
		if (heap->projects[parentIdx]->id < heap->projects[idx]->id)
		{
			Project* p = heap->projects[parentIdx];
			heap->projects[parentIdx] = heap->projects[idx];
			heap->projects[idx] = p;
			reheapup(heap, parentIdx);
		}
	}
}

void convertToHeap(Heap* heap, Node* list, int size, const char* author) {
	if (heap->projects == NULL)
	{
		heap->projects = (Project**)malloc(size * sizeof(Project*));
		heap->currentIndex = 0;
	}
	while (list)
	{
		if (strcmp(list->data->author, author) == 0)
		{
			Project* p = createProject(list->data->id, list->data->title, list->data->author, list->data->noExecutants, list->data->budget);
			heap->projects[heap->currentIndex] = p;
			reheapup(heap, heap->currentIndex);
			heap->currentIndex++;
		}
		list = list->next;
	}
}

int getSize(Node* list)
{
	int cnt = 0;
	while (list)
	{
		cnt++;
		list = list->next;
	}
	return cnt;
}

void sortList(Node* list)
{
	while (list)
	{
		Node* aux = list->next;

		while (aux)
		{
			if (list->data->budget > aux->data->budget)
			{
				Project* p = aux->data;
				aux->data = list->data;
				list->data = p;
			}
			aux = aux->next;
		}

		list = list->next;
	}
}

void updateList(Node* list, const char* author, float percentage)
{
	while (list)
	{
		if (strcmp(list->data->author, author) == 0)
			list->data->budget *= percentage + 1;
		list = list->next;
	}
}

int detProjects(Node* list, int min)
{
	int cnt = 0;

	while (list)
	{
		if (list->data->noExecutants > min)
			cnt++;
		list = list->next;
	}

	return cnt;
}

void freeList(Node** list)
{
	while (*list)
	{
		Node* freeMe = *list;
		*list = (*list)->next;
		freeProject(freeMe->data);
		free(freeMe);
	}
	*list = NULL;
}

void printList(Node* list)
{
	printf("\nPrinting list forwards:");
	while (list->next)
	{
		printProject(list->data);
		list = list->next;
	}
	printProject(list->data);
	printf("\nPrinting list backwards:");
	while (list)
	{
		printProject(list->data);
		list = list->prev;
	}
}

void addNode(Node** list, Node* node)
{
	if (*list == NULL)
		*list = node;
	else
	{
		node->next = *list;
		(*list)->prev = node;
		*list = node;
	}
}

Node* createNode(Project* p)
{
	Node* n = (Node*)malloc(sizeof(Node));
	n->next = n->prev = NULL;
	n->data = p;
	return n;
}

void printProject(Project* p)
{
	printf("\nProject: id: %d, title: %s, author: %s, noExecutants: %d, budget: %.2f",
		p->id, p->title, p->author, p->noExecutants, p->budget);
}

Project* createProject(unsigned int id, const char* title, const char* author, unsigned char noExecutants, float budget)
{
	Project* p = (Project*)malloc(sizeof(Project));
	p->id = id;
	p->noExecutants = noExecutants;
	p->budget = budget;

	p->title = (char*)malloc(strlen(title) + 1);
	strcpy(p->title, title);
	p->author = (char*)malloc(strlen(author) + 1);
	strcpy(p->author, author);
	return p;
}

void freeProject(Project* p)
{
	free(p->title);
	free(p->author);
}