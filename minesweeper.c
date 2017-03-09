#include <stdio.h>
#include "board.h"
#include "game.h"


int main()
{
	Board* b = init_board();
	print_board(b, false);
#ifdef DEBUG
	print_board(b, true);
#endif

	char str[100]; // to store user input

	while(1)
	{
		memset(str, '\0', sizeof(str));
		printf("Please enter a command to proceed: (h to see command list)\n");
		gets(str);
		handle_input(b, str);
		if (is_win(b))
		{
			win();
		}
	}

	free_board(b);
	return 0;
}
