/* 
    File name    : LLBukuOrang.c
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : Header file for implementation of linked list Orang
*/

#include "LLBukuOrang.h"

List** OrangList = Nil;
int orangListSize = 0;

List** ReallocOrangList(List** lists, int bookIndex) {
    if (bookIndex < 0) {
        return lists; // Don't process negative indices
    }
    
    // If list is NULL, create it
    if (lists == NULL) {
        int i, j, newSize = bookIndex + 1;
        lists = (List**)malloc(newSize * sizeof(List*));
        if (lists == NULL) {
            printf("Memory allocation failed!\n");
            return NULL;
        }
        
        for (i = 0; i < newSize; i++) {
            lists[i] = (List*)malloc(sizeof(List));
            if (lists[i] == NULL) {
                printf("Memory allocation failed for list %d!\n", i);
                // Free already allocated memory
                for (j = 0; j < i; j++) {
                    free(lists[j]);
                }
                free(lists);
                return NULL;
            }
            CreateList(lists[i]); // Initialize the list
        }
        orangListSize = newSize; // Track the new size
        return lists;
    }
    
    // If we need to expand the list
    if (bookIndex >= orangListSize) {
        int oldSize = orangListSize;
        int newSize = bookIndex + 1;
        
        // Reallocate the list pointers array
        List** newLists = (List**)realloc(lists, newSize * sizeof(List*));
        if (newLists == NULL) {
            printf("Memory reallocation failed!\n");
            return lists; // Return original list if realloc fails
        }
        lists = newLists;
        
        // Initialize new list pointers
        int i;
        for (i = oldSize; i < newSize; i++) {
            lists[i] = (List*)malloc(sizeof(List));
            if (lists[i] == NULL) {
                printf("Memory allocation failed for list %d!\n", i);
                orangListSize = i; // Update size to what we managed to allocate
                return lists;
            }
            CreateList(lists[i]); // Initialize the list
        }
        orangListSize = newSize; // Update the size counter
    }
    
    return lists;
}

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


// Function to get the number of lists
int getNumLists(List** L) {
    if (L == Nil) return 0;
    int count = 0;
    while (L[count] != Nil) {
		count++;
	}
    return count;
}

void printOrangList(List** L, List Buku) {
    if (L == NULL) {
        printf("No queues for borrowing books.\n");
        return;
    }

    address currentBook = First(Buku);
    int bookCounter = 0;

    while (currentBook != NULL) {
        printf("%d. %s Queue:\n", bookCounter + 1, Info(currentBook));
        
        if (bookCounter < orangListSize && L[bookCounter] != NULL) {
            if (!IsEmpty(*L[bookCounter])) {
                address currentBorrower = First(*L[bookCounter]);
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
            printf("   Queue not initialized.\n");
        }
        
        currentBook = Next(currentBook);
        bookCounter++;
        printf("\n");
    }
}

void AddBorrower (List *L, InfoType nama, InfoType judulBuku) {
    address newP = AlokasiBorrowed(nama, judulBuku);
    InsertFirst(L, newP);
}

void RemoveBorrower (List *L, InfoType nama, InfoType judulBuku) {
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
            DealokasiBorrower(current); // Use the new function
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
