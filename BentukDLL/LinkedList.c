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
    Last(*L) = Nil;  // Initialize Last pointer
}


// Allocate memory for a new node with string info
address Alokasi(InfoType X, int StockOrPrioritas) {
    address P = (address)malloc(sizeof(Node));
    if (P != Nil) {
        Info(P) = strdup(X);  // Make a copy of the string
        Int(P) = StockOrPrioritas;
        Next(P) = Nil;
        Prev(P) = Nil;    // Initialize Prev pointer
        Queue(P) = Nil;   // Initialize Queue pointer
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
    if (IsEmpty(*L)) {
        First(*L) = P;
        Last(*L) = P;
    } else {
        Next(P) = First(*L);
        Prev(First(*L)) = P;  // Link the old first to new P
        First(*L) = P;
    }
}

// Insert a node at the end of the list
void InsertLast(List *L, address P) {
    if (IsEmpty(*L)) {
        InsertFirst(L, P);
    } else {
        Next(Last(*L)) = P;
        Prev(P) = Last(*L);
        Last(*L) = P;
    }
}

void InsertPriority(List *L, address P) {
    if (IsEmpty(*L)) {
        InsertFirst(L, P);
        return;
    }

    address current = First(*L);
    
    // If should be inserted at the beginning
    if (Int(current) > Int(P)) {
        InsertFirst(L, P);
        return;
    }

    // Find position based on priority
    while (current != Nil && Int(current) <= Int(P)) {
        if (Next(current) == Nil || Int(Next(current)) > Int(P)) {
            break;
        }
        current = Next(current);
    }

    // Insert after current
    InsertAfter(L, P, current);
}

// Insert a node after a specific node
void InsertAfter(List *L, address P, address Prec) {
    if (Prec == Last(*L)) {
        InsertLast(L, P);
    } else {
        Next(P) = Next(Prec);
        Prev(P) = Prec;
        Prev(Next(Prec)) = P;
        Next(Prec) = P;
    }
}

// Delete the first node
void DeleteFirst(List *L, address *P) {
    *P = First(*L);
    if (*P != Nil) {
        if (First(*L) == Last(*L)) {  // Only one element
            First(*L) = Nil;
            Last(*L) = Nil;
        } else {
            First(*L) = Next(*P);
            Prev(First(*L)) = Nil;
            Next(*P) = Nil;
            Prev(*P) = Nil;
        }
    }
}

// Delete the last node
void DeleteLast(List *L, address *P) {
    if (!IsEmpty(*L)) {
        *P = Last(*L);
        if (First(*L) == Last(*L)) {  // Only one element
            First(*L) = Nil;
            Last(*L) = Nil;
        } else {
            Last(*L) = Prev(*P);
            Next(Last(*L)) = Nil;
            Prev(*P) = Nil;
        }
    }
}

// Delete a node after a specific node
void DeleteAfter(List *L, address *P, address Prec) {
    if (Prec != Nil && Next(Prec) != Nil) {
        *P = Next(Prec);
        if (*P == Last(*L)) {  // If deleting the last node
            DeleteLast(L, P);
        } else {
            Next(Prec) = Next(*P);
            Prev(Next(*P)) = Prec;
            Next(*P) = Nil;
            Prev(*P) = Nil;
        }
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

// Check if the list is empty
boolean IsEmpty(List L) {
    return (First(L) == Nil);
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

// Count the number of elements in the list
int countBook(List L, InfoType X) {
    int count = 0;
    address P = First(L);
    while (P != NULL) {
        count++;
        if (strcmp(Info(P), X) == 0) {
            return count; // 1-based position
        }
        P = Next(P);
    }
    return 0;  // Return 0 if not found (added for safety)
}

// Invert the list in place
void InversList(List *L) {
    if (IsEmpty(*L) || First(*L) == Last(*L)) {
        return;  // Nothing to do for empty list or single element
    }
    
    address P = First(*L);
    address temp;
    
    // Swap First and Last
    Last(*L) = First(*L);
    First(*L) = Nil;
    
    while (P != Nil) {
        // Save next node
        temp = Next(P);
        
        // Swap next and prev pointers
        Next(P) = Prev(P);
        Prev(P) = temp;
        
        // If this was the last node, it becomes the first
        if (Prev(P) == Nil) {
            First(*L) = P;
        }
        
        // Move to next node (which is now in Prev)
        P = Prev(P);
    }
}
