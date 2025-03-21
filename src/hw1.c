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
//User choice string variable
char choice[5];



//Allows the user to play the Skyscrapers game
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
    
    if (checkInitialKeys() == 0){
        return 0;
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
        //Counts occurences of each digit in row
        for (int j = 0; j < boardSize; j++)
            if (isdigit(board[i][j]))
                singleEntry[board[i][j]-'1']++;
        //Returns 0 if any count is greater than 1
        for (int j = 0; j < boardSize; j++)
            if(singleEntry[j] > 1)
                return 0;
        //Resets counts to 0 for next iteration
        for (int j = 0; j < boardSize; j++)
            singleEntry[j] = 0;
    }

    //Check columns
    for (int i = 0; i < boardSize; i++){
        //Counts occurences of each digit in column
        for (int j = 0; j < boardSize; j++)
            if (isdigit(board[j][i]))
                singleEntry[board[j][i]-'1']++;
        //Returns 0 if any count is greater than 1
        for (int j = 0; j < boardSize; j++)
            if(singleEntry[j] > 1)
                return 0;
        //Resets counts to 0 for next iteration
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

//Requirement 1H
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

//Requirement 1H in 1B
int checkInitialKeys(){
    for(int i = 0; i < boardSize; i++)
        if (checkFullRow(i) == 0 || checkFullCol(i) == 0)
            return 0;
    return 1;
}










//Part 2: Solution Heuristics
//Heuristic 1

//3D char array holding possible values for each board cell as a string
char constraints[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH + 1] = {0};
//Holds all clues on the board in top, bottom, left, right order. 
int boardClues[4*MAX_LENGTH];

//Prints constraints 2D array. Only used for debugging and visualization by programmer. 
void printConstraints(){
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            printf("%s\t", constraints[i][j]);
        }
        printf("\n");
    }
}

//Prints constraints for a single cell. Only used for debugging and visualization by programmer.
void printCellConstraints(int row, int col){
    printf("Constraints for [%d][%d]: %s\n", row, col, constraints[row][col]);
}



int solve(const char *initial_state, const char *keys, int size){
    initialize_board(initial_state, keys, size);
    initializeClues();
    initializeConstraints();
    printBoard();
    printConstraints();

    edgeClueElimination();
    constraintPropagation();

    while(processOfElimination() || sequenceFiltration()){}

    printBoard();
    printConstraints();
	return 0;
}



//Stores an array of possible values for each cell in board. This is stored in the 3D char array "constraints." 
void initializeConstraints(){
    for (int i = 0; i < boardSize; i++){
        for (int j = 0; j < boardSize; j++){
            if (isdigit(board[i][j]))
                setValue(board[i][j], i, j);
            else {
                for (int k = 0; k < boardSize; k++)
                    constraints[i][j][k] = '1' + k;
                constraints[i][j][boardSize] = '\0';
            }
        }
    }
}

//Stores keys in a single array, boardClues, in the order top from left to right, bottom from left to right, left from top to bottom, and right from top to bottom. 
void initializeClues(){
    int i = 0;
    for (; i < boardSize; i++)
        boardClues[i] = top_key[i];
    for (; i < 2*boardSize; i++)
        boardClues[i] = bottom_key[i-boardSize];
    for (; i < 3*boardSize; i++)
        boardClues[i] = left_key[i-2*boardSize];
    for (; i < 4*boardSize; i++)
        boardClues[i] = right_key[i-3*boardSize];
}

//Return the row that the given clue corresponds to. The clue is passed by its index in boardClues[]. 
int returnRow(int clueIndex){
    if (2*boardSize <= clueIndex && clueIndex < 3*boardSize)
        return clueIndex - 2*boardSize;
    if (3*boardSize <= clueIndex && clueIndex < 4*boardSize)
        return clueIndex - 3*boardSize;
    return -1;
}

//Return the col that the given clue corresponds to. The clue is passed by its index in boardClues[]. 
int returnCol(int clueIndex){
    if (0 <= clueIndex && clueIndex < boardSize)
        return clueIndex;
    if (boardSize <= clueIndex && clueIndex < 2*boardSize)
        return clueIndex - boardSize;
    return -1;
}

