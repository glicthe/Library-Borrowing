/* 
    File name    : Menu.c
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : for the Menu
*/

#include "Menu.h"
#define LENGTH 20

BookArray bukuArray;
List BorrowedList;
ActionStack undoStack;

//===============For the Main Menu===============//
int MainMenu() {
    char title[LENGTH];
    int pilihan, i, NumBook;
    CreateBookArray(&bukuArray);
    CreateList(&BorrowedList);
    CreateStack(&undoStack); // Initialize the undo stack

    do {
        printf("==============Welcome to Library==============\n");
        printf("1. Add new Book\n");
        printf("2. Add Stock\n");
        printf("3. Borrow a Book\n");
        printf("4. View the Book's Queue'\n");
        printf("5. Return a Book\n");
        printf("6. View the Borrowed Book\n");
        printf("7. Cancel Borrowing\n");
        printf("8. Undo Last Action\n"); // New option
        printf("9. Exit\n"); // Exit option moved to 9
        printf("Choose the Option: ");
        scanf("%d", &pilihan);
        clearBuffer();

        switch (pilihan) {
            case 1: {
                int stock;
                
                getNonEmptyInput("Enter the title: ", title, LENGTH);
                FormatSentence(title);
                
                printf("Enter the stock: ");
                scanf("%d", &stock);
                clearBuffer();
                
                // Add book to array
                AddBook(&bukuArray, title, stock);
                
                // Record action for undo
                RecordAddBook(&undoStack, title, stock);
                
                waitForEnter();
                break;
            }
            case 2: {
                int NewStock;
                PrintBookArray(bukuArray);
            
                getNonEmptyInput("What book's title you want to change?\n>>> ", title, LENGTH);
                FormatSentence(title);
            
                int bookIndex = SearchBook(bukuArray, title);
                if (bookIndex == -1) {
                    printf("Book not found!\n");
                    waitForEnter();
                    break;
                }
                
                printf("Current Stock: [%d]\n", Stock(BookAt(bukuArray, bookIndex)));
                printf("Add Stock: ");
                scanf("%d", &NewStock);
                clearBuffer();
            
                // Record action for undo before changing stock
                RecordAddStock(&undoStack, title, NewStock, bookIndex);
                
                // Add new stock to book
                Book *currentBook = &BookAt(bukuArray, bookIndex);
                Stock(*currentBook) += NewStock;
                printf("\nUpdated Stock: [%d]\n", Stock(*currentBook));
                
                // Check if there are people in queue who can now borrow the book
                if (!IsQueueEmpty(Queue(*currentBook))) {
                    printf("\nProcessing queue for book '%s'...\n", Title(*currentBook));
                    processQueue(currentBook, NewStock);
                }
                
                waitForEnter();
                break;
            }
            case 3: {
                char Name[LENGTH];
                int priority = 0;
                
                // Showing all of the book
                PrintBookArray(bukuArray);
                
                // Choose the book
                printf("\nwhat book you want to borrow?\n");
                getNonEmptyInput("Enter the title: ", title, LENGTH);
                FormatSentence(title);
                
                int bookIndex = SearchBook(bukuArray, title);
                if (bookIndex == -1) {
                    printf("Book not found!\n");
                    waitForEnter();
                    break;
                }
                
                Book *currentBook = &BookAt(bukuArray, bookIndex);
                
                // Enter user's name
                printf("\n\nWho's Borrowing?\n");
                getNonEmptyInput("Enter your Name: ", Name, LENGTH);
                FormatSentence(Name);
                
                // choose priority
                while(priority == 0) {
                    printf("\n\nWhat's your position?");
                    printf("\n1. Lecturer");
                    printf("\n2. Student");
                    printf("\n3. Visitor");
                    printf("\nChoose the Option: ");
                    scanf("%d", &priority);
                    clearBuffer();
                
                    if (priority < 1 || priority > 3) {
                        printf("Invalid priority!\n");
                        waitForEnter();
                        priority = 0;
                        continue;
                    } else break;
                }
                
                // Record action for undo
                RecordBorrowBook(&undoStack, title, Name, priority, bookIndex);
            
                if (Stock(*currentBook) > 0 && IsQueueEmpty(Queue(*currentBook))) {
                    // Immediate borrowing if queue is empty
                    Stock(*currentBook)--;
                    address NewBorrowed = AlokasiBorrowed(Name, title);
                    InsertFirst(&BorrowedList, NewBorrowed);
                    printf("Book borrowed successfully!\n");
                } else {
                    // Add to queue
                    address borrower = Alokasi(Name, priority);
                    InsertPriority(&Queue(*currentBook), borrower);
                    printf("Out of stock! Added to queue.\n");
                }
                waitForEnter();
                break;
            }           
            case 4: {
                printAllBookQueues(bukuArray);
                waitForEnter();
                break;
            }
            case 5: {
                char nama[LENGTH], judul[LENGTH];
                printBorrower(BorrowedList);
            
                getNonEmptyInput("Enter your name: ", nama, LENGTH);
                FormatSentence(nama);
                getNonEmptyInput("Enter book title: ", judul, LENGTH);
                FormatSentence(judul);
                
                // Record action for undo
                RecordReturnBook(&undoStack, judul, nama);
            
                RemoveBorrower(&BorrowedList, nama, judul);
                int bookIndex = SearchBook(bukuArray, judul);
                if (bookIndex != -1) {
                    Book *returnedBook = &BookAt(bukuArray, bookIndex);
                    Stock(*returnedBook)++; // Increment stock by 1
                    
                    // Check if there are people in queue who can now borrow the book
                    if (!IsQueueEmpty(Queue(*returnedBook))) {
                        printf("\nProcessing queue for book '%s'...\n", Title(*returnedBook));
                        processQueue(returnedBook, 1); // Process for the 1 returned book
                    } else {
                        printf("Book returned successfully! Stock increased.\n");
                    }
                } else {
                    printf("Book not found!\n");
                }
                waitForEnter();
                break;
            }
            case 6: {
                printBorrower(BorrowedList);
                waitForEnter();
                break;
            }
            case 7: {
                char name[LENGTH], bookTitle[LENGTH];
                
                printAllBookQueues(bukuArray);
                
                getNonEmptyInput("Enter your name: ", name, LENGTH);
                FormatSentence(name);
                getNonEmptyInput("Enter book title: ", bookTitle, LENGTH);
                FormatSentence(bookTitle);
                
                int bookIndex = SearchBook(bukuArray, bookTitle);
                if (bookIndex == -1) {
                    printf("Book not found!\n");
                    waitForEnter();
                    break;
                }
                
                Book *currentBook = &BookAt(bukuArray, bookIndex);
                
                // Find the borrower in the queue
                address prev = NULL;
                address current = Queue(*currentBook);
                boolean found = false;
                int priority = 0;
                
                while (current != NULL && !found) {
                    if (strcmp(Info(current), name) == 0) {
                        found = true;
                        priority = Int(current); // Store priority for undo
                        
                        // Record action for undo before removing
                        RecordCancelBorrowing(&undoStack, bookTitle, name, priority, bookIndex);
                        
                        if (prev == NULL) {
                            // At beginning of queue
                            DeleteFirst_Queue(&Queue(*currentBook), &current);
                        } else {
                            // In middle/end of queue
                            DeleteAfter_Queue(&Queue(*currentBook), &current, prev);
                        }
                        Dealokasi(current);
                        printf("Borrowing canceled for %s!\n", name);
                    } else {
                        prev = current;
                        current = Next(current);
                    }
                }
                
                if (!found) {
                    printf("Name not found in the queue for this book!\n");
                }
                
                waitForEnter();
                break;
            }
            case 8: { // Undo option
                UndoLastAction(&undoStack);
                waitForEnter();
                break;
            }
            case 9: { // Exit
                printf("Thank you for using the Library System!\n");
                // Clean up the undo stack before exiting
                ClearStack(&undoStack);
                return 0;
            }
            default:
                printf("Invalid option!\n");
                waitForEnter();
        }
        clearScreen();
    } while (1);
    return 0;
}

//=================For View Input================//
void waitForEnter() {
    printf("\nTekan Enter untuk melanjutkan...");
    clearBuffer();
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearScreen() {
    system("cls || clear");
}

void getNonEmptyInput(const char *prompt, char *buffer, int bufferSize) {
    int len;
    char *trimmed;
    do {
        printf("%s", prompt);
        fgets(buffer, bufferSize, stdin);
        
        // Hapus newline dan karakter berlebih
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            clearBuffer();
        }
        
        // Trim spasi di awal dan akhir
        trimmed = buffer;
        while (isspace((unsigned char)*trimmed)) trimmed++;
        char *end = trimmed + strlen(trimmed) - 1;
        while (end > trimmed && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';
        
        strcpy(buffer, trimmed);
        
        if (strlen(buffer) == 0) {
            printf("Input tidak boleh kosong!\n");
        } else {
            break;
        }
    } while (1);
}

void FormatSentence(char *sentence) {
    if (sentence == NULL) return;
    
    int i;
    for (i = 0; sentence[i]; i++) {
        if (i == 0 || sentence[i - 1] == ' ') {
            sentence[i] = toupper(sentence[i]);
        } else {
            sentence[i] = tolower(sentence[i]);
        }
    }
}
