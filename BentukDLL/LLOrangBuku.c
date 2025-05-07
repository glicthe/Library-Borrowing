/* 
    File name    : LLBukuOrang.c
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : Header file for implementation of linked list Orang
*/

#include "LLBukuOrang.h"

// Keep these variables for backward compatibility
// They won't be actively used in the new implementation
List** OrangList = Nil;
int orangListSize = 0;

List BorrowedList;

address AlokasiBorrowed(InfoType nama, InfoType judulBuku) {
    Borrower* newBorrower = (Borrower*)malloc(sizeof(Borrower));
    if (newBorrower != NULL) {
        nama(newBorrower) = strdup(nama);
        judul(newBorrower) = strdup(judulBuku);
        next(newBorrower) = NULL;
    }
    return (address)newBorrower;
}

void DealokasiBorrower(address P) {
    if (P != Nil) {
        Borrower* borrower = (Borrower*)P;
        free(borrower->nama);  // Free the nama string
        free(borrower->judulBuku);  // Free the judulBuku string
        free(P);  // Free the Borrower struct
    }
}

// New function to find a book by title
address FindBookByTitle(List Buku, InfoType title) {
    address book = First(Buku);
    while (book != NULL) {
        if (strcmp(Info(book), title) == 0) {
            return book;
        }
        book = Next(book);
    }
    return NULL;
}

// New function to get or create the queue for a book
List* GetQueueFromBook(address bookNode) {
    if (bookNode == NULL) {
        return NULL;
    }
    
    // If the book doesn't have a queue yet, create one
    if (Queue(bookNode) == NULL) {
        // Allocate memory for a new List
        List* newQueue = (List*)malloc(sizeof(List));
        if (newQueue == NULL) {
            printf("Memory allocation failed for queue!\n");
            return NULL;
        }
        CreateList(newQueue);
        
        // Set the book's Queue to point to this queue
        Queue(bookNode) = (address)newQueue;
        
        return newQueue;
    }
    
    // Return the existing queue
    return (List*)Queue(bookNode);
}

void printBookQueues(List Buku) {
    if (IsEmpty(Buku)) {
        printf("No books available.\n");
        return;
    }

    address currentBook = First(Buku);
    int bookCounter = 1;

    while (currentBook != NULL) {
        printf("%d. %s Queue:\n", bookCounter, Info(currentBook));
        
        if (Queue(currentBook) != NULL) {
            List* bookQueue = GetQueueFromBook(currentBook);
            if (bookQueue != NULL && !IsEmpty(*bookQueue)) {
                address currentBorrower = First(*bookQueue);
                int pos = 1;
                while (currentBorrower != NULL) {
                    printf("   %d. %s (Priority: %d)\n", pos, Info(currentBorrower), Int(currentBorrower));
                    currentBorrower = Next(currentBorrower);
                    pos++;
                }
            } else {
                printf("   No one in queue.\n");
            }
        } else {
            printf("   No queue established.\n");
        }
        
        currentBook = Next(currentBook);
        bookCounter++;
        printf("\n");
    }
}

void AddBorrower(List *L, InfoType nama, InfoType judulBuku) {
    address newP = AlokasiBorrowed(nama, judulBuku);
    InsertFirst(L, newP);
}

void RemoveBorrower(List *L, InfoType nama, InfoType judulBuku) {
    address prev = NULL;
    address current = First(*L);
    
    while (current != NULL) {
        Borrower* b = (Borrower*)current;
        if (strcmp(nama(b), nama) == 0 && strcmp(judul(b), judulBuku) == 0) {
            if (prev == NULL) {
                DeleteFirst(L, &current);
            } else {
                Next(prev) = Next(current);
                current = Next(prev);
            }
            DealokasiBorrower(current);
            return;
        }
        prev = current;
        current = Next(current);
    }
}

void printBorrower(List L) {
    address current = First(L);
    int count = 1;
    while (current != NULL) {
        Borrower* b = (Borrower*)current;
        printf("%d. %s - %s\n", count, nama(b), judul(b));
        current = Next(current);
        count++;
    }
}
