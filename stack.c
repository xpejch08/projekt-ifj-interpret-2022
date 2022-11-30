#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include "stack.h"


void DLL_Init( DLList *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

int DLL_InsertFirst( DLList *list, token token ) {
	DLLElementPtr insertingptr = malloc(sizeof(struct DLLElement));

	if(insertingptr == NULL){
        return INT_ERROR; 
	}
    insertingptr->type = token.type;
    insertingptr->content = token.content;  
    insertingptr->previousElement = NULL; 

	if(list->firstElement == NULL){ 
		insertingptr->nextElement = NULL;  
		list->lastElement = insertingptr; 
	}else{ 
		insertingptr->nextElement = list->firstElement;
		list->firstElement->previousElement = insertingptr;
	}
	list->firstElement = insertingptr;

    return SUCCES;
}

int DLL_InsertLast( DLList *list, token token ) {
	 DLLElementPtr newElementLast = malloc(sizeof(struct DLLElement));
        if(newElementLast == NULL) // Kontrola operace malloc
        {
                return INT_ERROR;
        }
		printf("%s\n", list->activeElement->content.str->str);
        if(list->firstElement != NULL)
        {
        newElementLast->nextElement = NULL;
        list->lastElement->nextElement = newElementLast; // Vloží prvek na konec seznamu.
        newElementLast->previousElement = list->lastElement; // Propojí prvky
        list->lastElement = newElementLast; // Nový prvek je nýní poslední.
        }

        // Pokud je seznam prázdný, nový prvek bude první i poslední.
        else{
        newElementLast->nextElement = NULL;
        list->firstElement = newElementLast;
        newElementLast->previousElement = NULL;
        list->lastElement = newElementLast;
        list->lastElement->previousElement = NULL;
        }
		
		list->activeElement = list->firstElement;
    return SUCCES;
}

void DLL_Free( DLList *list ) {
	  DLLElementPtr freeElement;
        //Pokud jsou v seznamu neuvolňené prvky.
        while(list->firstElement != list->lastElement)
        {
                freeElement = list->firstElement; // Uvolní vždy první prvek.
                list->firstElement = list->firstElement->nextElement; // Posune ukazatel na následující prvek.
                free(freeElement);
        }
        free(list->lastElement); // Uvolní poslední zbylý prvek seznamu.
        // Seznam uvede do stavu po inicializaci.
        list->activeElement = NULL;
        list->firstElement = NULL;
        list->lastElement = NULL;
	
}
void DLL_PrintLocal(DLList *list)
{
	
		
		while(list->activeElement != NULL)
		{
			if(list->activeElement == list->lastElement)
			{
			list->activeElement = NULL;
			return;
			}

			if(list->activeElement->type == TYPE_VARIABLE)
			{
			printf("LF@&%s ", list->activeElement->content.str->str);
			}
			else{
			printf("%s ", list->activeElement->content.str->str);
			}
			list->activeElement = list->activeElement->nextElement;
		}
	
}

void DLL_PrintGlobal(DLList *list)
{
		while(list->activeElement != NULL)
		{
			if(list->activeElement == list->lastElement)
			{
			list->activeElement = NULL;
			return;
			}

			if(list->activeElement->type == TYPE_VARIABLE)
			{
			printf("GF@&%s ", list->activeElement->content.str->str);
			}
			else{
			printf("%s ", list->activeElement->content.str->str);
			}
			list->activeElement = list->activeElement->nextElement;
		}
	
}