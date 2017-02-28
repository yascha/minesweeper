#include "board.h"
#include "game.h"



void explore_location(Board* b, uint16_t row, uint16_t col)
{
	if (row > NUM_ROWS)
	{
#ifdef DEBUG
		printf("Error: Trying to explore invalid row (%d)\n", row);
#endif
		return;
	}
	if (col > NUM_COLS)
	{
#ifdef DEBUG
		printf("Error: Trying to explore invalid col (%d)\n", col);
#endif
		return;
	}
	
	uint16_t index = row*NUM_COLS + col;
	
	int value = b->board[index];

	if (value == MINE)
	{
		lose(b);
	}

	if (value < DISCOVERED_OFFSET)
	{
		b->board[index] += DISCOVERED_OFFSET;
	}

	if (value == 0)
	{
		// If this square is empty and was unexplored, 
		// we need to explore all other adjacent (even diagonally) 
		// neighbours and those neighbours' neighbours

		// explore clockwise from top left

		// top left, top, top right
	        if (row != 0)
        	{
                	if (col != 0)
	                {
	                        explore_location(b, row - 1, col - 1);
	                }
	                explore_location(b, row - 1, col);
	                if (col != NUM_COLS - 1)
	                {
	                        explore_location(b, row - 1, col + 1);
	                }
	        }

		// right
		if (col != NUM_COLS - 1)
		{
			explore_location(b, row, col + 1);
		}

		// bottom right, bottom, bottom left
		if (row != NUM_ROWS - 1)
	        {
			if (col != NUM_COLS - 1)
			{
				explore_location(b, row + 1, col + 1);
			}
			explore_location(b, row + 1, col);

			if (col != 0)
			{
				explore_location(b, row + 1, col - 1);
			}
		}

		// left
		if (col != 0)
		{
			explore_location(b, row, col - 1);
		}
	}
}



void handle_input(Board* b, char* input)
{

	int row;
	int col;

	if (!strlen(input))
	{
		printf("Error: Invalid input received\n");
	}

	switch (input[0])
	{
		case 'h':
			printf("\n");
			printf("\th:\t\tview this command list\n");
			printf("\te:\t\texplore a coordinate (you will be prompted to enter coordinates)\n");
			printf("\tp:\t\tprint the map\n");
			printf("\tf:\t\tflag a cooardinate (you will be prompted to enter coordinates)\n");
			printf("\tq:\t\tquit\n");
			break;
		case 'p':
			print_board(b, false);
			break;
		case 'q':
			exit(0);
			break;
		case 'f':
                        printf("Enter row and col to flag (enter coordinate values with a space between ie. 5 6). Note that 0, 0 is top left.: ");
                        scanf("%d %d", &row, &col);
                        printf("\n");
                        flag_location(b, row, col);
                        printf("\n");
                        print_board(b, false);
                        break;
		case 'e':
                        printf("Enter row and col to flag (enter coordinate values with a space between ie. 5 6). Note that 0, 0 is top left.: ");
			scanf("%d %d", &row, &col);
			printf("\n");
			explore_location(b, row, col);
			printf("\n");
			print_board(b, false);
			break;
		// secret commands
                case 'y':
                        print_board(b, true);
                        break;
		case 'w':
			win();
			break;	
		default:
			printf("Invalid command entered, please try again.\n");
			break;
	}
}


bool is_win(Board* b)
{
	// win occurs if all mines are flagged
	for (int i = 0; i < NUM_MINES; i++)
	{
		uint16_t mine_index = b->mine_indices[i];
		uint16_t row = mine_index/NUM_COLS; 
		uint16_t col = mine_index % NUM_COLS;
		if (!is_flag(b, row, col))
		{
			return false;
		}
	}
	return true;
	
}


void lose(Board* b)
{
	printf("Oops!  You stepped on a mine.\n");
	printf("Better luck next time!\n");
	print_board(b, true);
}

void win()
{
	printf("==========================================\n");
	printf("=                                        =\n");
	printf("=           Congratulations!             =\n");
	printf("=           You win! You are             =\n");
	printf("=           a winner!                    =\n");
	printf("=                                        =\n");
	printf("==========================================\n");
	exit(0);
}



