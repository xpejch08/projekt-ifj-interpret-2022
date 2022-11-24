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
	DLLElementPtr insertinglastptr = malloc(sizeof(struct DLLElement));

	if(insertinglastptr == NULL){
        return INT_ERROR; 
	}
    insertinglastptr->content = token.content;
    insertinglastptr->type = token.type;
    insertinglastptr->nextElement = NULL;
	if(list->firstElement == NULL) 
	{
		list->firstElement = insertinglastptr; 
		insertinglastptr->previousElement = NULL; 
	}else{ 
		insertinglastptr->previousElement = list->lastElement; 
		list->lastElement->nextElement = insertinglastptr; 
	}
	list->lastElement = insertinglastptr;
    return SUCCES;
}

void DLL_Free( DLList *list ) {
	if(list->firstElement == NULL){
		return;
	}
	DLLElementPtr deletingElement = list->firstElement;
	
	do{ 
		list->firstElement = list->firstElement->nextElement; 
		free(deletingElement); 
		deletingElement = list->firstElement; 
	}while(deletingElement != NULL);
	
}