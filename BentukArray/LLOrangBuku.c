/* 
    File name    : LLBukuOrang.c
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : Implementation of linked list Orang
*/

#include "LLBukuOrang.h"

BookArray bukuArray;
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

// Print the queue for a specific book
void printQueueForBook(Book book, int bookIndex) {
    printf("%d. %s Queue:\n", bookIndex + 1, Title(book));
    
    if (!IsQueueEmpty(Queue(book))) {
        address currentBorrower = Queue(book);
        int pos = 1;
        while (currentBorrower != NULL) {
            printf("   %d. %s (Priority: %d)\n", pos, Info(currentBorrower), Int(currentBorrower));
            currentBorrower = Next(currentBorrower);
            pos++;
        }
    } else {
        printf("   No one in queue.\n");
    }
    printf("\n");
}

// Print all book queues
void printAllBookQueues(BookArray A) {
    if (Count(A) == 0) {
        printf("No books available.\n");
        return;
    }
	
	int i; 
    for (i = 0; i < Count(A); i++) {
        printQueueForBook(BookAt(A, i), i);
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
                Next(current) = Nil;
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

// Process queue for a book when stock is added
void processQueue(Book *book, int addedStock) {
    if (addedStock <= 0 || IsQueueEmpty(Queue(*book))) {
        return; // Exit if no stock is added or queue is empty
    }

    // Keep track of how many books we've processed
    int stockProcessed = 0;
    while (stockProcessed < addedStock && !IsQueueEmpty(Queue(*book))) {
        // Get the first person in queue
        address borrower;
        DeleteFirst_Queue(&Queue(*book), &borrower);
        
        if (borrower != NULL) {
            // Add to borrowed list
            address newBorrowed = AlokasiBorrowed(Info(borrower), Title(*book));
            InsertFirst(&BorrowedList, newBorrowed);
            
            // Print message
            printf("Book '%s' has been given to '%s'!\n", Title(*book), Info(borrower));
            
            // Decrement stock
            Stock(*book)--;
            stockProcessed++;
            
            // Free the queue node
            Dealokasi(borrower);
        }
    }
}
