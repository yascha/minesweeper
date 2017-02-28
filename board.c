

#include "board.h"
#include <stdlib.h> // for malloc
#include <string.h> // for memset
#include <stdio.h> // for printf
#include <time.h> // for seeding our random mine population

Board* init_board()
{
	Board* b = malloc(sizeof(Board));
	memset((void*)b, 0, NUM_ROWS*NUM_COLS);
	populate_board(b, NUM_MINES);
	return b;
}


void populate_board(Board* b, uint16_t num_mines)
{
	int mine;
	int i = 0;

	b->flags_remaining = NUM_MINES;

	srand(time(NULL));

	while (i < num_mines)
	{
		mine = rand() % (NUM_ROWS*NUM_COLS);
		if (!is_mine(b, mine))
		{
			add_mine(b, mine);
			b->mine_indices[i] = mine;
			i++;
		}
	}	

}

bool is_mine(Board* b, uint16_t index)
{
	if (b->board[index] == MINE)
	{
		return true;
	}
	return false;
}

void add_mine(Board* b, uint16_t index)
{
	uint16_t row = index / NUM_COLS;
	uint16_t col = index % NUM_COLS; 

        if (row > NUM_ROWS)
        {
                printf("Error: Trying to add mine to an invalid row (%i)\n", row);
        }
        if (col > NUM_COLS)
        {
                printf("Error: Trying to add mine to an invalid col (%i)\n", col);
        }

#ifdef DEBUG
	printf("Adding mine to %d %d (index %d)\n", row, col, index);
#endif

	b->board[index] = MINE;
	add_clues_around(b, row, col);
}

void add_clues_around(Board* b, uint16_t row, uint16_t col)
{
	if (row > NUM_ROWS)
	{
		printf("Error: Trying to add clue to an invalid row (%i)\n", row);
	}
	if (col > NUM_COLS)
	{
		printf("Error: Trying to add clue to an invalid col (%i)\n", col);
	}

	uint16_t index = row*NUM_COLS + col;

	// left and right
	if (col != 0)
	{
		add_clue(b, index - 1);
	}
	if (col != NUM_COLS - 1)
	{
		add_clue(b, index + 1);
	}

	// top left, top, top right
	if (row != 0)
	{
		if (col != 0)	
		{
			add_clue(b, index - NUM_COLS - 1);
		}
		add_clue(b, index - NUM_COLS);
		if (col != NUM_COLS - 1)
		{
			add_clue(b, index - NUM_COLS + 1);
		}
	}


	// bottom left, bottom, bottom right
	if (row != NUM_ROWS - 1)
	{
		if (col != 0)
		{
			add_clue(b, index + NUM_COLS - 1);
		}
		add_clue(b, index + NUM_COLS);
		if (col != NUM_COLS - 1)
		{
			add_clue(b, index + NUM_COLS + 1);
		}
	}
}

bool is_flag(Board* b, uint16_t row, uint16_t col)
{
	uint16_t index = row*NUM_COLS + col;
	int value = b->board[index];
	if (value >= FLAG_OFFSET && value < DISCOVERED_OFFSET)
	{
		return true;
	}
	return false;
}

bool is_discovered(Board* b, uint16_t row, uint16_t col)
{
	uint16_t index = row*NUM_COLS + col;
	int value = b->board[index];
	if (value >= DISCOVERED_OFFSET)
	{
		return true;
	}
	return false;
}

void flag_location(Board* b, uint16_t row, uint16_t col)
{
	if (!b->flags_remaining)
	{
		printf("You are out of flags! Please unflag another location and try again.\n");
	}

	uint16_t index = row*NUM_COLS + col;
	if (is_discovered(b, row, col))
	{
		// can't flag a coordinate that has already been explored
		printf("Error: You can't flag a coordinate that has already been explored.\n");
		return;
	}

	if (!is_flag(b, row, col))
	{
		b->board[index] += FLAG_OFFSET;
		b->flags_remaining--;
	}
	else
	{
		b->board[index] -= FLAG_OFFSET;
		b->flags_remaining++;
	}

}

void add_clue(Board* b, uint16_t index)
{
	if (index > 0 && index < NUM_ROWS*NUM_COLS)
	{
		if (!is_mine(b, index))
		{
			b->board[index] += 1;
#ifdef DEBUG
			printf("adding clue to index %d\n", index);
#endif
		}
		else
		{
#ifdef DEBUG
			printf("can't add clue to index %d, it is a mine\n", index);
#endif
		}
	}
	else
	{
#ifdef DEBUG
		printf("index %d out of bounds in add_clue()\n", index);
#endif
	}
}


void print_board(Board* b, bool show_all)
{
	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLS; j++)
		{
			print_piece(b->board[i*NUM_ROWS + j], show_all);
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
}

void print_piece(int piece, bool show_all)
{
	if (show_all)
	{
		if (piece >= FLAG_OFFSET && piece < DISCOVERED_OFFSET)
		{
			piece -= FLAG_OFFSET;
		}
		if (piece < DISCOVERED_OFFSET)
		{
			piece += DISCOVERED_OFFSET;
		}
	}

	if (piece >= FLAG_OFFSET && piece < DISCOVERED_OFFSET)
	{
		printf("f");
	}
	else if (piece < DISCOVERED_OFFSET)
	{
		printf("?");
	}
	else
	{
		piece -= DISCOVERED_OFFSET;

		if (piece == 0)
		{
			printf("-");
		}
		else if (piece == MINE)
		{
			printf("!");
		}
		else 
		{
			printf("%d", piece);
		}
	}
}


void free_board(Board* b)
{
	free(b);
}
