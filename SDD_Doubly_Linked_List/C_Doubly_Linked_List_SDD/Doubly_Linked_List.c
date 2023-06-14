#include <stdio.h>

struct node 
{
	char* data;
	struct node* prev;
	struct node* next;
};

struct node* head = NULL;

struct node* createNode(char* data) 
{
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	newNode->data = data;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

void insertAtHead(char* data) 
{
	struct node* newNode = createNode(data);
	if (head == NULL) 
	{
		head = newNode;
		newNode->prev = NULL;
		newNode->next = NULL;
	}
	else 
	{
		newNode->prev = NULL;
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}
}

void insertAtTail(char* data) 
{
	struct node* newNode = createNode(data);
	if (head == NULL) 
	{
		head = newNode;
		newNode->prev = NULL;
		newNode->next = NULL;
	}
	else 
	{
		struct node* temp = head;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		newNode->prev = temp;
		newNode->next = NULL;
		temp->next = newNode;
	}
}

void insert(char* data, int position) 
{
	struct node* newNode = createNode(data);
	if (head == NULL)
	{
		head = newNode;
		newNode->prev = NULL;
		newNode->next = NULL;
	}
	else
	{
		struct node* temp = head;
		int cursor = 1;
		int end = 0;
		if (position == 1)
		{
			newNode->next = temp;
			newNode->prev = NULL;
			temp->prev = newNode;
		}
		else
		{
			while (cursor != position)
			{
				if (temp->next == NULL)
				{
					end = 1;
					break;
				}
				temp = temp->next;
				cursor++;
			}
			if (end != 1)
			{
				newNode->prev = temp->prev;
				newNode->next = temp;
				temp->prev = newNode;
				temp = newNode->prev;
				temp->next = newNode;
			}
			else
			{
				newNode->prev = temp;
				newNode->next = NULL;
				temp->next = newNode;
			}
		}
	}
}

struct node* printList() 
{
	struct node* temp = head;
	while (temp->next != NULL)
	{
		printf("%s ",temp->data);
		temp = temp->next;
	}
	printf("%s ", temp->data);
}

int main() 
{
	insertAtHead("Popescu");
	insertAtTail("Ionescu", 2);
	insert("Georgescu", 3);
	insert("Iliescu", 3);
	insert("Antonescu", 5);
	insert("Patrulescu", 2);
	insertAtTail("Enachescu");
	insert("Fintescu", 7);
	insert("Odaescu", 5);
	insert("Calinescu", 9);

	printList();
}

