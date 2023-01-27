//Step 4
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define BLOCKSIZE 5

int maxInput = (5 * BLOCKSIZE);
//define the structures that represent the nodes of the list and the list as a unit
struct node {
    int i;
    struct node *next;
    struct node *prev;

};
//list handle, containing all information you need to access the list
struct list {
    struct node *head;
    struct node *tail;
    struct node *left;
    struct node *right;
    int length;

};


/*– allocate size bytes of memory by calling malloc,
– check if malloc returned a valid pointer, i.e. a non-null pointer, and, if so, increase the value of
the counter by one, and
– return the pointer returned by malloc.
*/
void *allocator(int size, int *counter) {
    int *size_byte = malloc(size);
    if ( size_byte != NULL){
        *counter = *counter +1;
    }
    return size_byte;
}

/*
– check that the first argument is a valid, i.e. non-null, pointer and, if so, free the memory pointed
by the first argument and decrease the counter by one and
– return nothing.
*/
void deAllocator(void *p, int *counter){
    if (p != NULL){
        free(p);
        *counter = *counter - 1;
    }
}

/*
– call allocator to allocate the head and the tail of the list, – initialise the members of the list as in Listing 1, and
– return nothing.
*/
void initialiseList(struct list *pList, int *counter) {
    pList->head = allocator(sizeof(struct node), counter);
    pList->tail = allocator(sizeof(struct node), counter);
    pList->length = 0;
    (pList->head)->next = pList->tail;
    (pList->tail)->prev = pList->head;
    (pList->head)->prev = NULL;
    (pList->tail)->next = NULL;
    (pList->tail)->i=0;
    (pList->head)->i=0;
    pList->right = pList->tail;
    pList->left = pList->head;
   
   
}

/*
– check that pList->left points to the same node as pList->head, pList->right points to the same node as pList->tail, and pList->length is zero, i.e. that the list does not contain any occupied or free nodes,
– if the list is empty, call deAllocator twice to deallocate the head and the tail of the list, and
– if the deallocation is successful, set all pointers to NULL and length to −1.
*/
void freeList (struct list *pList, int *counter){
    if (pList->right == pList->tail || pList->left == pList->head || pList->length == 0) {
        deAllocator(pList ->head, counter);
        deAllocator(pList ->tail, counter);
        pList->tail = NULL;
        pList->head = NULL;
        pList->right = NULL;
        pList->left = NULL;
        pList->length = -1;
    }
    return;
}


/*– allocate a new object of type struct node by calling allocator,
– link the new node to the existing ones so that the doubly-linked structure of the list is preserved, – setiofthenewnodeto−1,and
– increase pList->length by one.
*/
void allocateBlock(struct list *pList, int *counter, int nNodes){
    //struct node *current_node = pList->head;
    struct node *temp_node;
    for (int i = 0; i < nNodes; ++i){
        temp_node=allocator(sizeof(struct node), counter);
        
        temp_node->next = (pList->left)->next;
        temp_node->prev=pList->left;
        temp_node->i = -1;
        (pList->left)->next->prev = temp_node;
        (pList->left)->next = temp_node;
        
        
       
        pList->length = pList->length + 1;
    }
}
//prints list
void printList(struct list *pList, int *counter) { 
    struct node *cur = pList->head;
    while (cur) {
    printf(" | %d | ", cur->i);
    cur = cur->next; }
    if (!(pList->head) || !(pList->tail) || *counter == 0) printf(" | | ");
    printf("\ncounter = %d\n", *counter); }



void deAllocateBlock(struct list *pList, int *counter, int nNodes) {
    int n = (pList->length) - nNodes;
    while ((pList->length) > n) {
        struct node *temp = pList->left->next;
        pList->left->next->next->prev = pList->left;
        pList->left->next = temp->next;
        deAllocator(temp, counter);
        (pList->length)--;
    }
}

