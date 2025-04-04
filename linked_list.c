#include "linked_list.h"

#include <stdlib.h>

//added for easier reference
/*
struct list_node {
  size_t value;
  struct list_node *next;
};

struct linked_list {
  struct list_node *head;
};*/


struct list_node *new_node(size_t value)
{ 
  //allocate memory for the new list_node being made
  //MAKE SURE TO THEM FREE THE MEMORY IN FOLLOWING FUNCTIONS
  struct list_node *node = malloc(sizeof(struct list_node));

  //if the alloaction was successful
  if (node)
  {
    //set the valuye of the node ysuing size_t value
    node->value = value;
    //set the next node value
    //thre is no provided valye so i think we set to null
    node->next = NULL;
  }
  return node; 
}

//insert a node at the start of the list
void insert_at_head(struct linked_list *list, size_t value) 
{
  //create a node os struct list_node
  struct list_node *node = new_node(value);
  //make sure that the node was made correctly
  if (!node)
  {
    //check return value here
    return;
  }
  //if node creation was successful
  //the created node is the current head since it should be instered there
  node->next = list->head;
  //the head is now the new node
  list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) 
{
  //create a node os struct list_node
  struct list_node *node = new_node(value);
  //make sure that the node was made correctly
  if (node == NULL)
  {
    fprintf(stderr, "failed to allocate new node\n");
    fflush(stderr);
    //check return value here
    return;
  }
  
  if (list->head == NULL)
  {
    list->head = node;
    fprintf(stderr, "inserted first node(head): %zu\n", value);
    fflush(stderr);
    return;
  }
  //since tail is not a part of the structue like head is
  //have to find the last node

  //create struct to help find last node
  //will start at the first not incase it is empty
  struct list_node *nodePtr = list->head;

  /*
  //if the list is empty the new node made is both the tail and the head
  if (list->head == NULL || list->head == 0)
  {
    //set the node to the head because it is both the tail and the head
    //but the head is a part of the structure so it is easier to set it to this value
    list->head = node;
    //check return value?????
    return;
  }*/

  //if it is not empty have to find the last node
  //go through all the nodes in the list
  while (nodePtr->next)
  {
    //if there is a next node set the node to the next node
    //this will go through all the nodes until there is no more next
    nodePtr = nodePtr->next;
  }

  //when the loop was excited meaning that there is not next node
  //create a next node to attach the created node to
  nodePtr->next = node;

  //check return value!!!!
  //return;

  fprintf(stderr, "instered at tail: %zu\n", value);
  fflush(stderr);
}

size_t remove_from_head(struct linked_list *list) 
{ 

  if (list->head == NULL)
  {
    fprintf(stderr, "remove_from_head(): List is empty\n");
    fflush(stderr);
    return 0;
  }


  struct list_node *headNode = list->head;
  size_t valueHead = headNode->value;
  list->head = headNode->next;
  free(headNode);
  return valueHead;
  
  /*
  //find the current head of the list
  struct list_node *headNode = list->head;

  //if the list is empty return nothing because there is nothing to remove
  if (list->head == NULL || list->head == 0)
  {
    return 0;
  }

  //set the new head to the next node
  list->head = headNode->next;
  //store the value of the head node to a varibale to return
  size_t valueHead = headNode->value;

  //free the memory of the removed head node
  free(headNode);

  //return the head node value
  return valueHead; */
}

size_t remove_from_tail(struct linked_list *list) 
{ 
  //set the pointer to the head
  struct list_node *nodePtr = list->head;

  //if the list is empty return nothing because there is nothing to remove
  if (list->head == NULL || list->head == 0)
  {
    return 0;
  }

  //create node to keep lasy node so we van use ->next to get rid of the tial value
  struct list_node *ptrLast = NULL;

  //do the same traveralsal thing through the list to find the last value
  //if it is not empty have to find the last node
  //go through all the nodes in the list
  while (nodePtr->next)
  {
    //keep track of the previious pointer so we can use ->next to get rid of the tail
    ptrLast = nodePtr;
    //if there is a next node set the node to the next node
    //this will go through all the nodes until there is no more next
    nodePtr = nodePtr->next;
  }

  //store the value of the final node to return
  size_t nodeValue = nodePtr->value;

  //the value of the last node is stored but now the last value has to be emilinated
  //set the tail value to null
  ptrLast->next = NULL;

  //free the removed tail
  free(nodePtr);

  //return the tail node value
  return nodeValue; 
}

void free_list(struct linked_list list) 
{
  //have to free all of the lists 
  //gonna have to loop through all the lists to free them

  //set the ptr to the start of the list
  struct list_node *nodePtr = list.head;

  //while there is still a node to free
  while(nodePtr)
  {
    //store the nextptr
    struct list_node *nextPtr = nodePtr->next;
    //free the node
    free(nodePtr);
    //set the pointer to the next node
    nodePtr = nextPtr;
  }

  return;
}

//GIVEN
// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
