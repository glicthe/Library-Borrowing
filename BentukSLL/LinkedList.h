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
} Node;

// Structure for the linked list
typedef struct {
    address First;
} List;

// Macros for easier access
#define Nil NULL
#define Info(P) (P)->Info
#define Int(P) (P)->StockOrPrioritas
#define Next(P) (P)->Next
#define First(L) (L).First

// Function declarations
void CreateList(List *L);
/*
void CreateListToBook(List *L, address P);
boolean isListCreated(List *L, address P, int i);
*/
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
/*
List FInversList(List L);
void CopyList(List *L1, List *L2);
List FCopyList(List L);
void CpAlokList(List Lin, List *Lout);
void CpAlokList2(List Lin, List *Lout);
void Konkat(List L1, List L2, List *L3);
void Konkat1(List *L1, List L2, List *L3);
void PecahList(List *L1, List *L2, List L);
*/
#endif
