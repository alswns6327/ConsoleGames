#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "common.h"
#include "game_io.h"
#include "security.h"

typedef enum {
	GAME_ANSWER_WRONG,
	GAME_ANSWER_CORRECT
} GameAnswer;

typedef struct {
	int strike_cnt;
	int ball_cnt;
	GameAnswer answer;
} GuessResult;

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
	int pos[10] = { 0 };
	while ((c = getchar()) != '\n' && i < number_cnt) {
		if (c == ' ') continue;
		if (c > 57 || 48 > c) {
			clear_input_buffer();
			break;
		}
		c -= 48;
		if (pos[c]) { // 중복되는 경우
			clear_input_buffer();
			break; 
		}
		user_input_numbers[i++] = c;
		pos[c]++;
	}
	if (i < number_cnt) return NULL;
	if (c != '\n') {
		clear_input_buffer();
		return NULL;
	}
	return user_input_numbers;
}

GuessResult make_answer(int* game_numbers, int* user_input_numbers, int number_cnt) {
	GuessResult result = { 0, 0, GAME_ANSWER_WRONG };
	int pos[10];
	for (int i = 0; i < 10; i++) pos[i] = - 1;
	for (int i = 0; i < number_cnt; i++) pos[game_numbers[i]] = i;

	for (int i = 0; i < number_cnt; i++) {
		if (pos[user_input_numbers[i]] != -1) {
			if (pos[user_input_numbers[i]] == i) result.strike_cnt++;
			else result.ball_cnt++;
		}
	}
	if (result.strike_cnt == number_cnt) result.answer = GAME_ANSWER_CORRECT;
	return result;
}

void play_alone(int number_cnt) {
	int* game_numbers = make_game_numbers(number_cnt);
	int* user_input_numbers = (int*)malloc(sizeof(int) * number_cnt);
	char regame_check[4];
	char save_check[5];
	int tryCount = 0;
	printf("\n숫자 게임에 오신 것을 환영합니다.\n모드 : 혼자\n숫자 갯수: %d\n\n숫자는 붙여서 써도 되고 띄워서 써도 됩니다.\nex) 123 or 1 2 3 or 12 3 모두 동일합니다.\n", number_cnt);
	while (1) {
		printf("숫자 입력 : ");
		if (read_number(user_input_numbers, number_cnt) == NULL) {
			printf("\n%d개의 숫자를 중복 없이 입력해주세요.\n", number_cnt);
			continue;
		}
		GuessResult result = make_answer(game_numbers, user_input_numbers, number_cnt);
		tryCount++;
		if (result.answer == GAME_ANSWER_CORRECT) {
			printf("\n축하드립니다 %d번 만에 정답을 맞추셨습니다.\n", tryCount);
			printf("\n게임 기록을 저장하시려면 yes를 입력 아니라면 엔터를 눌러주세요.\n");
			fgets(save_check, sizeof(save_check), stdin);
			if (save_check[strlen(save_check) - 1] != '\n') clear_input_buffer();
			else save_check[strlen(save_check) - 1] = '\0';
			if (!strcmp(save_check, "yes")) {
				printf("\n게임 기록과 함께 저장될 닉네임을 입력해주세요.\n");
				char nickname[22];
				while (1) {
					fgets(nickname, sizeof(nickname), stdin);
					size_t len = strlen(nickname);
					if (nickname[len - 1] != '\n') {
						printf("\n영어 닉네임은 20자 한글 닉네임은 10자 이내로 입력해주세요.\n");
						clear_input_buffer();
					}
					else {
						nickname[len - 1] = '\0';
						break;
					}
				}
				printf("\n10자 이상 20자 이하의 영문/숫자로 된 password를 입력해주세요.\n");
				char password[22];
				while (1) {
					fgets(password, sizeof(password), stdin);

					size_t len = strlen(password);
					if (password[len - 1] != '\n') {
						printf("\n10자 이상 20자 이하의 영문/숫자로 된 password를 입력해주세요.\n");
						clear_input_buffer();
						continue;
					}
					if (has_non_ascii(password) || len < 10) {
						printf("\n10자 이상 20자 이하의 영문/숫자로 된 password를 입력해주세요.\n");
						continue;
					}

					password[len - 1] = '\0';
					break;
				}
				ScoreInfo scoreInfo = { 0 };
				scoreInfo.tryCount = tryCount;
				scoreInfo.password = basic_hash(password);
				strcpy(scoreInfo.nickname, nickname);
				if (!save_score(scoreInfo)) printf("\n저장에 실패하였습니다.\n");
			}
			printf("\n게임을 한 번 더 하시려면 're'를 입력 끝내시려면 엔터를 눌러주세요.\n");
			fgets(regame_check, sizeof(regame_check), stdin);
			if (regame_check[strlen(regame_check) - 1] != '\n') clear_input_buffer();
			else regame_check[strlen(regame_check) - 1] = '\0';
			if (!strcmp(regame_check, "re")) {
				free(game_numbers);
				game_numbers = make_game_numbers(number_cnt);
				char str[42] = "\n플레이 할 숫자의 갯수를 입력해주세요.\n";
				while (1) {
					printf("%s", str);
					scanf("%d%*c", &number_cnt);
					if (2 <= number_cnt && number_cnt <= 10) break;
					strcpy(str, "\n2~10 사이의 수로 정해주세요.\n");
				}
				free(user_input_numbers);
				user_input_numbers = (int*)malloc(sizeof(int) * number_cnt);
				continue;
			} else {
				break;
			}
		}

		printf("%dS %dB\n", result.strike_cnt, result.ball_cnt);
	}

	free(user_input_numbers);
	free(game_numbers);
}

