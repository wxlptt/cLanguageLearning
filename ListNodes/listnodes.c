// fig12_03.c
// Inserting and deleting nodes in a list
// Modified by: Bob Langelaan
// Date: July 11, 2022

// Assignment 6
// Author: Xiaolong Wang (Wilson)
// Date: Nov 9, 2023

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

// self-referential structure                       
struct listNode {                                    
   char data; // each listNode contains a character  
   struct listNode *nextPtr; // pointer to next node 
};                                                   

typedef struct listNode ListNode; // synonym for struct listNode
typedef ListNode *ListNodePtr; // synonym for ListNode*

// prototypes
void insert(ListNodePtr *sPtr, char value);
char delete(ListNodePtr *sPtr, char value);
int isEmpty(ListNodePtr sPtr);
void printList(ListNodePtr currentPtr, unsigned int number);
void instructions(void);
void merge(ListNodePtr* sPtr1, ListNodePtr* sPtr2);

int main(void) {
   ListNodePtr startPtr1 = NULL;    // initially there are no nodes.
   ListNodePtr startPtr2 = NULL;    // initially there are no nodes.

   char item = '\0'; // char entered by user

   instructions(); // display the menu
   
   int choice = 0; // user's choice

   // loop while user does not choose 3

   int timeToExit = 0;  // false to begin with
   do {
      printf("%s", "? ");
      scanf("%d", &choice);

      switch (choice) { 
         case 1: // insert an element into list 1
            printf("%s", "Enter a character to be inserted into list 1: ");
            scanf("\n%c", &item);
            insert(&startPtr1, item); // insert item in list
            printList(startPtr1, 1);
            break;
         case 2: // delete an element
            if (!isEmpty(startPtr1)) { // if list is not empty
               printf("%s", "Enter character to be deleted: ");
               scanf("\n%c", &item);

               // if character is found, remove it
               if (delete(&startPtr1, item)) { // remove item
                  printf("%c deleted.\n", item);
                  printList(startPtr1, 1);
               } 
               else {
                  printf("%c not found.\n\n", item);
               } 
            } 
            else {
               puts("List 1 is empty.\n");
            } 

            break;

         case 3: // insert an element into list 2
             printf("%s", "Enter a character to be inserted into list 2: ");
             scanf("\n%c", &item);
             insert(&startPtr2, item); // insert item in list
             printList(startPtr2, 2);
             break;

         case 4:
             if (!isEmpty(startPtr2)) { // if list is not empty
                 printf("%s", "Enter character to be deleted: ");
                 scanf("\n%c", &item);

                 // if character is found, remove it
                 if (delete(&startPtr2, item)) { // remove item
                     printf("%c deleted.\n", item);
                     printList(startPtr2, 2);
                 }
                 else {
                     printf("%c not found.\n\n", item);
                 }
             }
             else {
                 puts("List 1 is empty.\n");
             }

             break;
         
         case 5:
             // Print two lists before merge.
             printf("\n%s\n\n", "Before merge, the 2 lists are:");
             printList(startPtr1, 1);
             printList(startPtr2, 2);

             merge(&startPtr1, &startPtr2);

             // Print two lists after merge.
             printf("\n%s\n\n", "After merge, the 2 lists are:");
             printList(startPtr1, 1);
             printList(startPtr2, 2);

             break;
    
         case 6: //time to go home
             timeToExit = 1; // now set to true
             break;

         default:
            puts("Invalid choice.\n");
            instructions();
            break;
      }

   }while (!timeToExit);

   puts("End of run.");
} 

// display program instructions to user
void instructions(void) {
   puts("Enter your choice:\n"
      "   1 to insert an element into list 1.\n"
      "   2 to delete an element from list 1.\n"
      "   3 to insert an element into list 2.\n"
      "   4 to delete an element from list 2.\n"
      "   5 merge list 2 into list 1.\n"
      "   6 to end.");
} 

// insert a new value into the list in sorted order
void insert(ListNodePtr *sPtr, char value) {
   ListNodePtr newPtr = malloc(sizeof(ListNode)); // create node

   if (newPtr != NULL) { // is space available?
      newPtr->data = value; // place value in node
      newPtr->nextPtr = NULL; // node does not link to another node

      ListNodePtr previousPtr = NULL;
      ListNodePtr currentPtr = *sPtr;

      // loop to find the correct location in the list        
      while (currentPtr != NULL && value > currentPtr->data) {
         previousPtr = currentPtr; // walk to ...             
         currentPtr = currentPtr->nextPtr; // ... next node   
      }                                                       

      // insert new node at beginning of list
      if (previousPtr == NULL) { 
         newPtr->nextPtr = *sPtr;
         *sPtr = newPtr;
      } 
      else { // insert new node between previousPtr and currentPtr
         previousPtr->nextPtr = newPtr;
         newPtr->nextPtr = currentPtr;
      } 
   } 
   else {
      printf("%c not inserted. No memory available.\n", value);
   } 
} 

