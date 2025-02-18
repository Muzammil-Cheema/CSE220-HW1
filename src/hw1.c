#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "hw1.h"



int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};
int boardSize;
int gameOver = 0;
int filledBoardSpaces = 0;


//Game Board
char board[MAX_LENGTH][MAX_LENGTH] = {0};
//Stores top row, then alternates between left and right keys, ends with bottom key row. 
char visibleBuildings[4*MAX_LENGTH];
//User choice string variable
char choice[5];


//Plays the Skyscrapers game
//Requirement 1D
void game(){
    //Initial check of how many board spaces are pre-filled. Used for winning condition check.
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (isdigit(board[i][j]) != 0)
                filledBoardSpaces++;
    

    //Define and initialize variables that store user input and incorrect input flags.
    char piece = '-';
    int row = -1, col = -1;
    int validPiece = 0, validRow = 0, validCol = 0;

    //Skyscrapers main game logic.
    while (gameOver == 0){
        printBoard();

        //Checks if correct piece is chosen.
        validPiece = 0;
        while(validPiece == 0) {
            printf("Choose a piece (1-%d) or q to quit: ", boardSize);
            scanf(" %s", choice);
            if (isValidChoice('p', choice) == 2){
                gameOver = 2;
                break;
            }
            if (isValidChoice('p', choice) != 0){
                printf("Invalid Choice. ");
                continue;
            }
            piece = choice[0];
            validPiece = 1;
        }
        if (gameOver == 2)
            break;
        
        
        //Checks if correct row is chosen.
        validRow = 0; 
        while (validRow == 0){
            printf("Choose a row (0-%d): ", boardSize-1);
            scanf(" %s", choice);
            if (isValidChoice('r', choice) != 1){
                printf("Invalid Choice. ");
                continue;
            }
            row = choice[0] - '0';
            validRow = 1;
        } 
        

        //Checks if correct column is chosen.
        validCol = 0;
        while(validCol == 0){
            printf("Choose a column (0-4): ");
            scanf(" %s", choice);
            if (isValidChoice('c', choice) != 1){
                printf("Invalid Choice. ");
                continue;
            }
            col = choice[0] - '0';
            validCol = 1;
        }
        

        //Checks if board space is empty.
        if (emptyCell(row, col) == 0) {
            printf("invalid choice. That space is already occupied.\n");
            continue;
        }
        //Checks if there is a dupliacte in the row or col.
        if (checkDuplicates(piece, row, col) == 0) {
            printf("Invalid choice. There is already a building with that height in that row or column.\n");
            continue;
        }

        //Checks
        if (checkKeys(piece, row, col) == 0){
            printf("Invalid choice. You violate one of the key requirements.\n");
            continue;
        } 
        

        //Checks if board is filled (if game is won).
        if (filledBoardSpaces == boardSize*boardSize){
            gameOver = 1;
            printf("Congratulations, you have filled the board!\n");
            printBoard();
        }
    }
}


//Requirement 1B
int initialize_board(const char *initial_state, const char *keys, int size) {
	if (size < 2 || size > 8)
		return 0;

	boardSize = size;
	//Initializes board values
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			board[i][j] = initial_state[i*size + j];
    
    if (checkInitialDuplicates() == 0)
        return 0;
	
	//Stores visible building values in a convenient order for printing
	int vbCount = 0;
	for (int i = 0; i < size*4; i++){
		if (i < size)
			visibleBuildings[i] = keys[i];
		if (i >= size && i < 3*size && vbCount%2 == 0){
			visibleBuildings[i] = keys[i + size - vbCount/2];
			vbCount++;
            continue;
		}
		if (i >= size && i < 3*size && vbCount%2 == 1){
			visibleBuildings[i] = keys[i + 2*size - (vbCount+1)/2];
			vbCount++;
		}
		if (i >= 3*size)
			visibleBuildings[i] = keys[i-2*size];
	}

    if (checkInitialKeys() == 0)
        return 0;
    
	return 1;
}

//Requirement 1C
void printBoard(){
    //prints top rows
	int i = 0; 
    printf("   ");
	for (; i < boardSize; i++)
		printf(" %c", visibleBuildings[i]);
	printf("\n   ");
    for (int j = 0; j < boardSize; j++)
	    printf(" v");
    printf("\n");

    //prints board/middle rows
    int row = 0;
	while(i < 3*boardSize){
        printf("%c > ", visibleBuildings[i]);
        i++;
        for (int col= 0; col < boardSize; col++)
            printf("%c ", board[row][col]);
        row++;
        printf("< %c\n", visibleBuildings[i]);
        i++;
    }

    //prints bottom rows
	printf("   ");
    for (int j = 0; j < boardSize; j++)
        printf(" ^");
    printf("\n   ");
    for (; i < 4*boardSize; i++)
        printf(" %c", visibleBuildings[i]);
    printf("\n");
}

