/* 
    File name    : LinkedList.c
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : implementation of linked list
*/

#include "LinkedList.h"

// Create an empty list
void CreateList(List *L) {
    First(*L) = Nil;
}

// Create an empty book array
void CreateBookArray(BookArray *A) {
    Count(*A) = 0;
    int i;
	for (i = 0; i < MAX_BOOKS; i++) {
        Title(BookAt(*A, i)) = NULL;
        Stock(BookAt(*A, i)) = 0;
        Queue(BookAt(*A, i)) = Nil;
    }
}

// Allocate memory for a new node with string info
address Alokasi(InfoType X, int StockOrPrioritas) {
    address P = (address)malloc(sizeof(Node));
    if (P != Nil) {
        Info(P) = strdup(X);  // Make a copy of the string
        Int(P) = StockOrPrioritas;
        Next(P) = Nil;
    }
    return P;
}

// Free memory of a node
void Dealokasi(address P) {
    if (P != Nil) {        
        free(Info(P));  // Free the string memory
        free(P);
    }
}

// Insert a node at the beginning of the list
void InsertFirst(List *L, address P) {
    Next(P) = First(*L);
    First(*L) = P;
}

// Insert a node at the end of the list
void InsertLast(List *L, address P) {
    if (First(*L) == Nil) {
        InsertFirst(L, P);
    } else {
        address Last = First(*L);
        while (Next(Last) != Nil) {
            Last = Next(Last);
        }
        Next(Last) = P;
    }
}

// Insert a node with priority (lower number = higher priority)
void InsertPriority(address *QueueHead, address P) {
    if (*QueueHead == Nil) {
        *QueueHead = P;
        Next(P) = Nil;
        return;
    }

    address current = *QueueHead;
    address prev = Nil;

    while (current != Nil && Int(current) <= Int(P)) {
        prev = current;
        current = Next(current);
    }

    if (prev == Nil) {
        Next(P) = *QueueHead;
        *QueueHead = P;
    } else {
        Next(P) = Next(prev);
        Next(prev) = P;
    }
}

// Insert a node after a specific node
void InsertAfter(List *L, address P, address Prec) {
    Next(P) = Next(Prec);
    Next(Prec) = P;
}

// Delete the first node
void DeleteFirst(List *L, address *P) {
    *P = First(*L);
    if (*P != Nil) {
        First(*L) = Next(*P);
        Next(*P) = Nil;
    }
}

// Delete the first node in a queue
void DeleteFirst_Queue(address *QueueHead, address *P) {
    *P = *QueueHead;
    if (*P != Nil) {
        *QueueHead = Next(*P);
        Next(*P) = Nil;
    }
}

// Delete the last node
void DeleteLast(List *L, address *P) {
    if (First(*L) != Nil) {
        if (Next(First(*L)) == Nil) {
            DeleteFirst(L, P);
        } else {
            address Last = First(*L);
            while (Next(Next(Last)) != Nil) {
                Last = Next(Last);
            }
            *P = Next(Last);
            Next(Last) = Nil;
        }
    }
}

// Delete a node after a specific node
void DeleteAfter(List *L, address *P, address Prec) {
    *P = Next(Prec);
    if (*P != Nil) {
        Next(Prec) = Next(*P);
        Next(*P) = Nil;
    }
}

// Delete a node after a specific node in a queue
void DeleteAfter_Queue(address *QueueHead, address *P, address Prec) {
    *P = Next(Prec);
    if (*P != Nil) {
        Next(Prec) = Next(*P);
        Next(*P) = Nil;
    }
}

// Print all elements in the list
void PrintInfo(List L) {
    int Nomor = 1;
    address P = First(L);
    while (P != Nil) {
        printf("%d. %s (%d)\n", Nomor, Info(P), Int(P));
        Nomor++;
        P = Next(P);
    }
}

// Print all books in the array
void PrintBookArray(BookArray A) {
    int i;
	for (i = 0; i < Count(A); i++) {
        printf("%d. %s (Stock: %d)\n", i+1, Title(BookAt(A, i)), Stock(BookAt(A, i)));
    }
}

// Check if the list is empty
boolean IsEmpty(List L) {
    return (First(L) == Nil);
}

// Check if the queue is empty
boolean IsQueueEmpty(address QueueHead) {
    return (QueueHead == Nil);
}

// Search for a node with specific info
address Search(List L, InfoType X) {
    address P = First(L);
    while (P != Nil) {
        if (strcmp(Info(P), X) == 0) {
            return P;
        }
        P = Next(P);
    }
    return Nil;
}

// Search for a book with specific title
int SearchBook(BookArray A, InfoType X) {
    int i; 
	for (i = 0; i < Count(A); i++) {
        if (Title(BookAt(A, i)) != NULL && strcmp(Title(BookAt(A, i)), X) == 0) {
            return i;
        }
    }
    return -1;
}

// Add a book to the array
void AddBook(BookArray *A, InfoType X, int Stock) {
    if (Count(*A) < MAX_BOOKS) {
        int idx = Count(*A);
        Title(BookAt(*A, idx)) = strdup(X);
        Stock(BookAt(*A, idx)) = Stock;
        Queue(BookAt(*A, idx)) = Nil;
        Count(*A)++;
    } else {
        printf("Book array is full!\n");
    }
}

// Invert the list in place
void InversList(List *L) {
    address P = First(*L);
    address Prev = Nil;
    address NextNode;
    
    while (P != Nil) {
        NextNode = Next(P);
        Next(P) = Prev;
        Prev = P;
        P = NextNode;
    }
    
    First(*L) = Prev;
}
