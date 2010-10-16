//Bubble sort [linked list]

#include <stdio.h>
#include <stdlib.h>

#define MAX 10
#define PERSON struct person

PERSON {
 int data;
 PERSON *next;
} *head, *visit;

/* add a new entry to the linked list */
void llist_add(PERSON **q, int num);
/* preform a bubble sort on the linked list */
void llist_bubble_sort(void);
/* print the entire linked list */
void llist_print(void);

int main(void) {
 /* linked list */
 PERSON *newnode = NULL;
 int i = 0; /* a general counter */

 /* load some random values into the linked list */
 for(i = 0; i < MAX; i++) {
  llist_add(&newnode, (rand() % 100));
 }

 head = newnode;
 printf("Before bubble sort:\n");
 llist_print();
 printf("After  bubble sort:\n");
 llist_bubble_sort();
 llist_print();

 return 0;
}

/* adds a node at the end of a linked list */
void llist_add(PERSON **q, int num) {
 PERSON *tmp; 
 
 tmp = *q;

 /* if the list is empty, create first node */
 if(*q == NULL) {
  *q = (PERSON*)malloc(sizeof(PERSON));
   tmp = *q;
 } else {
  /* go to last node */
  while(tmp->next != NULL)
   tmp = tmp->next;

   /* add node at the end */
   tmp->next = (PERSON*)malloc(sizeof(PERSON));
   tmp = tmp->next;
 }

 /* assign data to the last node */
 tmp->data = num;
 tmp->next = NULL;
}

/* print the entire linked list */
void llist_print(void) {
 visit = head;

 while(visit != NULL) {
  printf("%d ", visit->data);
  visit = visit->next;
 }
 printf("\n");
}

/* preform a bubble sort on the linked list */
void llist_bubble_sort(void) {
 PERSON *a = NULL;
 PERSON *b = NULL; 
 PERSON *c = NULL;
 PERSON *e = NULL; 
 PERSON *tmp = NULL; 

 /* 
 // the `c' node precedes the `a' and `e' node 
 // pointing up the node to which the comparisons
 // are being made. 
 */
 while(e != head->next) {
 c = a = head;
 b = a->next;
  while(a != e) {
   if(a->data > b->data) {
    if(a == head) {
     tmp = b -> next;
     b->next = a;
     a->next = tmp;
     head = b;
     c = b;
    } else {
     tmp = b->next;
     b->next = a;
     a->next = tmp;
     c->next = b;
     c = b;
    }
   } else {
    c = a;
    a = a->next;
   }
   b = a->next;
   if(b == e)
    e = a;
  }
 }
}
