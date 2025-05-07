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

// Structure for a node in the linked list
typedef struct tNode* address;
typedef struct tNode {
    InfoType Info;
    int StockOrPrioritas;
    address Next;
    address Prev;    // Added for double linked list
    struct tNode* Queue;  // Pointer to the book's queue
} Node;

// Structure for the linked list
typedef struct {
    address First;
    address Last;    // Added for double linked list
} List;

// Macros for easier access
#define Nil NULL
#define Info(P) (P)->Info
#define Int(P) (P)->StockOrPrioritas
#define Next(P) (P)->Next
#define Prev(P) (P)->Prev    // New macro for Prev
#define Queue(P) (P)->Queue  // New macro for Queue
#define First(L) (L).First
#define Last(L) (L).Last     // New macro for Last

// Function declarations
void CreateList(List *L);
address Alokasi(InfoType X, int StockOrPrioritas);
void Dealokasi(address P);
void InsertFirst(List *L, address P);
void InsertLast(List *L, address P);
void InsertPriority(List *L, address P);
void InsertAfter(List *L, address P, address Prec);
void DeleteFirst(List *L, address *P);
void DeleteLast(List *L, address *P);
void DeleteAfter(List *L, address *P, address Prec);
void PrintInfo(List L);
boolean IsEmpty(List L);
address Search(List L, InfoType X);
int countBook(List L, InfoType X);
void InversList(List *L);

#endif