//Adds a board value and removes constraints from the corresponding cell by setting constraint to the determined board value. 
void setValue(char val, int row, int col){
    board[row][col] = val;
    constraints[row][col][0] = val;
    constraints[row][col][1] = '\0'; 
    // printf("Value ");
    // printCellConstraints(row, col);
}

//Removes the given character from the constraints list at a given cell. If one constraint is left, then the board value is set using setValue. If all constraints are removed, the constraints string is set to "X". 
void removeConstraint(int val, int row, int col){
    char* s = constraints[row][col];
    val += '0';
    int i = 0;

    //Searches for removal character
    while(s[i]){
        //When removal character found, shift all elements left in char array
        if (s[i] == val){
            while(s[i+1]){
                s[i] = s[i+1];
                i++;
            }
            s[i] = '\0';
            break;
        }
        i++;
    }

    
    //If there are no possible values left for the cell, constraints for that cell are set to "X".  
    if (strlen(constraints[row][col]) == 0){
        s[0] = 'X';
        s[1] = '\0';
    }

    // printf("Remove ");
    // printCellConstraints(row, col);
}

//Apply edge clue elimnation heuristic with each given clue
void edgeClueElimination(){
    int row, col, clue, i = 0;

    //Top column clues
    for (; i < boardSize; i++){
        clue = boardClues[i];
        col = returnCol(i);

        //Skip if clue is 0
        if (clue == 0)
            continue;
        //Fill column if clue means all buildings are visible
        else if (clue == boardSize)
            for (int j = 0; j < boardSize; j++)
                setValue('1' + j, j ,col);
        //Fill adjacent cell with largest building if clue is 1
        else if (clue == 1)
            setValue('0' + boardSize, 0, col);
        //For all other clue values (1 < clue < boardSize), remove possible constraints based on min of N-c+d+2
        else {
            int min;
            for (int j = 0; j < boardSize; j++){
                min = boardSize - clue + j + 2;
                for (int k = min; k <= boardSize; k++)
                    removeConstraint(k, j, col);
            }
        }
    }

    //Bottom column clues
    for (; i < 2*boardSize; i++){
        clue = boardClues[i];
        col = returnCol(i);

        //Skip if clue is 0
        if (clue == 0)
            continue;
        //Fill column if clue means all buildings are visible
        else if (clue == boardSize)
            for (int j = boardSize-1; j >= 0; j--)
                setValue('0'+boardSize-j, j ,col);
        //Fill adjacent cell with largest building if clue is 1
        else if (clue == 1)
            setValue('0'+boardSize, boardSize-1, col);
        //For all other clue values (1 < clue < boardSize), remove possible constraints based on min of N-c+d+2
        else {
            int min;
            for (int j = boardSize-1; j >= 0; j--){
                min = boardSize - clue + (boardSize - 1 - j) + 2;
                for (int k = min; k <= boardSize; k++)
                    removeConstraint(k, j, col);
            }
        }
    }

    //Left row clues
    for (; i < 3*boardSize; i++){
        clue = boardClues[i];
        row = returnRow(i);

        //Skip if clue is 0
        if (clue == 0)
            continue;
        //Fill row if clue means all buildings are visible
        else if (clue == boardSize)
            for (int j = 0; j < boardSize; j++)
                setValue('1'+j, row, j);
        //Fill adjacent cell with largest building if clue is 1
        else if (clue == 1)
            setValue('0'+boardSize, row, 0);
        //For all other clue values (1 < clue < boardSize), remove possible constraints based on min of N-c+d+2
        else {
            int min;
            for (int j = 0; j < boardSize; j++){
                min = boardSize - clue + j + 2;
                for (int k = min; k <= boardSize; k++)
                    removeConstraint(k, row, j);
            }
        }
    }

    //Right row clues
    for (; i < 4*boardSize; i++){
        clue = boardClues[i];
        row = returnRow(i);

        //Skip if clue is 0
        if (clue == 0)
            continue;
        //Fill row if clue means all buildings are visible
        else if (clue == boardSize)
            for (int j = boardSize-1; j >= 0; j--)
                setValue('0'+boardSize-j, row, j);
        //Fill adjacent cell with largest building if clue is 1
        else if (clue == 1)
            setValue('0'+boardSize, row, boardSize-1);
        //For all other clue values (1 < clue < boardSize), remove possible constraints based on min of N-c+d+2
        else {
            int min;
            for (int j = boardSize-1; j >= 0; j--){
                min = boardSize - clue + (boardSize - 1 - j) + 2;
                for (int k = min; k <= boardSize; k++)
                    removeConstraint(k, row, j);
            }
        }
    } 
}



