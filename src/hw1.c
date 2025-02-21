// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <assert.h>
// #include <ctype.h>
// #include <string.h>
// #include "hw1.h"

// int top_key[MAX_LENGTH] = {0};
// int bottom_key[MAX_LENGTH] = {0};
// int left_key[MAX_LENGTH] = {0};
// int right_key[MAX_LENGTH] = {0};
// int boardSize;

// //Game Board
// char board[MAX_LENGTH][MAX_LENGTH] = {0};
// //Reading input
// char choice[5];

// int initialize_board(const char *initial_state, const char *keys, int size){
//     boardSize = size;

//     //Set board
//     for (int i = 0; i < boardSize; i++){
//         for (int j = 0; j < boardSize; j++){
//             board[i][j] = initial_state[i*boardSize + j];
//         }
//     }

//     //Set keys
//     for (int i = 0; i < boardSize; i++){
//         top_key[i] = keys[i] - '0';
//     }
//     for (int i = 0; i < boardSize; i++){
//         bottom_key[i] = keys[boardSize + i] - '0';
//     }
//     for (int i = 0; i < boardSize; i++){
//         left_key[i] = keys[i] - '0';
//     }
//     for (int i = 0; i < boardSize; i++){
//         right_key[i] = keys[i] - '0';
//     }

//     return 1;
// }

// void printBoard(){
//     printf("   ");
//     for (int i = 0; i < boardSize; i++){
//         printf(" %d", top_key[i]);
//     }
//     printf("\n   ");
//     for (int i = 0; i < boardSize; i++){
//         printf(" v");
//     }
//     printf("\n");


//     for (int i = 0; i < boardSize; i++){
//         printf("%d >", left_key[i]);
//         for (int j = 0; j < boardSize; j++){
//             printf(" %c", board[i][j]);
//         }
//         printf(" < %d\n", right_key[i]);
//     }

//     printf("   ");
//     for (int i = 0; i < boardSize; i++){
//         printf(" ^");
//     }
//     printf("\n   ");
//     for (int i = 0; i < boardSize; i++){
//         printf(" %d", bottom_key[i]);
//     }
//     printf("\n");

// }















































#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "hw1.h"
//gcc -g -Iinclude -o hw1_game src/hw1.c src/hw1_game.c


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
// char visibleBuildings[4*MAX_LENGTH];
//User choice string variable
char choice[5];


//Plays the Skyscrapers game
//Requirement 1D
void game(){
    //Initial check of how many board spaces are pre-filled. Used for winning condition check.
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (isdigit(board[i][j]))
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
            if (isValidChoice('p', choice) != 1){
                printf("Invalid choice. ");
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
                printf("Invalid choice. ");
                continue;
            }
            row = choice[0] - '0';
            validRow = 1;
        } 
        

        //Checks if correct column is chosen.
        validCol = 0;
        while(validCol == 0){
            printf("Choose a column (0-%d): ", boardSize-1);
            scanf(" %s", choice);
            if (isValidChoice('c', choice) != 1){
                printf("Invalid choice. ");
                continue;
            }
            col = choice[0] - '0';
            validCol = 1;
        }
        

        //Checks if board space is empty.
        if (emptyCell(row, col) == 0) {
            printf("Invalid choice. That space is already occupied.\n");
            continue;
        }

        //Checks if there is a dupliacte in the row or col.
        if (checkDuplicates(piece, row, col) == 0) {
            printf("Invalid choice. There is already a building with that height in that row or column.\n");
            continue;
        }

        //Checks key requirements
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
	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < size; j++)
			board[i][j] = initial_state[i*size + j];
    
    if (checkInitialDuplicates() == 0)
        return 0;
	

    //Set keys
    for (int i = 0; i < boardSize; i++)
        top_key[i] = keys[i] - '0';
    for (int i = 0; i < boardSize; i++)
        bottom_key[i] = keys[boardSize + i] - '0';
    for (int i = 0; i < boardSize; i++)
        left_key[i] = keys[boardSize*2 + i] - '0';
    for (int i = 0; i < boardSize; i++){
        right_key[i] = keys[boardSize*3 + i] - '0';
    }
    
	return 1;
}