/*
– check if i is odd or even,
– if i is odd, check that pList->left ̸= pList->head, i.e. there is at least one odd integer in the
list, and, if so,
* replace the digit in the node pointed by pList->left with −1,
* move pList->left to pList->left->prev, and
* increase pList->length by one,
– if i is even, check that pList->right ̸= pList->tail, i.e. there is at least one even integer in
the list, and, if so,
* replace the digit in the node pointed by pList->right with −1,
* move pList->right to pList->right->next, and
* increase pList->length by one,
– check if pList->length > BLOCKSIZE and, if so, call deAllocateBlock to remove BLOCKSIZE free
nodes from the free space.
*/
void pullInt(struct list *pList, int *counter, int i){
    if (i % 2 != 0){//if odd
        if(pList->head != pList->left){
            pList->left->i = -1;
            pList ->left = pList->left->prev;
            pList->length = pList->length + 1;
        }
    }
    if (i % 2 == 0){//if even
        if(pList->right != pList->tail){
            pList->right->i = -1;
            pList ->right = pList->right->next;
            pList->length = pList->length + 1;
        }
    }
    if (pList->length >BLOCKSIZE){
        deAllocateBlock(pList, counter, BLOCKSIZE);
    }

}
/*
– remove all digits in the head-side stack by calling pullInt with i = 1 until pList->left points to the head of the list,
– remove all digits in the tail-side stack by calling pullInt with i = 0 until pList->right points to the tail of the list,
– remove all free nodes by calling deAllocateBlock with pList->length as the third argument, and
– free the list by calling freeList.
*/
void clearList(struct list *pList, int *counter){
    while(pList->head !=pList->left){
        pullInt(pList, counter, 1);

    }
    while(pList->tail != pList->right){
        pullInt(pList, counter, 0);
    }
    int count;
    for (count = pList->length; count>0; count--){
        deAllocateBlock(pList, counter, pList->length);
    }
    freeList(pList, counter);

}

/*
– call allocateBlock, with BLOCKSIZE as a third argument, if there are no free nodes in the gap,
– check if i is odd or even,
– if i is odd, store it on the first available node on the right of the head-side stack, i.e. on the node on the right of the node pointed by pList->left, and,
– if i is even, store it on the first available node on the left of the tail-side stack, i.e. on the node on the left of the node pointed by pList->right.
*/
void pushInt(struct list *pList, int *counter, int i){
    
    
    struct node *even_number = pList->tail; //node holding even number
    struct node *odd_number = pList ->head;
    

    //call allocateBlock, with BLOCKSIZE as a third argument, if there are no free nodes in the gap,
    if (pList->length== 0){
        allocateBlock(pList, counter, BLOCKSIZE);
    }
    
    if (i % 2 == 0){//checks if even
        
        while (even_number -> i != -1){
            even_number = even_number ->prev;
        }
        
        even_number->i = i;
        (pList->length) = (pList->length) - 1;
        pList->right = even_number;
    }
    
    
    if (i % 2 != 0){ //checks if odd
        while (odd_number->i != -1){//finds first available node on righ
            odd_number = odd_number -> next;
        }
        odd_number -> i = i;
        pList -> left = odd_number;
        (pList->length) = (pList->length)-1;
        //stores in the list  
        
    }
    
    

}

int getInput(char *s){
    char c;
    int i;
    c = '\0';
    i = 0;
    while (c != '\n' && i < maxInput){
        c = getc(stdin);
        if(c == '0' ||c == '1' ||c == '2' ||c == '3' ||c == '4' ||c == '5' ||c == '6' ||c == '7' ||c == '8' ||c == '9'){
            s[i] = c;
            i = i + 1;
        }
    }
    if (i == maxInput){
        while (c != '\n'){
            c = getc(stdin);    
        }
    }
    return i;
}




int main() {
    int counter = 0;
    struct list myList; 
    initialiseList(&myList, &counter); 
    printList(&myList, &counter);
    char sPush[maxInput];
    char sPull[maxInput];
    int lenPush = getInput(sPush);
    int lenPull = getInput(sPull);
    int i = 0;
    int j = 0;
    
    while ((i + j) < (lenPush + lenPull)) {
        
        if(i<lenPush){
            
            //pushInt(&myList, &counter, i); <===this is creating problems with my main
        
            i = i + 1;
            printList(&myList, &counter);

        }
        
        if(j<lenPull){
            pullInt(&myList, &counter, j);
            j = j+ 1;
            printList(&myList, &counter);

        }
        
        printList(&myList, &counter);
        clearList(&myList, &counter);
        printList(&myList, &counter);

    }


}

