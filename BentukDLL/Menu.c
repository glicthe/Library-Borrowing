/* 
    File name    : Menu.c
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : for the Menu
*/

#include "Menu.h"
#define LENGTH 20

//===============For the Main Menu===============//
int MainMenu() {
	char title[LENGTH];
	int pilihan, i, NumBook;
	CreateList(&bukuList);
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
                
                address buku = Alokasi(title, stock);
            	InsertLast(&bukuList, buku);
    			
    			// Record action for undo
    			RecordAddBook(&undoStack, title, stock);
    			
                waitForEnter();
    			break;
            }
			case 2: {
			    int NewStock;
			    PrintInfo(bukuList);
			
			    getNonEmptyInput("What book's title you want to change?\n>>> ", title, LENGTH);
			    FormatSentence(title);
			
			    address AddStock = Search(bukuList, title);
			    if (AddStock == NULL) {
			        printf("Book not found!\n");
			        waitForEnter();
			        break;
			    }
			    
			    printf("Current Stock: [%d]\n", Int(AddStock));
			    printf("Add Stock: ");
			    scanf("%d", &NewStock);
			    clearBuffer();
			    
			    // Record action for undo before changing stock
			    RecordAddStock(&undoStack, title, NewStock, -1);  // -1 as we don't use bookIndex anymore
			    
			    // Add new stock to book
			    Int(AddStock) += NewStock;
			    printf("\nUpdated Stock: [%d]\n", Int(AddStock));
			    
			    // Check if there are people in queue who can now borrow the book
			    List* bookQueue = GetQueueFromBook(AddStock);
			    if (bookQueue != NULL && !IsEmpty(*bookQueue)) {
			        printf("\nProcessing queue for book '%s'...\n", title);
			        processQueue(bookQueue, AddStock, NewStock);
			    }
			    
			    waitForEnter();
			    break;
			}
			case 3: {
            	char Name[LENGTH];
            	int priority = 0;
            	
            	// Showing all of the book
				PrintInfo(bukuList);
				
				// Choose the book
            	printf("\nwhat book you want to borrow?\n");
            	getNonEmptyInput("Enter the title: ", title, LENGTH);
            	FormatSentence(title);
				                
                address TheBook = FindBookByTitle(bukuList, title);
                if (TheBook == NULL) {
     			   	printf("Book not found!\n");
        			waitForEnter();
			        break;
			    }
                
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
        				continue;
				    } else break;
				}
				
				// Record action for undo
                RecordBorrowBook(&undoStack, title, Name, priority, -1);  // -1 as we don't use bookIndex anymore
                
                // Get or create the queue for this book
                List* bookQueue = GetQueueFromBook(TheBook);
                if (bookQueue == NULL) {
                    printf("Error creating queue for book.\n");
                    waitForEnter();
                    break;
                }
            
				if (Int(TheBook) > 0 && IsEmpty(*bookQueue)) {
     			   // Immediate borrowing if queue is empty
        			Int(TheBook)--;
        			address NewBorrowed = AlokasiBorrowed(Name, title);
        			InsertFirst(&BorrowedList, NewBorrowed);
        			printf("Book borrowed successfully!\n");
    			} else {
		        	// Add to queue
        			address borrower = Alokasi(Name, priority);
        			InsertPriority(bookQueue, borrower);
        			printf("Out of stock! Added to queue.\n");
			    }
    			waitForEnter();
    			break;
			}			
            case 4: {
                printBookQueues(bukuList);
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
			    address returnedBook = FindBookByTitle(bukuList, judul);
			    if (returnedBook != NULL) {
			        Int(returnedBook)++; // Increment stock by 1
			        
			        // Check if there are people in queue who can now borrow the book
			        List* bookQueue = GetQueueFromBook(returnedBook);
			        if (bookQueue != NULL && !IsEmpty(*bookQueue)) {
			            printf("\nProcessing queue for book '%s'...\n", judul);
			            processQueue(bookQueue, returnedBook, 1); // Process for the 1 returned book
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
				
				printBookQueues(bukuList);
				
				getNonEmptyInput("Enter your name: ", name, LENGTH);
				FormatSentence(name);
				getNonEmptyInput("Enter book title: ", bookTitle, LENGTH);
				FormatSentence(bookTitle);
				
				address book = FindBookByTitle(bukuList, bookTitle);
				if (book == NULL) {
					printf("Book not found!\n");
					waitForEnter();
					break;
				}
				
				// Get the queue for this book
				List* bookQueue = GetQueueFromBook(book);
				if (bookQueue == NULL || IsEmpty(*bookQueue)) {
					printf("No queue for this book or queue is empty!\n");
					waitForEnter();
					break;
				}
				
				// Find the borrower in the queue
				address prev = NULL;
				address current = First(*bookQueue);
				boolean found = false;
				int priority = 0;
				
				while (current != NULL && !found) {
					if (strcmp(Info(current), name) == 0) {
						found = true;
						priority = Int(current); // Store priority for undo
						
						// Record action for undo before removing
                        RecordCancelBorrowing(&undoStack, bookTitle, name, priority, -1);
						
						if (prev == NULL) {
							// At beginning of queue
							DeleteFirst(bookQueue, &current);
						} else {
							// In middle/end of queue
							DeleteAfter(bookQueue, &current, prev);
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
			case 8: { // New Undo option
				UndoLastAction(&undoStack);
				waitForEnter();
				break;
			}
			case 9: { // Exit moved to option 9
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

void processQueue(List *queueList, address book, int addedStock) {
    if (addedStock <= 0 || IsEmpty(*queueList)) {
        return; // Exit if no stock is added or queue is empty
    }

    // Keep track of how many books we've processed
    int stockProcessed = 0;
    while (stockProcessed < addedStock && !IsEmpty(*queueList)) {
        // Get the first person in queue
        address borrower;
        DeleteFirst(queueList, &borrower);
        
        if (borrower != NULL) {
            // Add to borrowed list (using the actual borrower name and book title)
            address newBorrowed = AlokasiBorrowed(Info(borrower), Info(book));
            InsertFirst(&BorrowedList, newBorrowed);
            
            // Print message
            printf("Book '%s' has been given to '%s'!\n", Info(book), Info(borrower));
            
            // Decrement stock
            Int(book)--;
            stockProcessed++;
            
            // Free the queue node
            Dealokasi(borrower);
        }
    }
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
