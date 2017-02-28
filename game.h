#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void explore_location(Board* b, uint16_t row, uint16_t col);
void handle_input(Board* b, char* str);
void win();
bool is_win(Board* b);
void lose(Board* b);

