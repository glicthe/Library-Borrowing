/* 
    File name    : LinkedListBUKu.h
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : Header file for implementation of linked list
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define boolean type
typedef enum { false, true } boolean;

// Define InfoType as char* (string)
typedef char* InfoType;

// Structure for a node in the linked list of people (borrowers)
typedef struct tNode* address;
typedef struct tNode {
    InfoType Info;
    int StockOrPrioritas;
    address Next;
} Node;

// Structure for a book in the array
typedef struct {
    InfoType title;
    int stock;
    address queue; // Points to the first person in queue for this book
} Book;

// Maximum number of books
#define MAX_BOOKS 100

// Structure for the book array
typedef struct {
    Book books[MAX_BOOKS];
    int count;  // Number of books currently in the array
} BookArray;

// Structure for the borrower list
typedef struct {
    address First;
} List;

// Macros for easier access
#define Nil NULL
#define Info(P) (P)->Info
#define Int(P) (P)->StockOrPrioritas
#define Next(P) (P)->Next
#define First(L) (L).First

// Book array macros
#define Title(B) (B).title
#define Stock(B) (B).stock
#define Queue(B) (B).queue
#define Count(A) (A).count
#define BookAt(A, i) (A).books[i]

// Function declarations
void CreateList(List *L);
void CreateBookArray(BookArray *A);
address Alokasi(InfoType X, int StockOrPrioritas);
void Dealokasi(address P);
void InsertFirst(List *L, address P);
void InsertLast(List *L, address P);
void InsertPriority(address *QueueHead, address P);
void InsertAfter(List *L, address P, address Prec);
void DeleteFirst(List *L, address *P);
void DeleteFirst_Queue(address *QueueHead, address *P);
void DeleteLast(List *L, address *P);
void DeleteAfter(List *L, address *P, address Prec);
void DeleteAfter_Queue(address *QueueHead, address *P, address Prec);
void PrintInfo(List L);
void PrintBookArray(BookArray A);
boolean IsEmpty(List L);
boolean IsQueueEmpty(address QueueHead);
address Search(List L, InfoType X);
int SearchBook(BookArray A, InfoType X);
void AddBook(BookArray *A, InfoType X, int Stock);
void InversList(List *L);

#endif
