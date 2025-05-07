/* 
    File name    : stack.c
    Made by      : Fauzan Juniar Mulyana
    Date         : 07 May 2025
    Description  : Implementation of stack for undo functionality
*/

#include "stack.h"

// External references to global variables from Menu.h
extern List bukuList;
extern List BorrowedList;
extern List** OrangList;
extern int orangListSize;

// Create an empty stack
void CreateStack(ActionStack* S) {
    S->top = NULL;
}

// Check if stack is empty
int IsStackEmpty(ActionStack S) {
    return (S.top == NULL);
}

// Push an action onto the stack
void Push(ActionStack* S, Action A) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    // Copy action data
    newNode->action = A;
    
    // Allocate memory for strings
    if (A.title != NULL) {
        newNode->action.title = strdup(A.title);
    } else {
        newNode->action.title = NULL;
    }
    
    if (A.name != NULL) {
        newNode->action.name = strdup(A.name);
    } else {
        newNode->action.name = NULL;
    }
    
    // Add to top of stack
    newNode->next = S->top;
    S->top = newNode;
}

// Pop an action from the stack
int Pop(ActionStack* S, Action* A) {
    if (IsStackEmpty(*S)) {
        return 0; // Stack is empty
    }
    
    StackNode* temp = S->top;
    
    // Copy data to output parameter
    *A = temp->action;
    
    // We'll need to make new copies of strings before freeing the node
    if (temp->action.title != NULL) {
        A->title = strdup(temp->action.title);
    }
    
    if (temp->action.name != NULL) {
        A->name = strdup(temp->action.name);
    }
    
    // Remove node from stack
    S->top = temp->next;
    
    // Free memory
    free(temp->action.title);
    free(temp->action.name);
    free(temp);
    
    return 1; // Success
}

// Clear all elements from the stack
void ClearStack(ActionStack* S) {
    Action A;
    while (Pop(S, &A)) {
        // Free memory allocated by Pop
        free(A.title);
        free(A.name);
    }
}

// Record adding a new book
void RecordAddBook(ActionStack* S, InfoType title, int stock) {
    Action A;
    A.type = ACTION_ADD_BOOK;
    A.title = title;
    A.name = NULL;
    A.value = stock;
    A.bookIndex = -1; // Will not be used for this action
    
    Push(S, A);
}

// Record adding stock to a book
void RecordAddStock(ActionStack* S, InfoType title, int stock, int bookIndex) {
    Action A;
    A.type = ACTION_ADD_STOCK;
    A.title = title;
    A.name = NULL;
    A.value = stock;
    A.bookIndex = bookIndex;
    
    Push(S, A);
}

// Record borrowing a book
void RecordBorrowBook(ActionStack* S, InfoType title, InfoType name, int priority, int bookIndex) {
    Action A;
    A.type = ACTION_BORROW_BOOK;
    A.title = title;
    A.name = name;
    A.value = priority;
    A.bookIndex = bookIndex;
    
    Push(S, A);
}

// Record returning a book
void RecordReturnBook(ActionStack* S, InfoType title, InfoType name) {
    Action A;
    A.type = ACTION_RETURN_BOOK;
    A.title = title;
    A.name = name;
    A.value = 0; // Not used for this action
    A.bookIndex = -1; // Will be computed during undo if needed
    
    Push(S, A);
}

// Record canceling a borrowing
void RecordCancelBorrowing(ActionStack* S, InfoType title, InfoType name, int priority, int bookIndex) {
    Action A;
    A.type = ACTION_CANCEL_BORROWING;
    A.title = title;
    A.name = name;
    A.value = priority;
    A.bookIndex = bookIndex;
    
    Push(S, A);
}

// Undo the last action
void UndoLastAction(ActionStack* S) {
    Action A;
    if (!Pop(S, &A)) {
        printf("No action to undo!\n");
        return;
    }
    
    // Call appropriate undo function based on action type
    switch (A.type) {
        case ACTION_ADD_BOOK:
            UndoAddBook(A);
            break;
        case ACTION_ADD_STOCK:
            UndoAddStock(A);
            break;
        case ACTION_BORROW_BOOK:
            UndoBorrowBook(A);
            break;
        case ACTION_RETURN_BOOK:
            UndoReturnBook(A);
            break;
        case ACTION_CANCEL_BORROWING:
            UndoCancelBorrowing(A);
            break;
        default:
            printf("Unknown action type!\n");
    }
    
    // Free memory allocated by Pop
    free(A.title);
    free(A.name);
}