//Requeirement 1E assitance
int isValidChoice(char question, char response[]){
    if (strcmp(response, "q") == 0)
        return 2;
    if (strlen(response) > 1)
        return 3;
    
    char res = response[0];
    res -= '0';
    switch (question) {
    case 'p':
        if (res < 1 || res > boardSize)
            return 0;
        break;
    case 'r': case 'c':
        if (res < 0 || res > (boardSize-1))
            return 0;
        break;
    default:
        printf("Unexpected behavior in choice validity checking");
        break;
    }

    return 1;
}

//Requirement 1F
int emptyCell(int row, int col){
    if (board[row][col] != '-')
        return 0; 
    return 1;
}

//Requirement 1G
int checkDuplicates(char piece, int row, int col){
    for (int i = 0; i < boardSize; i++)
        if(board[row][i] == piece)
            return 0;
    for (int i = 0; i < boardSize; i++)
        if(board[i][col] == piece)
            return 0;
    
    return 1;
}

//Requirement 1G in 1B
int checkInitialDuplicates(){
    //Define and initialize array to track duplicates in each row or column
    int singleEntry[boardSize];
    for (int i = 0; i < boardSize; i++){
        singleEntry[i] = 0;
    }

    //Check rows
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++)
            singleEntry[board[i][j]-1]++;
        for (int j = 0; j < boardSize; j++)
            if(singleEntry[j] > 1)
                return 0;
        for (int j = 0; j < boardSize; j++)
            singleEntry[j] = 0;
    }

    //Check columns
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++)
            singleEntry[board[j][i]-1]++;
        for (int j = 0; j < boardSize; j++)
            if(singleEntry[j] > 1)
                return 0;
        for (int j = 0; j < boardSize; j++)
            singleEntry[j] = 0;
    }

    return 1;
}

//Requirement 1H row completion
int checkFullRow(int row){
    int rowComplete = -boardSize + 1;
    for (int i = 0; i < boardSize; i++)
        if (board[row][i] != '-')
            rowComplete++;

    //If row is completed
    int max = 0, visibleCount = 0;
    if (rowComplete == 1){
        int leftKey = visibleBuildings[2*boardSize + row*2]; 
        int rightKey = visibleBuildings[2*boardSize + row*2 + 1];

        //Iterate from left to right
        for(int i = 0 ; i < boardSize; i++){
            if (board[row][i] > max){
                max = board[row][i];
                visibleCount++;
            }
            if (max == boardSize)
                break;
        } 
        if (visibleCount != leftKey)
            return 0;

        //Iterate from right to left
        max = 0, visibleCount = 0;
        for(int i = boardSize-1 ; i >= 0; i--){
            if (board[row][i] > max){
                max = board[row][i];
                visibleCount++;
            }
            if (max == boardSize)
                break;
        } 
        if (visibleCount != rightKey)
            return 0;
    }

    return 1;
}

//Requirement 1H column completion
int checkFullCol(int col){
    int colComplete = -boardSize + 1;
    for (int i = 0; i < boardSize; i++)
        if (board[i][col] != '-')
            colComplete++;

    //If row is completed
    int max = 0, visibleCount = 0;
    if (colComplete == 1){
        int topKey = visibleBuildings[col];
        int bottomKey = visibleBuildings[3*boardSize + col];

        //Iterate from top to bottom
        for(int i = 0 ; i < boardSize; i++){
            if (board[i][col] > max){
                max = board[i][col];
                visibleCount++;
            }
            if (max == boardSize)
                break;
        } 
        if (visibleCount != topKey)
            return 0;

        //Iterate from bottom to top
        max = 0, visibleCount = 0;
        for(int i = boardSize-1 ; i >= 0; i--){
            if (board[i][col] > max){
                max = board[i][col];
                visibleCount++;
            }
            if (max == boardSize)
                break;
        } 
        if (visibleCount != bottomKey)
            return 0;
    }

    return 1;
}

//Requirement 1H
//Track current height max from one end of a row or column until we reach the tallest possible building. Count how many buildings are visible until the tallest possible building is reached. Compare count with key. If different, then return 0. If all counts are the same as their respective keys, then update the board. 
int checkKeys(char piece, int row, int col){
    board[row][col] = piece;

    if (checkFullRow(row) == 0 || checkFullCol(col) == 0){
        board[row][col] = '-';
        return 0;
    }

    //If all checks are passed
    return 1;
}

//Requirement 1H in 1B
int checkInitialKeys(){
    for(int i = 0; i < boardSize; i++)
        if (checkFullRow(i) == 0 || checkFullCol(i) == 0);
            return 0;
    return 1;
}

int solve(const char *initial_state, const char *keys, int size){
	(void) initial_state;
	(void) keys;
	(void) size;
	
	return 0;
}





//Observations
//Range of visible buildings from both ends of one row/column is [2, n+1] where n is the board size. 
//Strings can be traversed and stored as char pointers!!
//Last argument (visible buildings) is ordered as top, bottom, left, right