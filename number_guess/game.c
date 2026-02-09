#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"

static int* make_game_numbers(int number_cnt) {
	int numbers[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, cnt=0, index;
	int* game_numbers = (int*)malloc(sizeof(int) * number_cnt);
	srand((unsigned)time(NULL));
	while (cnt != number_cnt) {
		index = rand() % 10;
		if (numbers[index] != -1) {
			game_numbers[cnt++] = numbers[index];
			numbers[index] = -1;
		}
	}

	return game_numbers;
}

int* read_number(int* user_input_numbers, int number_cnt) {
	int c, i=0;
	while ((c = getchar()) != '\n') {
		if (c == ' ') continue;
		if (c > 57 || 48 > c) {
			return NULL;
		}
		// 숫자 읽어들일 부분
	}
	return NULL;
}

void play_alone(int number_cnt) {
	int* game_numbers = make_game_numbers(number_cnt);
	int* user_input_numbers = (int*)malloc(sizeof(int) * number_cnt);
	char answer[6], user_input_number[11];
	int i;
	printf("숫자 게임에 오신 것을 환영합니다.\n모드 : 혼자\n숫자 갯수: %d\n숫자는 붙여서 써도 되고 띄워서 써도 됩니다.\nex) 123 or 1 2 3 or 12 3 모두 동일합니다.\n", number_cnt);
	while (1) {
		i = 0;
		printf("숫자 입력 : ");

		
	}

	free(user_input_numbers);
	free(game_numbers);
}