//Heuristic 2
//When a board value is determined, that value is removed from the possibilities of all other cells that share a row or column with the solved cell. 
void cellProp(int val, int row, int col){
    //Iterate over a given row
    for (int i = 0; i < boardSize; i++){
        if (!isdigit(board[row][i]))
            removeConstraint(val, row, i);
        if (strlen(constraints[row][i]) == 1)
            setValue(constraints[row][i][0], row, i);
    }
    for (int i = 0; i < boardSize; i++){
        if (!isdigit(board[i][col]))
            removeConstraint(val, i, col);
        if (strlen(constraints[i][col]) == 1)
            setValue(constraints[i][col][0], i, col);
    }
}

//Used to compare the constraints 3D array before and after constraint propagation. Returns 1 if not the same or 0 if there were no changes. 
int compareConstraints(char array[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1]){
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (strcmp(constraints[i][j], array[i][j]) != 0)
                return 1;
    return 0;
}

//Repeatedly performs constraint propagation heuristic until no new values are determined.  
int constraintPropagation(){
    //Holds a copy of constraints before each propagation iteration
    char prePropConstraints[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1] = {0};
    //Flag to check if any changes were made between iterations
    int fullyPropagated = 1;
    //Count of how many iterations were done. This is used to go back and forth between heuristics 2 and 3. 
    int iterCount = -1;

    do {
        //Creates a copy of the constraints before each propagation takes places.
        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                strncpy(prePropConstraints[i][j], constraints[i][j], MAX_LENGTH);
        
        //Propagates for each solved cell. 
        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                if (isdigit(board[i][j]))
                    cellProp(board[i][j]-'0', i, j);
    
        //Ends propagation loop when no new constraints are removed. 
        fullyPropagated = compareConstraints(prePropConstraints);
        iterCount++;
    } while (fullyPropagated);
    return iterCount;
}



//Heuristic 3
int containsNumber(const char *string, int sub){
    sub += '0';
    for (int i = 0; string[i] != '\0'; i++){
        if (string[i] == sub)
            return 1;
    }

    return 0;
}

//Returns 0 if no changes were made. 
int processOfElimination(){
    int valCount[MAX_LENGTH] = {0};
    int changeFlag = 0;
    int returnFlag = 0;

    do {
        changeFlag = 0;

        //Checks rows for cases where a possible value is present in only one constraint list. In this case, the corresponding cell must be set to that value. 
        for (int i = 0; i < boardSize; i++){
            for (int j = 0; j < boardSize; j++)
                if (!isdigit(board[i][j]))
                    for (int k = 0; constraints[i][j][k] != '\0'; k++)
                        valCount[constraints[i][j][k]-'1']++;

            //If there are any possible values that only appear once, the cell with the unique possible value is found and is set to that value. 
            // printf("ValCount: ");
            // for (int k = 0; k < boardSize; k++){
            //     printf("%d ", valCount[k]);
            // }
            // printf("\n");
            for (int k = 0; k < boardSize; k++){
                if (valCount[k] == 1){
                    changeFlag = 1;
                    // printBoard();
                    // printConstraints();
                    for (int j = 0; j < boardSize; j++){
                        if (containsNumber(constraints[i][j], k+1)){
                            setValue(k+'1', i, j);
                            break;
                        }
                    }
                }
            }

            //Resets valCount to 0s
            for (int k = 0; k < boardSize; k++)
                valCount[k] = 0;
        }

        // printf("ROW POE\n");
        // printBoard();
        // printConstraints();

        if (constraintPropagation() > 0)
            changeFlag = 1;
        
        // printf("PROP POE\n");
        // printBoard();
        // printConstraints();


        //Checks columns for cases where a possible value is present in only one constraint list. In this case, the corresponding cell must be set to that value. 
        for (int i = 0; i < boardSize; i++){
            for (int j = 0; j < boardSize; j++)
                if (!isdigit(board[j][i]))
                    for (int k = 0; constraints[j][i][k] != '\0'; k++)
                        valCount[constraints[j][i][k]-'1']++;

            //If there are any possible values that only appear once, the cell with the unique possible value is found and is set to that value. 
            // printf("ValCount: ");
            // for (int k = 0; k < boardSize; k++){
            //     printf("%d ", valCount[k]);
            // }
            // printf("\n");
            for (int k = 0; k < boardSize; k++){
                if (valCount[k] == 1){
                    changeFlag = 1;
                    // printBoard();
                    // printConstraints();
                    for (int j = 0; j < boardSize; j++){
                        if (containsNumber(constraints[j][i], k+1)){
                            setValue(k+'1', j, i);
                            break;
                        }
                    }
                }
            }

            //Reset valCount to 0s
            for (int k = 0; k < boardSize; k++)
                valCount[k] = 0;
        }

        // printf("COL POE\n");
        // printBoard();
        // printConstraints();


        if (constraintPropagation() > 0)
            changeFlag = 1;
        
        // printf("PROP POE\n");
        // printBoard();
        // printConstraints();

        if (changeFlag == 1)
            returnFlag++;
    } while (changeFlag);

    return returnFlag;
}










