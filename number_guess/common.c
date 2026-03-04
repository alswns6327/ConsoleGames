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

int big_rand() {
	return (rand() << 15) | rand();
}

void input_number(int* num, int min, int max) {
	while (1) {	
		if (scanf("%d", num) != 1) {
			printf("\n숫자만 입력 가능합니다.\n");
			clear_input_buffer();
			continue;
		}
		if (*num < min || *num > max) printf("\n%d~%d 사이의 수를 입력해주세요.\n", min, max);
		else break;
	}
	clear_input_buffer();
}

void make_answer_yes_no(char* answer) {
	size_t len;
	int check;
	while (1) {
		fgets(answer, sizeof(answer), stdin);
		len = strlen(answer);
		check = 1;
		if (answer[len - 1] != '\n') {
			check = 0;
			clear_input_buffer();
		}
		else if (len < 3) check = 0;
		else answer[len - 1] = '\0';

		if (!check) {
			printf("\nyes or no를 입력해주세요.\n");
			continue;
		}

		for (int i = 0; i < len - 1; i++) {
			if (answer[i] < 97) answer[i] += 32;
		}
		if (strcmp(answer, "yes") * strcmp(answer, "no")) {
			printf("\nyes or no를 입력해주세요.\n");
			continue;
		}
		else break;
	}
}
