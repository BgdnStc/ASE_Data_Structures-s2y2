#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define HashTable_size 20

struct Room
{
	unsigned short floor;
	unsigned short roomNo;
	char* roomType;
	double price;
};
typedef struct Room Room;

struct SimpleLinkedList
{
	Room* data;
	struct SimpleLinkedList* next;
};
typedef struct SimpleLinkedList Node;

struct HashTable
{
	Node** buckets;
	int size;
};
typedef struct HashTable HashTable;

struct DoubleLinkedList
{
	Room* data;
	struct DoubleLinkedList* prev;
	struct DoubleLinkedList* next;
};
typedef struct DoubleLinkedList DNode;

Room* createRoom(unsigned short floor, unsigned short roomNo, char* roomType, double price);
Node* createNode(Room* room);
DNode* creadeDNode(Room* data);
int fhash(unsigned short key, int size);
void putHT(HashTable* ht, Node* node);
void putNode(HashTable* ht, Node* node, int index);
int countRoomsFloor(HashTable* ht, int floor);
DNode* higherPriceDLL(HashTable* ht, double price);
void deleteRoomType(DNode** list, char* roomType);
void printRoom(Room* room);
void printDLL(DNode* list);
void printHT(HashTable ht);

int main()
{
	FILE* f = fopen("rooms.txt", "r");
	if (!f)
	{
		printf("\nFile not found");
		return -1;
	}
	char buffer[256];
	char delimiter[] = ",\n";
	unsigned short floor;
	unsigned short roomNo;
	char* roomType;
	double price;

	HashTable ht = { NULL, 0 };

	while (fgets(buffer, 256, f))
	{
		char* token = strtok(buffer, delimiter);
		roomType = (char*)malloc(strlen(token) + 1);
		strcpy(roomType, token);
		token = strtok(NULL, delimiter);
		floor = atoi(token);
		token = strtok(NULL, delimiter);
		roomNo = atoi(token);
		token = strtok(NULL, delimiter);
		price = atof(token);
		Room* room = createRoom(floor, roomNo, roomType, price);
		Node* node = createNode(room);
		putHT(&ht, node);
	}
	printHT(ht);

	int floorSearch = 10;
	int countRoomsOnFloor = countRoomsFloor(&ht, floorSearch);
	printf("\n\nNumber of rooms on floor %d is: %d\n", floorSearch, countRoomsOnFloor);

	double priceStep = 2000;
	DNode* doubleLinkedList = higherPriceDLL(&ht, priceStep);
	printDLL(doubleLinkedList);

	deleteRoomType(&doubleLinkedList, "Single");
	printDLL(doubleLinkedList);

	fclose(f);
}

Room* createRoom(unsigned short floor, unsigned short roomNo, char* roomType, double price)
{
	Room* room = (Room*)malloc(sizeof(Room));
	room->floor = floor;
	room->roomNo = roomNo;
	room->roomType = (char*)malloc(strlen(roomType) + 1);
	strcpy(room->roomType, roomType);
	room->price = price;
	return room;
}

Node* createNode(Room* room)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->data = room;
	node->next = NULL;
	return node;
}

int fhash(unsigned short key, int size)
{
	return key % size;
}

void putHT(HashTable* ht, Node* node)
{
	if (ht->buckets == NULL)
	{
		ht->size = HashTable_size;
		ht->buckets = (Node**)malloc(ht->size * sizeof(Node*));
		memset(ht->buckets, NULL, ht->size * sizeof(Node*));
	}
	int index = fhash(node->data->floor, ht->size);
	putNode(ht, node, index);
}

void putNode(HashTable* ht, Node* node, int index)
{
	if (ht->buckets[index] == NULL)
	{
		ht->buckets[index] = node;
	}
	else
	{
		node->next = ht->buckets[index];
		ht->buckets[index] = node;
	}
}

DNode* creadeDNode(Room* data)
{
	DNode* node = (DNode*)malloc(sizeof(DNode));
	node->data = data;
	node->prev = node->next = NULL;
	return node;
}

int countRoomsFloor(HashTable* ht, int floor)
{
	int cnt = 0;
	int index = fhash(floor, HashTable_size);
	Node* bucket = ht->buckets[index];
	while (bucket)
	{
		cnt++;
		bucket = bucket->next;
	}
	return cnt;
}

DNode* higherPriceDLL(HashTable* ht, double price)
{
	DNode* list = NULL;
	for (int i = 0; i < HashTable_size; i++)
	{
		if (ht->buckets[i] != NULL)
		{
			Node* bucket = ht->buckets[i];
			while (bucket)
			{
				if (bucket->data->price > price)
				{
					DNode* node = creadeDNode(bucket->data);
					if (list != NULL)
					{
						node->next = list;
						list->prev = node;
						list = node;
					}
					else
					{
						list = node;
					}
				}
				bucket = bucket->next;
			}
		}
	}
	return list;
}

void deleteRoomType(DNode** list, const char* roomType)
{
	while (strcmp((*list)->data->roomType, roomType) == 0)
	{
		DNode* freeMe = *list;
		*list = (*list)->next;
		free(freeMe);
	}
	DNode* dlist = *list;
	while (dlist)
	{
		if (strcmp(dlist->data->roomType, roomType) == 0)
		{
			DNode* freeMe = dlist;
			dlist->prev->next = dlist->next;
			if (dlist->next)
			{
				dlist->next->prev = dlist->prev;
			}
			dlist = dlist->prev;
			free(freeMe->data->roomType);
			free(freeMe);
		}
		dlist = dlist->next;
	}
}

void printRoom(Room* room)
{
	printf("\nRoom type %s, floor: %d, room number: %d, price: %.1f", room->roomType, room->floor, room->roomNo, room->price);
}

void printDLL(DNode* list)
{
	printf("\nDouble linked list:");
	while (list)
	{
		printRoom(list->data);
		list = list->next;
	}
	printf("\n");
}

void printHT(HashTable ht)
{
	printf("\nHash Table: ");
	for (int i = 0; i < ht.size; i++)
	{
		printf("\nBucket %d: ", i);
		Node* bucket = ht.buckets[i];
		while (bucket)
		{
			printRoom(bucket->data);
			bucket = bucket->next;
		}
	}
	printf("\n______________________");
}