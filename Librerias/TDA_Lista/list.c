#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct Node Node;

struct Node {
    const void * data;
    Node * next;
    Node * prev;
};

struct List {
    Node * head;
    Node * tail;
    Node * current;
};

typedef List List;

Node * createNode(const void * data) {
    Node * new = (Node *)malloc(sizeof(Node));
    assert(new != NULL);
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}

//1
List * createList() 
{
  List * lista = malloc (sizeof(List));
	lista->head = NULL;
	lista->tail = NULL;
	lista->current = NULL;
	return lista;
}

//2
void * firstList(List * list) 
{
	if(list->head == NULL) return NULL;
	list->current = list->head;
  return (void *) list->current->data;
}


void * nextList(List * list) 
{
	if(list->current == NULL || list->current->next == NULL) return NULL;
	list->current = list->current->next;
	return (void *) list->current->data;
}

//3
void * lastList(List * list) 
{
	if(list->tail == NULL) return NULL;
	list->current = list->tail;
  return (void *) list->current->data;
}

void * prevList(List * list) 
{
  if(list->current == NULL || list->current->prev == NULL) return NULL;
	list->current = list->current->prev;
	return (void *) list->current->data;
}

//4
void pushFront(List * list, const void * data) 
{
	Node * nodo = createNode(data);
	
	if(list->tail == NULL) 
	{
		list->tail = nodo;
		list->head = nodo;
	}
	else
	{
		nodo->next = list->head;
		nodo->prev = NULL;
		list->head->prev = nodo;
		list->head = nodo;
	}
}

void pushBack(List * list, const void * data) 
{
	Node * nodo = createNode(data);
  
  	if(list->head == NULL)
	{
	 	list->head = nodo;
		list->tail = nodo;
	}
	else
	{
		nodo->next = NULL;
		nodo->prev = list->tail;
		list->tail->next = nodo;
		list->tail = nodo;

	}
}

//5
void pushCurrent(List * list, const void * data) 
{
	Node * nodo = createNode(data); 
	nodo->prev = list->current;

	if(list->current == list->tail)//esta al final
	{
		nodo->next = NULL;
		list->current->next = nodo;
		list->tail = nodo;
		list->tail->prev = list->current;
	}
	else 
	{
		if (list->current->next == list->head) //esta al inicio
		{
			nodo->next = list->head;
			list->current->next = nodo;
			list->head = nodo;
			list->head->next = list->current;
		}
		else //para cualquier posicion
		{
			//union con el nodo siguiente al current, para el nuevo nodo
			nodo->next = list->current->next;
			list->current->next->prev = nodo;

			//union con el nodo nuevo al current
			list->current->next = nodo;
		}
	}
	
}

void * popFront(List * list) {
    list->current = list->head;
    return popCurrent(list);
}

void * popBack(List * list) {
    list->current = list->tail;
    return popCurrent(list);
}

void * popCurrent(List * list) 
{
	const void * auxiliar = list->current->data;

	if(list->current == list->head)
	{
		list->current->next->prev = NULL;
		list->head = list->current->next;
		free(list->current);
		list->current = list->head;
	}
	else
	{
		if(list->current == list->tail)
		{
			list->current->prev->next = NULL;
			list->tail = list->current->prev;
			free(list->current);
			list->current = list->tail;
		}
		else
		{
			list->current->next->prev = list->current->prev;
			list->current->prev->next = list->current->next;
		}
	}

	return (void *) auxiliar;
}

void cleanList(List * list) {
    while (list->head != NULL) {
        popFront(list);
    }
}