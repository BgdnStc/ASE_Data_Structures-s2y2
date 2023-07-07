#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define ARRAY_SIZE 1024

struct MutualFund
{
	char* code;
	char* group;
	int risk;
	double value;
	float returnPercentage;
};
typedef struct MutualFund MutualFund;

struct DoubleLinkedList
{
	MutualFund* data;
	struct DoubleLinkedList* prev;
	struct DoubleLinkedList* next;
};
typedef struct DoubleLinkedList Node;

MutualFund* createMutualFund(char* code, char* group, int risk, double value, float returnPercentage);
Node* createNode(MutualFund* mutualFund);
void addNode(Node** list, Node** tail, Node* node);
int countRisk(Node* list, int risk);
void determineGain(Node* list);
MutualFund* firstFundAboveValue(Node* list, double value);
MutualFund** arrayWithHigherGain(const Node* list, double gain);
void printMutualFund(MutualFund* mutualFund);
void printDLL(Node* head, Node* tail);

int main()
{
	FILE* f = fopen("mutualfunds.txt", "r");
	if (!f)
	{
		printf("\nFile not found");
		return -1;
	}
	char* code;
	char* group;
	int risk;
	double value;
	float returnPercentage;
	Node* list = NULL;
	Node* tail = NULL;

	char buffer[256];
	char delimiter[] = ",\n";

	while (fgets(buffer, ARRAY_SIZE, f) != NULL)
	{
		char* token = strtok(buffer, delimiter);
		code = (char*)malloc(strlen(token));
		strcpy(code, token);
		token = strtok(NULL, delimiter);
		group = (char*)malloc(strlen(token));
		strcpy(group, token);
		token = strtok(NULL, delimiter);
		risk = atoi(token);
		token = strtok(NULL, delimiter);
		value = atof(token);
		token = strtok(NULL, delimiter);
		returnPercentage = atof(token);

		MutualFund* mutualFund = createMutualFund(code, group, risk, value, returnPercentage);
		Node* node = createNode(mutualFund);
		addNode(&list, &tail, node);
	}
	printDLL(list, tail);

	int riskLevel = 5;
	int cntRisk = countRisk(list, riskLevel);
	printf("\n\nMutual funds with risk higher than %d: %d", riskLevel, cntRisk);

	determineGain(list);

	double thresholdValue = 2000;
	MutualFund* firstMutalFund = firstFundAboveValue(list, thresholdValue);
	printf("\n");
	if (firstMutalFund == NULL)
	{
		printf("\nNo mutual fund found above %.2f", thresholdValue);
	}
	else
	{
		printMutualFund(firstMutalFund);
	}

	double returnExpected = 20;
	MutualFund** array = arrayWithHigherGain(list, returnExpected);

	fclose(f);
}

MutualFund* createMutualFund(char* code, char* group, int risk, double value, float returnPercentage)
{
	MutualFund* mutualFund = (MutualFund*)malloc(sizeof(MutualFund));
	mutualFund->code = (char*)malloc(strlen(code));
	strcpy(mutualFund->code, code);
	mutualFund->group = (char*)malloc(strlen(group));
	strcpy(mutualFund->group, group);
	mutualFund->risk = risk;
	mutualFund->value = value;
	mutualFund->returnPercentage = returnPercentage;
	return mutualFund;
}

Node* createNode(MutualFund* mutualFund)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->data = mutualFund;
	node->prev = node->next = NULL;
	return node;
}

void addNode(Node** list, Node** tail, Node* node)
{
	if (*list == NULL)
	{
		*list = node;
		*tail = node;
	}
	else
	{
		node->next = *list;
		(*list)->prev = node;
		*list = node;
	}
}

int countRisk(Node* list, int risk)
{
	int cnt = 0;
	while (list)
	{
		if (list->data->risk > risk)
		{
			cnt++;
		}
		list = list->next;
	}
	return cnt;
}

void determineGain(Node* list)
{
	printf("\n");
	while (list)
	{
		double gain = list->data->value + list->data->value * list->data->returnPercentage / 100;
		printf("\nMutual fund %s return: %.2f", list->data->code, gain);
		list = list->next;
	}
}

MutualFund* firstFundAboveValue(Node* list, double value)
{
	MutualFund* mutualFund = NULL;
	while (list)
	{
		if (list->data->value > value)
		{
			mutualFund = list->data;
			break;
		}
		list = list->next;
	}
	return mutualFund;
}

MutualFund** arrayWithHigherGain(const Node* list, double gain)
{
	int size = 0;
	Node* listAux = list;
	while (list)
	{
		if ((list->data->value * list->data->returnPercentage / 100) > gain)
		{
			size++;
		}
		list = list->next;
	}
	MutualFund** array = (MutualFund**)malloc(size * sizeof(MutualFund*));
	int index = 0;
	if (size > 0)
	{
		printf("\n\nArray:");
		while (listAux)
		{
			if ((listAux->data->value * listAux->data->returnPercentage / 100) > gain)
			{
				array[index] = listAux->data;
				index++;
			}
			if (index > size)
			{
				break;
			}
			listAux = listAux->next;
		}
		for (int i = 0; i < size; i++)
		{
			printMutualFund(array[i]);
		}
	}
	else
	{
		printf("\n\nArray is empty!");
	}
	return array;
}

void printMutualFund(MutualFund* mutualFund)
{
	printf("\nMutual fund code: %s, group: %s, risk: %d, value: %.2f, return: %.2f", mutualFund->code, mutualFund->group, mutualFund->risk, mutualFund->value, mutualFund->returnPercentage);
}

void printDLL(Node* head, Node* tail)
{
	printf("\n\nForward print:");
	while (head)
	{
		printMutualFund(head->data);
		head = head->next;
	}
	printf("\n\nBackwards print:");
	while (tail)
	{
		printMutualFund(tail->data);
		tail = tail->prev;
	}
}