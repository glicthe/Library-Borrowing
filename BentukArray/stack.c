/* 
    File name    : stack.c
    Made by      : Fauzan Juniar Mulyana
    Date         : 07 May 2025
    Description  : Implementation of stack for undo functionality
*/

#include "stack.h"

// External references to global variables from Menu.h
extern BookArray bukuArray;
extern List BorrowedList;

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
    A.bookIndex = -1; // Will be set when we add the book
    
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
    int bookIndex = SearchBook(bukuArray, A.title);
    if (bookIndex == -1) {
        printf("Cannot undo: Book not found!\n");
        return;
    }
    
    // We need to remove the book from the array
    // First, free any memory associated with the book
    if (Title(BookAt(bukuArray, bookIndex)) != NULL) {
        free(Title(BookAt(bukuArray, bookIndex)));
        Title(BookAt(bukuArray, bookIndex)) = NULL;
    }
    
    // Free any memory in the queue
    address current = Queue(BookAt(bukuArray, bookIndex));
    while (current != NULL) {
        address next = Next(current);
        Dealokasi(current);
        current = next;
    }
    Queue(BookAt(bukuArray, bookIndex)) = Nil;
    
    // Shift all books after this one back by one
    int i;
    for (i = bookIndex; i < Count(bukuArray) - 1; i++) {
        Title(BookAt(bukuArray, i)) = Title(BookAt(bukuArray, i + 1));
        Stock(BookAt(bukuArray, i)) = Stock(BookAt(bukuArray, i + 1));
        Queue(BookAt(bukuArray, i)) = Queue(BookAt(bukuArray, i + 1));
    }
    
    // Clear the last book (now duplicated)
    Title(BookAt(bukuArray, Count(bukuArray) - 1)) = NULL;
    Stock(BookAt(bukuArray, Count(bukuArray) - 1)) = 0;
    Queue(BookAt(bukuArray, Count(bukuArray) - 1)) = Nil;
    
    // Decrease the book count
    Count(bukuArray)--;
    
    printf("Undone: Removed book '%s'\n", A.title);
}

// Undo adding stock to a book
void UndoAddStock(Action A) {
    int bookIndex = SearchBook(bukuArray, A.title);
    if (bookIndex == -1) {
        printf("Cannot undo: Book not found!\n");
        return;
    }
    
    // Reduce stock by the amount that was added
    Stock(BookAt(bukuArray, bookIndex)) -= A.value;
    printf("Undone: Removed %d stock from '%s'\n", A.value, A.title);
    
    // Make sure stock doesn't go negative
    if (Stock(BookAt(bukuArray, bookIndex)) < 0) {
        Stock(BookAt(bukuArray, bookIndex)) = 0;
        printf("Warning: Stock was set to 0 to avoid negative values\n");
    }
}

// Undo borrowing a book
void UndoBorrowBook(Action A) {
    int bookIndex = SearchBook(bukuArray, A.title);
    if (bookIndex == -1) {
        printf("Cannot undo: Book not found!\n");
        return;
    }
    
    Book *book = &BookAt(bukuArray, bookIndex);
    
    // Case 1: Book was immediately borrowed (removed from stock)
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
                Stock(*book)++;
                printf("Undone: Book '%s' returned by '%s'\n", A.title, A.name);
            }
        } else {
            prev = current;
            current = Next(current);
        }
    }
    
    // Case 2: Person was added to queue
    if (!found) {
        // Re-add person back to the queue with their priority
        address borrower = Alokasi(A.name, A.value);
        InsertPriority(&Queue(*book), borrower);
        printf("Undone: Re-added '%s' to queue for '%s'\n", A.name, A.title);
    }
}

// Undo returning a book
void UndoReturnBook(Action A) {
    int bookIndex = SearchBook(bukuArray, A.title);
    if (bookIndex == -1) {
        printf("Cannot undo: Book not found!\n");
        return;
    }
    
    Book *book = &BookAt(bukuArray, bookIndex);
    
    // Decrease stock (since it was increased when the book was returned)
    if (Stock(*book) > 0) {
        Stock(*book)--;
        
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
    int bookIndex = SearchBook(bukuArray, A.title);
    if (bookIndex == -1) {
        printf("Cannot undo: Invalid book index!\n");
        return;
    }
    
    Book *book = &BookAt(bukuArray, bookIndex);
    
    // Add person back to the queue with their priority
    address borrower = Alokasi(A.name, A.value);
    InsertPriority(&Queue(*book), borrower);
    
    printf("Undone: Re-added '%s' to queue for '%s'\n", A.name, A.title);
}
