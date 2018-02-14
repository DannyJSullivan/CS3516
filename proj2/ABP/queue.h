#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
	struct msg data;
	struct Node* next;
}Node;

struct Node* first = NULL;
struct Node* last = NULL;

void queue(struct msg someMsg){
	struct Node* temp=(struct Node*)malloc(sizeof(struct Node));
	strncpy(temp->data.data, someMsg.data, 20);
	temp->next=NULL;
	if(first==NULL&&last==NULL){
		first=temp;
		last=temp;
		return;
	}
	last->next=temp;
	last=temp;
}

void unqueue(){
	struct Node* temp=first;
	if(first==NULL){
		printf("Empty queue...\n");
		return;
	}
	if(first==last){
		first=NULL;
		last=NULL;
	}
	else{
		first=first->next;
	}
	free(temp);
}

struct msg firstStatus(){
	if(first==NULL) {
	  	printf("First in queue is empty...\n");
		struct msg newFirst;
		strcpy(newFirst.data, "empty");
		return newFirst;
	}
	return first->data;
}

int emptyBuff(){
	if(first==NULL){
		return 1;
	}
	else{
		return 0;
	}
}