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

extern BookArray bukuArray;
extern List BorrowedList;

typedef struct {
    InfoType nama;
    InfoType judulBuku;
    address next;
} Borrower;

address AlokasiBorrowed(InfoType nama, InfoType judulBuku);
void DealokasiBorrower(address P);
void printQueueForBook(Book book, int bookIndex);
void printAllBookQueues(BookArray A);
void AddBorrower(List *L, InfoType nama, InfoType judulBuku);
void RemoveBorrower(List *L, InfoType nama, InfoType judulBuku);
void printBorrower(List L);
void processQueue(Book *book, int addedStock);

#endif