// Undo adding a book
void UndoAddBook(Action A) {
    address P = Search(bukuList, A.title);
    if (P == NULL) {
        printf("Cannot undo: Book not found!\n");
        return;
    }
    
    // Find the book to remove
    address prev = NULL;
    address current = First(bukuList);
    
    while (current != NULL && current != P) {
        prev = current;
        current = Next(current);
    }
    
    if (current == NULL) {
        printf("Cannot undo: Book not found in list!\n");
        return;
    }
    
    address removed;
    if (prev == NULL) {
        // It's the first book in the list
        DeleteFirst(&bukuList, &removed);
    } else {
        // It's somewhere in the middle or at the end
        DeleteAfter(&bukuList, &removed, prev);
    }
    
    if (removed != NULL) {
        printf("Undone: Removed book '%s'\n", Info(removed));
        Dealokasi(removed);
    }
}

// Undo adding stock to a book
void UndoAddStock(Action A) {
    address book = Search(bukuList, A.title);
    if (book == NULL) {
        printf("Cannot undo: Book not found!\n");
        return;
    }
    
    // Reduce stock by the amount that was added
    Int(book) -= A.value;
    printf("Undone: Removed %d stock from '%s'\n", A.value, A.title);
    
    // Make sure stock doesn't go negative
    if (Int(book) < 0) {
        Int(book) = 0;
        printf("Warning: Stock was set to 0 to avoid negative values\n");
    }
}

// Undo borrowing a book
void UndoBorrowBook(Action A) {
    // Case 1: Book was immediately borrowed (removed from stock)
    address book = Search(bukuList, A.title);
    if (book == NULL) {
        printf("Cannot undo: Book not found!\n");
        return;
    }
    
    // Check if this person has the book in borrowed list
    address current = First(BorrowedList);
    address prev = NULL;
    boolean found = false;
    
    while (current != NULL && !found) {
        Borrower* b = (Borrower*)current;
        if (A.name != NULL && A.title != NULL && 
            strcmp(nama(b), A.name) == 0 && 
            strcmp(judul(b), A.title) == 0) {
            
            found = true;
            
            // Remove from borrowed list
            address removed;
            if (prev == NULL) {
                DeleteFirst(&BorrowedList, &removed);
            } else {
                DeleteAfter(&BorrowedList, &removed, prev);
            }
            
            if (removed != NULL) {
                DealokasiBorrower(removed);
                
                // Increase stock
                Int(book)++;
                printf("Undone: Book '%s' returned by '%s'\n", A.title, A.name);
            }
        } else {
            prev = current;
            current = Next(current);
        }
    }
    
    // Case 2: Person was added to queue
    if (!found && A.bookIndex >= 0 && A.bookIndex < orangListSize) {
        // Add person back to the queue with their priority
        address borrower = Alokasi(A.name, A.value);
        InsertPriority(OrangList[A.bookIndex], borrower);
        printf("Undone: Re-added '%s' to queue for '%s'\n", A.name, A.title);
    }
}

// Undo returning a book
void UndoReturnBook(Action A) {
    address book = Search(bukuList, A.title);
    if (book == NULL) {
        printf("Cannot undo: Book not found!\n");
        return;
    }
    
    // Decrease stock (since it was increased when the book was returned)
    if (Int(book) > 0) {
        Int(book)--;
        
        // Add back to borrowed list
        address newBorrowed = AlokasiBorrowed(A.name, A.title);
        InsertFirst(&BorrowedList, newBorrowed);
        
        printf("Undone: Book '%s' is borrowed again by '%s'\n", A.title, A.name);
    } else {
        printf("Cannot undo: No stock available!\n");
    }
}

// Undo canceling a borrowing request
void UndoCancelBorrowing(Action A) {
    if (A.bookIndex < 0 || A.bookIndex >= orangListSize) {
        printf("Cannot undo: Invalid book index!\n");
        return;
    }
    
    // Add person back to the queue with their priority
    address borrower = Alokasi(A.name, A.value);
    InsertPriority(OrangList[A.bookIndex], borrower);
    
    printf("Undone: Re-added '%s' to queue for '%s'\n", A.name, A.title);
}
