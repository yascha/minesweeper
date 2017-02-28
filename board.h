#include <stdint.h>
#include <stdbool.h>

#define NUM_ROWS (20)
#define NUM_COLS (20)
#define MINE (255)
#define DISCOVERED_OFFSET (1000)
#define FLAG_OFFSET (450)

#define NUM_MINES (25)

typedef struct _board
{
        int board[NUM_ROWS*NUM_COLS];
	int mine_indices[NUM_MINES];
	int flags_remaining;
} Board;


Board* init_board();
void populate_board(Board* b, uint16_t num_mines);
bool is_mine(Board* b, uint16_t index);
void add_mine(Board* b, uint16_t index);


void print_board(Board* b, bool show_all);
void free_board(Board* b);
void print_piece(int piece, bool show_all);


void add_clue(Board* b, uint16_t index);
void add_clues_around(Board* b, uint16_t row, uint16_t col);


bool is_flag(Board* b, uint16_t row, uint16_t col);
void flag_location(Board* b, uint16_t row, uint16_t col);

