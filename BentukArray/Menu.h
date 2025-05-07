/* 
    File name    : Menu.h
    Made by      : Fauzan uniar Mulyana
    Date         : 26 April 2025
    Description  : Header file for Menu
*/

#ifndef MENU_H
#define MENU_H

#include "LinkedList.h"
#include "LLBukuOrang.h"
#include "stack.h"  // Include the stack header

extern BookArray bukuArray;
extern List BorrowedList;
extern ActionStack undoStack; // Stack for undo operations

//===============For the Main Menu===============//

// function: for the main view
int MainMenu();

//=================For View Input================//

// function: clearing the input 
void clearBuffer();

// function: Press Enter for continuing the program
void waitForEnter();

// function: Clearing the screen
void clearScreen();

// fucntion: Make sure if the input is not blank
void getNonEmptyInput(const char *prompt, char *buffer, int bufferSize);

// function: make a format sentence
void FormatSentence(char *sentence);
#endif
