#ifndef __HW1_H
#define __HW1_H

#define INFO(...) do {fprintf(stderr, "[          ] [ INFO ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0)
#define ERROR(...) do {fprintf(stderr, "[          ] [ ERR  ] "); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr);} while(0) 


#define MAX_LENGTH 8
#define MAX_SEQUENCE_CAP  40320

//Required:
int initialize_board(const char *initial_state, const char *keys, int size);
int solve(const char *initial_state, const char *keys, int size);

//Part 1 Game
void game();
void printBoard();
int isValidChoice(char, char[]);
int emptyCell(int, int);
int checkDuplicates(char, int, int);
int checkInitialDuplicates();
int checkFullRow(int);
int checkFullCol(int);
int checkKeys(char, int, int);
int checkInitialKeys();

//Part 2 Solver
//Setup and heuristic 1
void initializeConstraints();
void initializeClues();
int returnRow(int);
int returnCol(int);
void setValue(char, int, int);
void removeConstraint(int, int, int);
void edgeClueElimination();

//Heuristic 2
void cellProp(int, int, int);
int compareConstraints(char[MAX_LENGTH][MAX_LENGTH][MAX_LENGTH+1]);
int constraintPropagation();

//Heuristic 3
int containsNumber(const char*, int);
int processOfElimination();

//Part 3: Heuristic 4
int isValidSequence(char[MAX_LENGTH+1], int , int);
void generateRowSequences(char[MAX_LENGTH+1], int[MAX_LENGTH], int, int );
int sequenceFiltration();

/* Hints from our solution (feel free to use or ignore):

Part 1

void print_board();
int get_move();
int check_dupes_col(int col);
int check_dupes_row(int row);
int check_row(int row);
int check_col(int col);
int try_move(char choice, int row, int col);

Part 2

void edge_clue_initialization(void);
void apply_edge_constraint_rule(void);
bool apply_constraint_propagation(int row, int column, int piece);
void apply_process_of_elimination(int row, int column, int piece);

Part 3

void generate_valid_sequences(int valid_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH], bool is_horizontal, int index);
void generate_valid_sequences_helper(int valid_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH], bool is_horizontal, int index, int sequence[MAX_LENGTH], int starts_at);
void generate_filtered_sequences(int filtered_sequences[MAX_SEQUENCE_CAP][MAX_LENGTH], bool is_horizontal, int index);
bool sequence_filtration(bool is_horizontal, int index);
bool apply_sequence_filtration(void);

Testing functions

void print_possible_pieces_state(void);
void print_possible_pieces_at_cell(int row, int column);
*/


#endif // HW1_H