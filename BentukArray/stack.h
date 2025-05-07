/* 
    File name    : stack.h
    Made by      : fauzan Juniar mulyana
    Date         : 07 May 2025
    Description  : Header file for stack implementation for undo functionality
*/

#ifndef STACK_H
#define STACK_H

#include "LinkedList.h"
#include "LLBukuOrang.h"

// Define the types of actions that can be undone
typedef enum {
    ACTION_ADD_BOOK,
    ACTION_ADD_STOCK,
    ACTION_BORROW_BOOK,
    ACTION_RETURN_BOOK,
    ACTION_CANCEL_BORROWING
} ActionType;

// Structure to store information about an action
typedef struct {
    ActionType type;
    InfoType title;       // Book title
    InfoType name;        // Person name (if applicable)
    int value;            // Stock value or priority
    int bookIndex;        // Book index in the array
} Action;

// Stack node for storing actions
typedef struct StackNode {
    Action action;
    struct StackNode* next;
} StackNode;

// Stack structure
typedef struct {
    StackNode* top;
} ActionStack;

// Function declarations
void CreateStack(ActionStack* S);
int IsStackEmpty(ActionStack S);
void Push(ActionStack* S, Action A);
int Pop(ActionStack* S, Action* A);
void ClearStack(ActionStack* S);

// Action recording functions
void RecordAddBook(ActionStack* S, InfoType title, int stock);
void RecordAddStock(ActionStack* S, InfoType title, int stock, int bookIndex);
void RecordBorrowBook(ActionStack* S, InfoType title, InfoType name, int priority, int bookIndex);
void RecordReturnBook(ActionStack* S, InfoType title, InfoType name);
void RecordCancelBorrowing(ActionStack* S, InfoType title, InfoType name, int priority, int bookIndex);

// Action undo functions
void UndoLastAction(ActionStack* S);
void UndoAddBook(Action A);
void UndoAddStock(Action A);
void UndoBorrowBook(Action A);
void UndoReturnBook(Action A);
void UndoCancelBorrowing(Action A);

#endif
