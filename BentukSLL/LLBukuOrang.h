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

extern List** OrangList;
extern orangListSize;

typedef struct {
    InfoType nama;
    InfoType judulBuku;
    address next;
} Borrower;

extern List BorrowedList;

address AlokasiBorrowed(InfoType nama, InfoType judulBuku);
void DealokasiBorrower(address P);
List** ReallocOrangList(List** lists, int bookIndex);
int getNumLists(List** lists);
void printOrangList(List** L, List Buku);
void AddBorrower(List *L, InfoType nama, InfoType judulBuku);
void RemoveBorrower(List *L, InfoType nama, InfoType judulBuku);
void printBorrower(List L);

#endif
