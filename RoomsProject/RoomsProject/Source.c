#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define HashTable_Size 20

struct Room
{
	unsigned short floor;
	unsigned short roomNo;
	char* roomType;
	double price;
};
typedef struct Room Room;

struct CollisionList
{
	Room* room;
	struct CollisionList* next;
};
typedef struct CollisionList Node;

struct HashTable
{
	Node** buckets;
	int size;
};
typedef struct HashTable HashTable;

void putHT(HashTable* hashTable, Node* node);
void putNode(HashTable* hashTable, Node* node, int index);
void printHashTable(HashTable hashTable);
Room* createRoom(unsigned short floor, unsigned short roomNo, char* roomType, double price);
Node* createNode(Room* room);

int main()
{
	FILE* f = fopen("data.txt", "r");	
	if (!f) 
	{
		printf("bro, no file :(");
		return -1;
	}
	char buffer[256];
	char delimitator[] = ",\n";
	unsigned short floor;
	unsigned short roomNo;
	char* roomType;
	double price;

	HashTable hashTable = { NULL, 0 } ;

	while (fgets(buffer, 256, f))
	{
		char* token = strtok(buffer, delimitator);
		roomType = (char*)malloc(strlen(token) + 1);
		strcpy(roomType, token);
		token = strtok(NULL, delimitator);
		floor = atoi(token);
		token = strtok(NULL, delimitator);
		roomNo = atoi(token);
		token = strtok(NULL, delimitator);
		price = atof(token);

		Room* room = createRoom(floor, roomNo, roomType, price);
		Node* node = createNode(room);
		putHT(&hashTable, node);
	}

	printHashTable(hashTable);
}

void printRoom(Room* room)
{
	printf("\nRoom floor: %d, number: %d, type: %s, price: %.1f", room->floor, room->roomNo, room->roomType, room->price);
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
	node->next = NULL;
	node->room = room;
	return node;
}

int fhash(unsigned short key, int size)
{
	return key % size;
}

void putHT(HashTable* hashTable, Node* node)
{
	if (hashTable->buckets == NULL)
	{
		hashTable->size = HashTable_Size;
		hashTable->buckets = (Node**)malloc(hashTable->size * sizeof(Node*));
		memset(hashTable->buckets, NULL, hashTable->size * sizeof(Node*));
	}
	int index = fhash(node->room->floor, hashTable->size);
	putNode(hashTable, node, index);
}

void putNode(HashTable* hashTable, Node* node, int index)
{
	if (hashTable->buckets[index] == NULL)
	{
		hashTable->buckets[index] = node;
	}
	else
	{
		node->next = hashTable->buckets[index];
		hashTable->buckets[index] = node;
	}
}

void printHashTable(HashTable hashTable)
{
	for (int i = 0; i < hashTable.size; i++)
	{
		printf("\nBuket %d", i);
		Node* bucket = hashTable.buckets[i];
		while (bucket)
		{
			printRoom(bucket->room);
			bucket = bucket->next;
		}
	}
}