//Part 3
//Heuristic 4: Clue Elimination/Sequence Filtration
//Stores all the valid sequences for a single row or column at a time. Validity is determined by isValidSequence and changes to the array are made by generateRowSequences and the main sequenceFiltration function. 
char validSequences[MAX_SEQUENCE_CAP][MAX_LENGTH+1] = {0};
int validSequencesSize = 0;

int leftClueIndex(int row){
    return 2*boardSize + row; 
}

int rightClueIndex(int row){
    return 3*boardSize + row; 
}

int topClueIndex(int col){
    return col;
}

int bottomClueIndex(int col){
    return boardSize + col;
}


//Checks if a sequence is valid using both of its corresponding clues. Accounts for cases where either clue is 0, although, that check should happen before sequences are generated in the case where both clues are 0. Works for both rows and columns. clue1 should be the clue that appears earlier in the boardClues array (topClue, bottomClue and leftClue, rightClue are the only valid orders).  
int isValidSequence(char sequence[MAX_LENGTH+1], int clue1, int clue2){
    int max = 0, visibleCount = 0; 
    if (clue1 != 0){
        for (int i = 0; sequence[i] != '\0' && max != boardSize; i++){
            if (sequence[i] > max){
                max = sequence[i];
                visibleCount++;
            }
        }
        if (visibleCount != clue1)
            return 0;    
    }
    
    max = 0, visibleCount = 0;
    if (clue2 != 0){
        for (int i = boardSize; i >= 0 && max != boardSize; i--){
            if (sequence[i] > max){
                max = sequence[i];
                visibleCount++;
            }
        }
        if (visibleCount != clue2)
            return 0;
    }
    
    return 1;
}

//Generates all possible sequences for a row. Uses isValidSequence() to check if a sequence is valid according to its two clues and adds the sequence to the validSequences 2D array if true. validSequencesSize must also be incremented as it keeps track of how many elements are in the validSequences array for iteration and assists in optimization. The buildingUsage array tracks if a digit has already been used in the current sequence and will orevent generating sequences with repeated digits.  
void generateRowSequences(char currentSequence[MAX_LENGTH+1], int buildingUsage[MAX_LENGTH], int row, int sequenceCol){
    if (sequenceCol == boardSize){
        currentSequence[boardSize] = '\0';
        if (isValidSequence(currentSequence, boardClues[leftClueIndex(row)], boardClues[rightClueIndex(row)]) == 1){
            strncpy(validSequences[validSequencesSize], currentSequence, MAX_LENGTH+1);
            validSequencesSize++;
        }
        return;
    }

    for (int k = 0; constraints[row][sequenceCol][k] != '\0'; k++){
        if (buildingUsage[constraints[row][sequenceCol][k] - '1'] != 0)
            continue;
        currentSequence[sequenceCol] = constraints[row][sequenceCol][k];
        buildingUsage[constraints[row][sequenceCol][k] - '1']++;
        generateRowSequences(currentSequence, buildingUsage, row, sequenceCol+1);
        buildingUsage[constraints[row][sequenceCol][k] - '1']--;
    }
}

