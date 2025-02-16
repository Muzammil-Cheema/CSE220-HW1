#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "hw1.h"

int top_key[MAX_LENGTH] = {0};
int bottom_key[MAX_LENGTH] = {0};
int left_key[MAX_LENGTH] = {0};
int right_key[MAX_LENGTH] = {0};
int boardSize;


char board[MAX_LENGTH][MAX_LENGTH] = {0};
char visibleBuildings[4*MAX_LENGTH];


int initialize_board(const char *initial_state, const char *keys, int size) {
	if (size < 2 || size > 8)
		return 1;

	boardSize = size;
	//Initializes board values
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			board[i][j] = initial_state[i*size + j];
	
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

	//i = size		vb == 0
	// [i] = [i + size - vb/2],
	// i = size+1	vb == 1
	// 			, [i] = [i + 2*size - ((vb+1)/2)] 
	//i = size+2	vb == 2
	//[i] = [i+size-vb/2]
	// i = size+3	vb == 3
	// 			, [i] = [i + 2*size - ((vb+1)/2)]


	return 0;
}


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















// char choice[5];
// int gameOver = 0, filledBoardSpaces = 0, stringLen = 0;
// void game(){
//     //Initial check of how many baord spaces are pre-filled. Used for winning condition check.
//     for (int i = 0; i < 5; i++)
//         for (int j = 0; j < 5; j++)
//             if (isdigit(board[i][j]) != 0)
//                 filledBoardSpaces++;

//     //Initialize variables that store user input and incorrect input flags
//     char piece = '-';
//     int row = -1, col = -1;
//     int invalidPiece = 0, invalidRow = 0, invalidCol = 0;

//     //Skyscrapers game logic
//     while (gameOver == 0){
//         printBoard();

//         //Checks if correct piece is chosen
//         invalidPiece = 0;
//         while(invalidPiece == 0) {
//             printf("Choose a piece (1-5) or q to quit: ");
//             scanf(" %s", choice);
//             if (isValidChoice('p', choice) == 2){
//                 gameOver = 2;
//                 break;
//             }
//             if (isValidChoice('p', choice) != 0){
//                 printf("Invalid Choice. ");
//                 continue;
//             }
//             piece = choice[0];
//             invalidPiece = 1;
//         }
//         if (gameOver == 2)
//             break;
        
        
//         //Checks if correct row is chosen
//         invalidRow = 0; 
//         while (invalidRow == 0){
//             printf("Choose a row (0-4): ");
//             scanf(" %s", choice);
//             if (isValidChoice('r', choice) != 0){
//                 printf("Invalid Choice. ");
//                 continue;
//             }
//             row = choice[0] - '0';
//             invalidRow = 1;
//         } 
        

//         //Checks if correct column is chosen
//         invalidCol = 0;
//         while(invalidCol == 0){
//             printf("Choose a column (0-4): ");
//             scanf(" %s", choice);
//             if (isValidChoice('c', choice) != 0){
//                 printf("Invalid Choice. ");
//                 continue;
//             }
//             col = choice[0] - '0';
//             invalidCol = 1;
//         }
        

//         //Checks if board space is empty
//         if (board[row][col] != '-')
//             printf("Invalid choice. That space is already occupied.\n");
//         else {
//             board[row][col] = piece;
//             filledBoardSpaces++;
//         }

//         if (filledBoardSpaces == MAX_LENGTH*MAX_LENGTH){
//             gameOver = 1;
//             printf("Congratulations, you have filled the board!\n");
//             printBoard();
//         }
//     }
// }


// int isValidChoice(char question, char response[]){
//     if (strcmp(response, "q") == 0)
//         return 2;
//     if (strlen(response) > 1)
//         return 3;
    
//     char res = response[0];
//     res -= '0';
//     switch (question)
//     {
//     case 'p':
//         if (res < 1 || res > 5)
//             return 1;
//         break;
//     case 'r': case 'c':
//         if (res < 0 || res > 4)
//             return 1;
//         break;
//     default:
//         printf("Unexpected behavior in choice validity checking");
//         break;
//     }

//     return 0;
// }