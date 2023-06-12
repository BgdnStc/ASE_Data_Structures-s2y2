#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define BSIZE 1024

struct Booking 
{
	unsigned int id;
	char* hotel;
	unsigned char noRooms;
	char* guestName;
	float price;
	char paid;
};
typedef struct Booking Booking;

struct SimpleLinkedList 
{
	Booking* booking;
	struct SimpleLinkedList* next;
};
typedef struct SimpleLinkedList Node;

struct SimpleBooking 
{
	char* hotel;
	unsigned char noRooms;
};
typedef struct SimpleBooking SB;

struct SimpleLinkedListS 
{
	SB* booking;
	struct SimpleLinkedListS* next;
};
typedef struct SimpleLinkedListS NodeS;

Booking* createBooking(unsigned int id, const char* hotel, unsigned char noRooms, const char* guestName, float price, char paid)
{
	Booking* b = NULL;
	b = (Booking*)malloc(sizeof(Booking));
	if (b)
	{
		b->id = id;
		b->noRooms = noRooms;
		b->price = price;
		b->paid = paid;
		b->hotel = (char*)malloc(strlen(hotel) + 1);
		strcpy(b->hotel, hotel);
		b->guestName = (char*)malloc(strlen(guestName) + 1);
		strcpy(b->guestName, guestName);
	}
	return b;
}

Node* add(Node* list, Node* node)
{
	node->next = list;
	return node;
}

Node* createNode(Booking* b)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->next = NULL;
	node->booking = b;
	return node;
}

void printBooking(Booking* b)
{
	printf("\nBooking %d: name: %s, noRooms: %d, guestName: %s, price: %.2f, paid: %c",
		b->id, b->hotel, b->noRooms, b->guestName, b->price, b->paid);
}

void printList(Node* list)
{
	while (list)
	{
		printBooking(list->booking);
		list = list->next;
	}
}

//void add(Node** list, Node* node)
//{
//	node->next = *list;
//	*list = node;
//}

int getApp(Node* list, const char* name)
{
	int noApp = 0;
	while (list)
	{
		if (strcmp(list->booking->guestName, name) == 0)
			noApp++;
		list = list->next;
	}
	return noApp;
}

void freeBooking(Booking* b)
{
	free(b->guestName);
	free(b->hotel);
}

Node* deletePaid(Node* list)
{
	if(list == NULL)
		return NULL;
	while (list && list->booking->paid == 'Y')
	{
		Node* freeMe = list;
		freeBooking(list->booking);
		list = list->next;
		free(freeMe);
	}
	if (list != NULL)
	{
		Node* aux = list;

		while (aux->next)
		{
			if (aux->next && aux->next->booking->paid == 'Y')
			{
				Node* freeMe = aux->next;
				aux->next = aux->next->next;
				freeBooking(freeMe->booking);
				free(freeMe);
			}
			else
				aux = aux->next;
		}
	}
	
	return list;
}

NodeS* addNode(NodeS* list, Node* node)
{
	NodeS* nodeS = (NodeS*)malloc(sizeof(NodeS));
	nodeS->next = list;
	nodeS->booking = (SB*)malloc(sizeof(SB));
	nodeS->booking->hotel = (char*)malloc(strlen(node->booking->hotel) + 1);
	strcpy(nodeS->booking->hotel, node->booking->hotel);
	nodeS->booking->noRooms = node->booking->noRooms;
	return nodeS;
}

NodeS* createListFromCustomer(Node* list, const char* cust)
{
	NodeS* list2 = NULL;

	while (list)
	{
		if (strcmp(list->booking->guestName, cust) == 0)
			list2 = addNode(list2, list);
		list = list->next;
	}

	return list2;
}

void printListS(NodeS* list)
{
	while (list)
	{
		printf("\nHotel: %s, noRooms: %d", list->booking->hotel, list->booking->noRooms);
		list = list->next;
	}
}

void main()
{
	FILE* fp = fopen("bookings.txt", "r");
	if (!fp) return -1;

	char buffer[BSIZE];
	char* tok = NULL;
	char del[] = ",\n";

	Node* list = NULL;

	unsigned int id;
	char hotel[BSIZE];
	unsigned char noRooms;
	char guestName[BSIZE];
	float price;
	char paid;

	while (fgets(buffer, BSIZE, fp))
	{
		tok = strtok(buffer, del);
		id = atoi(tok);

		tok = strtok(NULL, del);
		strcpy(hotel, tok);
		memmove(hotel, hotel + 1, strlen(hotel) + 1);

		tok = strtok(NULL, del);
		noRooms = atoi(tok);

		tok = strtok(NULL, del);
		strcpy(guestName, tok);
		memmove(guestName, guestName + 1, strlen(guestName) + 1);

		tok = strtok(NULL, del);
		price = atof(tok);

		tok = strtok(NULL, del);
		paid = tok[1];

		Booking* booking = createBooking(id, hotel, noRooms, guestName, price, paid);
		Node* node = createNode(booking);
		list = add(list, node);
		//addNode(&list, node);
	}

	printList(list);

	int noApp = getApp(list, "George");
	printf("\n\nNo of appearances for George: %d", noApp);

	NodeS* list2 = createListFromCustomer(list, "George");

	list = deletePaid(list);
	printList(list);

	printListS(list2);

	fclose(fp);
}