//Requirement 1C
void printBoard(){
    printf("   ");
    for (int i = 0; i < boardSize; i++){
        printf(" %d", top_key[i]);
    }
    printf("\n   ");
    for (int i = 0; i < boardSize; i++){
        printf(" v");
    }
    printf("\n");


    for (int i = 0; i < boardSize; i++){
        printf("%d >", left_key[i]);
        for (int j = 0; j < boardSize; j++){
            printf(" %c", board[i][j]);
        }
        printf(" < %d\n", right_key[i]);
    }

    printf("   ");
    for (int i = 0; i < boardSize; i++){
        printf(" ^");
    }
    printf("\n   ");
    for (int i = 0; i < boardSize; i++){
        printf(" %d", bottom_key[i]);
    }
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

// //Requirement 1F
int emptyCell(int row, int col){
    if (board[row][col] != '-')
        return 0; 
    return 1;
}

// //Requirement 1G
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
            singleEntry[board[i][j]-'0'-1]++;
        for (int j = 0; j < boardSize; j++)
            if(singleEntry[j] > 1)
                return 0;
        for (int j = 0; j < boardSize; j++)
            singleEntry[j] = 0;
    }

    //Check columns
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++)
            singleEntry[board[j][i]-'0'-1]++;
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
    //Check if row is completed
    int rowComplete = -boardSize + 1;
    for (int i = 0; i < boardSize; i++)
        if (board[row][i] != '-')
            rowComplete++;

    //If row is completed
    int max = 0, visibleCount = 0;
    if (rowComplete == 1){
        int leftKey = left_key[row]; 
        int rightKey = right_key[row];

        //Iterate from left to right
        if (leftKey != 0){
            for(int i = 0 ; i < boardSize; i++){
                if (board[row][i] - '0' > max){
                    max = board[row][i] - '0';
                    visibleCount++;
                }
                if (max == boardSize)
                    break;
            } 
            if (visibleCount != leftKey)
                return 0;
        }

        //Iterate from right to left
        if(rightKey != 0){
            max = 0, visibleCount = 0;
            for(int i = boardSize-1 ; i >= 0; i--){
                if (board[row][i] - '0' > max){
                    max = board[row][i] - '0';
                    visibleCount++;
                }
                if (max == boardSize)
                    break;
            } 
            if (visibleCount != rightKey){
                return 0;
            }
        }
    }

    return 1;
}

//Requirement 1H column completion
int checkFullCol(int col){
    int colComplete = -boardSize + 1;
    for (int i = 0; i < boardSize; i++)
        if (board[i][col] != '-')
            colComplete++;

    //If column is completed
    int max = 0, visibleCount = 0;
    if (colComplete == 1){
        int topKey = top_key[col];
        int bottomKey = bottom_key[col];

        //Iterate from top to bottom
        if (topKey != 0){
            for(int i = 0 ; i < boardSize; i++){
                if (board[i][col] - '0' > max){
                    max = board[i][col] - '0';
                    visibleCount++;
                }
                if (max == boardSize)
                    break;
            } 
            if (visibleCount != topKey)
                return 0;
        }

        //Iterate from bottom to top
        if (bottomKey != 0){
            max = 0, visibleCount = 0;
            for(int i = boardSize-1 ; i >= 0; i--){
                if (board[i][col] - '0' > max){
                    max = board[i][col] - '0';
                    visibleCount++;
                }
                if (max == boardSize)
                    break;
            } 
            if (visibleCount != bottomKey)
                return 0;
        }
    }

    return 1;
}

// //Requirement 1H
// //Track current height max from one end of a row or column until we reach the tallest possible building. Count how many buildings are visible until the tallest possible building is reached. Compare count with key. If different, then return 0. If all counts are the same as their respective keys, then update the board. 
int checkKeys(char piece, int row, int col){
    board[row][col] = piece;
    if (checkFullRow(row) == 0 || checkFullCol(col) == 0){
        board[row][col] = '-';
        return 0;
    }

    //If all checks are passed
    filledBoardSpaces++;
    return 1;
}

// //Requirement 1H in 1B
int checkInitialKeys(){
    for(int i = 0; i < boardSize; i++)
        if (checkFullRow(i) == 0 || checkFullCol(i) == 0)
            return 0;
    return 1;
}

int solve(const char *initial_state, const char *keys, int size){
	(void) initial_state;
	(void) keys;
	(void) size;
	
	return 0;
}





// //Observations
// //Range of visible buildings from both ends of one row/column is [2, n+1] where n is the board size. 
// //Strings can be traversed and stored as char pointers!!
// //Last argument (visible buildings) is ordered as top, bottom, left, right