// delete a list element
char delete(ListNodePtr *sPtr, char value) {
   // delete first node if a match is found
   if (value == (*sPtr)->data) { 
      ListNodePtr tempPtr = *sPtr; // hold onto node being removed
      *sPtr = (*sPtr)->nextPtr; // de-thread the node
      free(tempPtr); // free the de-threaded node
      return value;
   } 
   else { 
      ListNodePtr previousPtr = *sPtr;
      ListNodePtr currentPtr = (*sPtr)->nextPtr;

      // loop to find the correct location in the list
      while (currentPtr != NULL && currentPtr->data < value) { 
         previousPtr = currentPtr; // walk to ...  
         currentPtr = currentPtr->nextPtr; // ... next node  
      } 

      // delete node at currentPtr
      if (currentPtr != NULL && currentPtr->data == value) { 
         ListNodePtr tempPtr = currentPtr;
         previousPtr->nextPtr = currentPtr->nextPtr;
         free(tempPtr);
         return value;
      } 
   } 

   return '\0';
} 

// Merge the sorted list 2 into the sorted list 1.
void merge(ListNodePtr* sPtr1, ListNodePtr* sPtr2) {
    ListNodePtr previousListNode = NULL;
    ListNodePtr followingListNode = *sPtr1;
    ListNodePtr insertNode = *sPtr2;

    // If the list 1 is NULL, copy list2 into list1.
    if (*sPtr1 == NULL)
    {
        *sPtr1 = *sPtr2;
        *sPtr2 = NULL;
    }

    else
    {
        // Move the start pointer of list 2, when it is not NULL, merge the element into list 1.
        while (insertNode != NULL)
        {
            // If the following list node is NULL, directly paste the element to list 1.
            if (followingListNode == NULL)
            {
                // Move the start pointer of list 2.
                *sPtr2 = (*sPtr2)->nextPtr;

                // Merge data
                previousListNode->nextPtr = insertNode;
                insertNode->nextPtr = NULL;

                // Move the previous list node and insert node.
                previousListNode = insertNode;
                insertNode = *sPtr2;
            }

            // If the insert data less than or equal the following list data, merge it into the list 1.
            else if (insertNode->data <= followingListNode->data)
            {
                // Move the start pointer of list 2.
                *sPtr2 = (*sPtr2)->nextPtr;

                // If no data is less than insert data in the list 1, paste the data into front of the list.
                if (previousListNode == NULL)
                {
                    insertNode->nextPtr = *sPtr1;
                    *sPtr1 = insertNode;
                }
                else
                {
                    // Insert the data into list 1.
                    previousListNode->nextPtr = insertNode;
                    insertNode->nextPtr = followingListNode;
                }

                // Move the previous list node to newly inserted list node. That can check next inserted data.
                previousListNode = insertNode;
                // Set the next data inserted.
                insertNode = *sPtr2;
                
            }
            
            else
            {
                // If the inserted data bigger than the current position, move the position to next node.
                previousListNode = followingListNode;
                followingListNode = followingListNode->nextPtr;
            }
        }
    }
}

// return 1 if the list is empty, 0 otherwise
int isEmpty(ListNodePtr sPtr) {
   return sPtr == NULL;
} 

// print the list, the parameter number simply represents the index of the lists.
void printList(ListNodePtr currentPtr, unsigned int number) {
   // if list is empty
   if (isEmpty(currentPtr)) {
      printf("List %u is empty.\n", number);
   } 
   else { 
      printf("List %u is:\n", number);

      // while not the end of the list
      while (currentPtr != NULL) { 
         printf("%c --> ", currentPtr->data);
         currentPtr = currentPtr->nextPtr;   
      } 

      puts("NULL\n");
   } 
} 





/**************************************************************************
 * (C) Copyright 1992-2015 by Deitel & Associates, Inc. and               *
 * Pearson Education, Inc. All Rights Reserved.                           *
 *                                                                        *
 * DISCLAIMER: The authors and publisher of this book have used their     *
 * best efforts in preparing the book. These efforts include the          *
 * development, research, and testing of the theories and programs        *
 * to determine their effectiveness. The authors and publisher make       *
 * no warranty of any kind, expressed or implied, with regard to these    *
 * programs or to the documentation contained in these books. The authors *
 * and publisher shall not be liable in any event for incidental or       *
 * consequential damages in connection with, or arising out of, the       *
 * furnishing, performance, or use of these programs.                     *
 *************************************************************************/
