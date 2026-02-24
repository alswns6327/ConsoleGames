#include <stdio.h>
#include "game_io.h"

void clear_input_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int has_non_ascii(char* str) {
	while (*str) {
		if ((unsigned)*str++ > 127) return 1;
	}
	return 0;
}

int compare_ascending(const void* score1, const void* score2) {
	ScoreInfo* x = (ScoreInfo*) score1;
	ScoreInfo* y = (ScoreInfo*) score2;
	if (x->tryCount > y->tryCount) return 1;
	else if(x->tryCount < y->tryCount) return -1;
	
	if (x->time > y->time) return 1;
	else if (y->time < x->time) return -1;

	return 0;
}

int compare_descending(const void* score1, const void* score2) {
	ScoreInfo* x = (ScoreInfo*)score1;
	ScoreInfo* y = (ScoreInfo*)score2;
	if (x->tryCount < y->tryCount) return 1;
	else if (x->tryCount > y->tryCount) return -1;

	if (x->time < y->time) return 1;
	else if (y->time > x->time) return -1;

	return 0;
}