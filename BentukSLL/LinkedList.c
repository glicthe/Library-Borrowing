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

void InsertPriority(List *L, address P) {
    if (IsEmpty(*L)) {
        InsertFirst(L, P);
        return;
    }

    address current = First(*L);
    address prev = Nil;

    while (current != Nil && Int(current) <= Int(P)) {
        prev = current;
        current = Next(current);
    }

    if (prev == Nil) {
        InsertFirst(L, P);
    } else {
        InsertAfter(L, P, prev);
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
/*
// Create a new inverted list
List FInversList(List L) {
    List LOut;
    CreateList(&LOut);
    
    address P = First(L);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertFirst(&LOut, Q);
        P = Next(P);
    }
    
    return LOut;
}


// Copy list L1 to L2
void CopyList(List *L1, List *L2) {
    CreateList(L2);
    address P = First(*L1);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertLast(L2, Q);
        P = Next(P);
    }
}

// Create a new copy of list L
List FCopyList(List L) {
    List LOut;
    CreateList(&LOut);
    
    address P = First(L);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertLast(&LOut, Q);
        P = Next(P);
    }
    
    return LOut;
}

// Copy list with allocation
void CpAlokList(List Lin, List *Lout) {
    CreateList(Lout);
    address P = First(Lin);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertLast(Lout, Q);
        P = Next(P);
    }
}

// Alternative copy with allocation
void CpAlokList2(List Lin, List *Lout) {
    CreateList(Lout);
    address P = First(Lin);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertFirst(Lout, Q);
        P = Next(P);
    }
    InversList(Lout);
}

// Concatenate two lists
void Konkat(List L1, List L2, List *L3) {
    CreateList(L3);
    address P = First(L1);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertLast(L3, Q);
        P = Next(P);
    }
    
    P = First(L2);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertLast(L3, Q);
        P = Next(P);
    }
}

// Concatenate lists and empty L1
void Konkat1(List *L1, List L2, List *L3) {
    CreateList(L3);
    address P = First(*L1);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertLast(L3, Q);
        P = Next(P);
    }
    
    P = First(L2);
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        InsertLast(L3, Q);
        P = Next(P);
    }
    
    CreateList(L1);
}

// Split list into two lists
void PecahList(List *L1, List *L2, List L) {
    CreateList(L1);
    CreateList(L2);
    
    int count = NbElmt(L);
    int mid = count / 2;
    
    address P = First(L);
    int i = 0;
    
    while (P != Nil) {
        address Q = Alokasi(Info(P));
        if (i < mid) {
            InsertLast(L1, Q);
        } else {
            InsertLast(L2, Q);
        }
        P = Next(P);
        i++;
    }
}
*/