//Column equivalent of generateRowSequences
void generateColSequences(char currentSequence[MAX_LENGTH+1], int buildingUsage[MAX_LENGTH], int col, int sequenceRow){
    if (sequenceRow == boardSize){
        currentSequence[boardSize] = '\0';
        if (isValidSequence(currentSequence, boardClues[topClueIndex(col)], boardClues[bottomClueIndex(col)]) == 1){
            strncpy(validSequences[validSequencesSize], currentSequence, MAX_LENGTH+1);
            validSequencesSize++;
        }
        return;
    }

    for (int k = 0; constraints[sequenceRow][col][k] != '\0'; k++){
        if (buildingUsage[constraints[sequenceRow][col][k] - '1'] != 0)
            continue;
        currentSequence[sequenceRow] = constraints[sequenceRow][col][k];
        buildingUsage[constraints[sequenceRow][col][k] - '1']++;
        generateColSequences(currentSequence, buildingUsage, col, sequenceRow+1);
        buildingUsage[constraints[sequenceRow][col][k] - '1']--;
    }
}

//Performs repeated Sequence Filtration until no new improvements could be discovered. 
int sequenceFiltration(){
    int iterCount = -1;
    //Holds a copy of constraints before each filtration iteration
    char preFilterConstraints[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1] = {0};

    do {
        //Creates a copy of the constraints before each filtration takes places.
        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                strncpy(preFilterConstraints[i][j], constraints[i][j], MAX_LENGTH);
        
        //Iterate through rows
        for (int i = 0; i < boardSize; i++){
            char currentSequence[MAX_LENGTH+1] = {0};
            int buildingUsage[MAX_LENGTH] = {0};
            //Generate sequences for the given row. 
            generateRowSequences(currentSequence, buildingUsage, i, 0);

            //Tracks how many times each digit has appeared in a given column within the valid sequences.
            int elementsCount[MAX_LENGTH] = {0}; 
            for (int k = 0; k < boardSize; k++){   //For each index/col in a sequence
                for (int j = 0; j < validSequencesSize; j++)       //For each sequence
                    elementsCount[validSequences[j][k]-'1']++;      //Increment elementCount 
                for (int j = 0; j < boardSize; j++){
                    //If a digit doesnt appear in any possible sequence for the ith row in the kth column, we can remove that constraint from the [i][k] cell. 
                    if (elementsCount[j] == 0)
                        removeConstraint(j+1, i, k);
                }
                //Resets elementCount for next column of the sequences
                for (int j = 0; j < boardSize; j++)
                    elementsCount[j] = 0;
            }

            //Should print the valid sequences for every row. 
            // printf("-------------------------\nROW %d SEQUENCES\n", i);
            for (int j = 0; j < validSequencesSize; j++){
                // printf("%s\n", validSequences[j]);
                validSequences[j][0] = '\0';
            }
            validSequencesSize = 0;
        }



        //Iterate through cols
        for (int i = 0; i < boardSize; i++){
            char currentSequence[MAX_LENGTH+1] = {0};
            int buildingUsage[MAX_LENGTH] = {0};
            //Generate sequences for the given col. 
            generateColSequences(currentSequence, buildingUsage, i, 0);

            //Tracks how many times each digit has appeared in a given row within the valid sequences.
            int elementsCount[MAX_LENGTH] = {0}; 
            for (int k = 0; k < boardSize; k++){   //For each index/row in a sequence
                for (int j = 0; j < validSequencesSize; j++)       //For each sequence
                    elementsCount[validSequences[j][k]-'1']++;      //Increment elementCount 
                for (int j = 0; j < boardSize; j++){
                    //If a digit doesnt appear in any possible sequence for the ith row in the kth column, we can remove that constraint from the [i][k] cell. 
                    if (elementsCount[j] == 0)
                        removeConstraint(j+1, k, i);
                }
                //Resets elementCount for next column of the sequences
                for (int j = 0; j < boardSize; j++)
                    elementsCount[j] = 0;
            }

            //Should print the valid sequences for every row. 
            // printf("-------------------------COL %d SEQUENCES\n", i);
            for (int j = 0; j < validSequencesSize; j++){
                // printf("%s\n", validSequences[j]);
                validSequences[j][0] = '\0';
            }
            validSequencesSize = 0;
        }

        iterCount++;
    } while(compareConstraints(preFilterConstraints));
    
    //Once all constraints are organized, perform one sweep over the constraints to see if any cells have been resolved. 
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (board[i][j] == '-' && strlen(constraints[i][j]) == 1)
                setValue(constraints[i][j][0], i, j);

    return iterCount;
}
