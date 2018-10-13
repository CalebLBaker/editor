#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#define BLOCK_SIZE 1024
#define BLOCK_END 1023
#define TAB_WIDTH 4
#define MOVEMENT 0
#define CHAR_ADDED 1
#define NEWLINE_ADDED 2

typedef struct Block Block;

struct Block {
	char *data;
	Block *prev;
	Block *next;
	unsigned short size;
};

Block* newBlock(Block *prev, Block *next) {
	Block *ret = (Block*)malloc(sizeof(Block));
	ret->data = (char*)malloc(BLOCK_SIZE * sizeof(char));
	ret->prev = prev;
	ret->next = next;
	ret->size = 0;
}

void insert(Block *b, unsigned short i, char c) {
	char *data = b->data;
	char last_of_block = data[BLOCK_END];
	char *pos = data + i;
	memmove(pos + 1, pos, b->size - i);
	data[i] = c;
	b->size++;
}

int main(int argc, char **argv) {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	refresh();
	Block start;
	start.data = (char*)malloc(BLOCK_SIZE);
	start.prev = NULL;
	start.next = NULL;
	start.size = 0;
	Block *cursor_block = &start;
	unsigned short cursor_pos = 0;
	Block *screen_block = cursor_block;
	unsigned short screen_pos = 0;
	unsigned y = 0;
	unsigned x = 0;
	while (true) {
		int c = getch();
		char event = CHAR_ADDED;
		unsigned temp_x = x;
		switch (c) {
			case (KEY_LEFT) : {
				event = MOVEMENT;
				if (cursor_pos)  {
					x--;
					cursor_pos--;
				}
				break;
			}
			case (KEY_RIGHT) : {
				event = MOVEMENT;
				if (cursor_pos != cursor_block->size) {
					x++;
					cursor_pos++;
				}
				break;
			}
			case ('\n') : {
				event = NEWLINE_ADDED;
				x = 0;
				y++;
				break;
			}
			case ('\t') : {
				x += TAB_WIDTH - x % TAB_WIDTH;
				break;
			}
			default : {
				x++;
			}
		}
		if (event) {
			insert(cursor_block, cursor_pos, (char)c);
			char *data = cursor_block->data;
			if (event == CHAR_ADDED) {
				for (unsigned short i = cursor_pos; i < cursor_block->size; i++) {
					c = (int)data[i];
					if (c == '\n') {
						break;
					}
					else if (c == '\t') {
						temp_x += TAB_WIDTH - temp_x % TAB_WIDTH;
					}
					else {
						mvaddch(y, temp_x, c);
						temp_x++;
					}
				}
				cursor_pos++;
			}
		}
		move(y, x);
		refresh();
	}
	endwin();
}
