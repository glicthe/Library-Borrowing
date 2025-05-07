/* 
    File name    : LLBukuOrang.h
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : Header file for implementation of linked list Orang
*/

#ifndef LLBUKUORANG_H
#define LLBUKUORANG_H

#include "LinkedList.h"

#define nama(P) P->nama
#define judul(P) P->judulBuku
#define next(P) P->next

// We don't need OrangList anymore as each book points to its own queue
// Kept for backward compatibility but will be unused
extern List** OrangList;
extern int orangListSize;

typedef struct {
    InfoType nama;
    InfoType judulBuku;
    address next;
} Borrower;

extern List BorrowedList;

address AlokasiBorrowed(InfoType nama, InfoType judulBuku);
void DealokasiBorrower(address P);
void printBookQueues(List Buku);
void AddBorrower(List *L, InfoType nama, InfoType judulBuku);
void RemoveBorrower(List *L, InfoType nama, InfoType judulBuku);
void printBorrower(List L);
address FindBookByTitle(List Buku, InfoType title);
List* GetQueueFromBook(address bookNode);

#endif

